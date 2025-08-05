/**
 * 使用PubSubClient库
 * ESP8266向MQTT服务器发布信息
 */
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Ticker.h>

// 设置wifi接入信息
const char* ssid = "ohhhhh";
const char* password = "20001210";
// 设置MQTT服务器(然也物联)
const char* mqttServer = "test.ranye-iot.net";

Ticker ticker;
WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);
// Ticker计数用变量
int count = 0;
double temp = 28.30;
double humt = 1.03;

void setup() {
  Serial.begin(115200);
  //设置ESP8266工作模式为无线终端模式
  WiFi.mode(WIFI_STA);
  // 连接WiFi
  connectWifi();
  // 设置MQTT服务器和端口号
  mqttClient.setServer(mqttServer, 1883);
  // 设置MQTT订阅回调函数
  mqttClient.setCallback(receiveCallback);
  // 连接MQTT服务器
  connectMQTTServer();
  // Ticker定时对象
  ticker.attach(1, tickerCount);
}

void loop() {
  // 如果开发板成功连接服务器
  count++;
  if (mqttClient.connected()) {
    // 每隔8秒钟发布一次信息
    if (count %8 == 0) {
      pubMQTTmsg();
      delay(5000);
    }
    // 保持心跳
    mqttClient.loop();
  } else {// 如果开发板未能成功连接服务器
    // 再次连接服务器
    connectMQTTServer();
  }
}

void tickerCount() {
  ++count;
}

void connectMQTTServer() {
  // 根据ESP8266的MAC地址生成客户端ID（避免与其它ESP8266的客户端ID重名）
  String clientId = "esp8266-" + WiFi.macAddress();

  // 连接MQTT服务器
  if (mqttClient.connect(clientId.c_str())) {
    Serial.println("MQTT Server 连接成功！");
    Serial.print("Server Address(MQTT服务器地址): ");Serial.println(mqttServer);
    Serial.print("ClientId(客户端id): ");Serial.println(clientId);
    // 订阅指定主题
    subscribeTopic();
  } else {
    Serial.print("MQTT Server 连接失败！ Client State:");
    Serial.println(mqttClient.state());
    delay(3000);
  }
}

// 收到信息后的回调函数
void receiveCallback(char* topic, byte* payload, unsigned int length) {
  Serial.print("[");Serial.print(topic);
  Serial.print("]主题收到消息: ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println("");
  Serial.print("消息长度(Bytes): ");
  Serial.println(length);
}

// 订阅指定主题
void subscribeTopic() {
  // 主题名称以CiWater-ServerToSensor为前缀，后面添加设备的MAC地址。
  String topicString = "CiWater-ServerToSensor" + WiFi.macAddress();
  char subTopic[topicString.length() + 1];
  strcpy(subTopic, topicString.c_str());

  // 通过串口监视器输出是否成功订阅主题以及订阅的主题名称
  Serial.print("订阅主题: ");Serial.print(subTopic);
  if (mqttClient.subscribe(subTopic)) {
    Serial.println(" 成功!");
  } else {
    Serial.println(" 失败!");
  }
}

// 发布信息
void pubMQTTmsg() {
  temp += 0.01;humt += 0.01;
  
  // 发布主题名称以CiWater-SensorToServer-为前缀，后面添加设备的MAC地址。
  String topicString = "CiWater-SensorToServer-" + WiFi.macAddress();
  char publishTopic[topicString.length() + 1];
  strcpy(publishTopic, topicString.c_str());

  // 建立需要发布信息
  //String messageString = "{\n\"hum\": \"" + String(value++) + "\",\n\"tem\": \"" + String(value++) + "\"\n}";
//  {
//  "utcTime": "xxx",
//  "deviceId": "xxx",
//  "sensorHumidity": "xxx",
//  "sensorTemp": "xxx"
//  }
  String messageString = "{\n\"utcTime\": \"" + String(count) + "\",\n"
                         + "\"deviceId\": \"8\",\n"
                         + "\"sensorHumidity\": \"" + String(temp) + "\",\n"
                         + "\"sensorTemp\": \"" + String(humt) + "\"\n"
                         + "}";

  char publishMsg[messageString.length() + 1];
  strcpy(publishMsg, messageString.c_str());

  // 实现ESP8266向主题发布信息
  if (mqttClient.publish(publishTopic, publishMsg)) {
    Serial.print("Publish Topic(向主题发布消息):"); Serial.println(publishTopic);
    //Serial.println("Publish message:"); Serial.println(publishMsg);
  } else {
    Serial.println("Message Publish Failed.");
  }
}

void connectWifi() {
  WiFi.begin(ssid, password);
  //等待WiFi连接,成功连接后输出成功信息
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("WiFi Connected!");
}
