#include "esp_camera.h"
#include "esp_http_client.h"
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

#define CAMERA_MODEL_AI_THINKER // Has PSRAM

#include "camera_pins.h"

const char* ssid = "ciwater";
const char* password = "20001210";
// 默认上传地址
const char* post_url = "http://192.168.43.129:8080/testPhoto2";
// 接收服务器返回信息
static String httpResponseString;

void startCameraServer();
void setupLedFlash(int pin);

// 设置每20秒上传一次图片
int delay_ms = 20000;
WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

void connectWifi() {
  WiFi.begin(ssid, password);
  WiFi.setSleep(false);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected!");
}

/*
 * http请求处理函数
 */
esp_err_t _http_event_handler(esp_http_client_event_t *evt) {
  if (evt->event_id == HTTP_EVENT_ON_DATA) {
    httpResponseString.concat((char *)evt->data);
  }
  return ESP_OK;
}

void setup() {
  Serial.begin(115200);
  Serial.setDebugOutput(true);

  camera_config_t config;
  /*!< GPIO pin for camera power down line */
  config.pin_pwdn = PWDN_GPIO_NUM;
  /*!< GPIO pin for camera reset line */
  config.pin_reset = RESET_GPIO_NUM;
  /*!< GPIO pin for camera XCLK line */
  config.pin_xclk = XCLK_GPIO_NUM;
  /*!< GPIO pin for camera SDA line */
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  /*!< GPIO pin for camera SCL line */
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  /*!< GPIO pin for camera D0--D7 line */
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  /*!< GPIO pin for camera VSYNC line */
  config.pin_vsync = VSYNC_GPIO_NUM;
  /*!< GPIO pin for camera HREF line */
  config.pin_href = HREF_GPIO_NUM;
  /*!< GPIO pin for camera PCLK line */
  config.pin_pclk = PCLK_GPIO_NUM;
  
  /*!< Frequency of XCLK signal, in Hz. EXPERIMENTAL: 
    Set to 16MHz on ESP32-S2 or ESP32-S3 to enable EDMA mode */
  config.xclk_freq_hz = 20000000;
  
  /*!< LEDC timer to be used for generating XCLK  */
  config.ledc_timer = LEDC_TIMER_0;
  /*!< LEDC channel to be used for generating XCLK  */
  config.ledc_channel = LEDC_CHANNEL_0;
  
  /*!< Format of the pixel data: PIXFORMAT_ + YUV422|GRAYSCALE|RGB565|JPEG  */
  config.pixel_format = PIXFORMAT_JPEG; // for streaming
  //config.pixel_format = PIXFORMAT_RGB565; // for face detection/recognition
  /*!< Size of the output image: FRAMESIZE_ + QVGA|CIF|VGA|SVGA|XGA|SXGA|UXGA  */
  config.frame_size = FRAMESIZE_SVGA;

  /*!< Quality of JPEG output. 0-63 lower means higher quality  */
  config.jpeg_quality = 12;
  /*!< Number of frame buffers to be allocated. 
    If more than one, then each frame will be acquired (double speed)  */
  config.fb_count = 1;
  /*!< The location where the frame buffer will be allocated */
  config.fb_location = CAMERA_FB_IN_PSRAM;
  /*!< When buffers should be filled */
  config.grab_mode = CAMERA_GRAB_WHEN_EMPTY;
  
  // if PSRAM IC present, 
  // init with UXGA resolution and higher JPEG quality
  // for larger pre-allocated frame buffer.
  if(config.pixel_format == PIXFORMAT_JPEG){
    if(psramFound()){
      config.jpeg_quality = 10;
      config.fb_count = 2;
      config.grab_mode = CAMERA_GRAB_LATEST;
    } else {
      // Limit the frame size when PSRAM is not available
      config.frame_size = FRAMESIZE_SVGA;
      config.fb_location = CAMERA_FB_IN_DRAM;
    }
  } else {
    // Best option for face detection/recognition
    config.frame_size = FRAMESIZE_240X240;
#if CONFIG_IDF_TARGET_ESP32S3
    config.fb_count = 2;
#endif
  }

  // camera init
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }

  /**
   * @brief Get a pointer to the image sensor control structure
   * @return pointer to the sensor
   */
  sensor_t* s = esp_camera_sensor_get();
  Serial.print("cam pid:");
  Serial.println(s->id.PID);
  // initial sensors are flipped vertically and colors are a bit saturated
  if (s->id.PID == OV3660_PID) {
    s->set_vflip(s, 1); // flip it back
    s->set_brightness(s, 1); // up the brightness just a bit
    s->set_saturation(s, -2); // lower the saturation
  }
  // drop down frame size for higher initial frame rate
  if(config.pixel_format == PIXFORMAT_JPEG){
//    s->set_framesize(s, FRAMESIZE_QVGA);
    // 把图片设置成600*800像素
    s->set_framesize(s, FRAMESIZE_SVGA);
  }

// Setup LED FLash if LED pin is defined in camera_pins.h
#if defined(LED_GPIO_NUM)
  setupLedFlash(LED_GPIO_NUM);
#endif

  // 连接wifi
  connectWifi();
  // 开启摄像头服务器
  startCameraServer();
  Serial.print("Camera server ok, http://");
  Serial.println(WiFi.localIP());
}

void loop() {
  // acquire a frame
  camera_fb_t * fb = esp_camera_fb_get();
  if (!fb){
    Serial.println( "Camera capture failed");
  }else{
    Serial.println( "Camera capture!");
    // 清空http应答报文
    httpResponseString = "";
    
    // HTTP configuration
    esp_http_client_config_t config_client = {0};
    config_client.url = post_url;
    config_client.method = HTTP_METHOD_POST;
    // Network timeout in milliseconds
    config_client.timeout_ms = 25000;
    config_client.event_handler = _http_event_handler;
    
    /**
     * @brief      Start a HTTP session
     *             This function must be the first function to call,
     *             and it returns a esp_http_client_handle_t that you must use as input to other functions in the interface.
     *             This call MUST have a corresponding call to esp_http_client_cleanup when the operation is complete.
     *
     * @param[in]  config   The configurations, see `http_client_config_t`
     *
     * @return
     *     - `esp_http_client_handle_t`
     *     - NULL if any errors
     */
    esp_http_client_handle_t http_client = esp_http_client_init(&config_client);
    
    // 设置http发送的内容和长度
    esp_http_client_set_post_field(http_client, (const char *)fb->buf, fb->len);
    // 设置http头部字段
    esp_http_client_set_header(http_client, "Content-Type", "image/jpg");
    // 发送http请求
    esp_err_t err = esp_http_client_perform(http_client);

    if (err == ESP_OK) {
        Serial.println("ESP_OK");
    } else {
      Serial.println("ESP_NOT_OK");
    }
    esp_http_client_cleanup(http_client);
    esp_camera_fb_return(fb);
  }
  //mqttClient.loop();
  delay(delay_ms);
}
