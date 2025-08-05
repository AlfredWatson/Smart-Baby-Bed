#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "Adafruit_ssd1306syp.h"

#define BODY_TEMP_MAX 37.3
#define ENV_TEMP_MAX 30
#define ENV_TEMP_MIN 20
#define ENV_HUMI_MAX 60
#define ENV_HUMI_MIN 30
#define ENV_MQ 3
#define ENV_SOUND 3

#define BLUE_LED D5
#define BUZZ D6
#define SDA_PIN D0
#define SCL_PIN D1
Adafruit_ssd1306syp display(SDA_PIN, SCL_PIN);

typedef struct{
  bool sw520d;
  bool mhrc;
  float bodytemp;
  int envtemp;
  int envhemi;
  float mq135;
  float czn15e;
}esp01sData;

typedef struct{
  short face;//cry:0 or smile:1 or sleep:2 or week:0,1
  bool cover;// cover or uncover
}serverData;

const char* ssid = "ciwater";
const char* password = "20001210";
//const char* mqttServer = "test.mosquitto.org";
const char* mqttServer = "broker.emqx.io";
//const char* mqttServer = "test.ranye-iot.net";
const String topic1 = "SenToSer:48:3F:DA:9B:E8:C7";
const String topic2 = "SerToSen:24:D7:EB:CC:CB:A7";

WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);
esp01sData e_d = {false,false,36.5,25,40,1.5,1.5};
serverData s_d = {-1,true};
bool buzz_flag[6] = {false,false,false,false,false,false};
short buzz_flag_count[6] = {0,0,0,0,0,0};

void connectWifi() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
}

void subscribeTopic(String topicString) {
  char subTopic[topicString.length() + 1];
  strcpy(subTopic, topicString.c_str());
  display.print("subscribe topic:");display.print(subTopic);
  if (mqttClient.subscribe(subTopic)) {
    display.println(",succeed!");
  } else {
    display.println(",failed!");
  }
}

void connectMQTTserver() {
  // 根据MAC地址生成客户端ID
  String clientId = "esp8266nodemcu:" + WiFi.macAddress();
  // 连接MQTT服务器
  if (mqttClient.connect(clientId.c_str())) {
    display.println("MQTT server connected!");
    display.print("client id: ");display.println(clientId);
    display.print("server address: ");display.println(mqttServer);
    // 订阅指定主题
    subscribeTopic(topic1);
    subscribeTopic(topic2);
  } else {
    display.print("MQTT server connect failed! client state: ");
    display.println(mqttClient.state());
    delay(1000);
  }
}

/**
 * 在char型数组*subPayload中，
 * 截取从第*start_i个字符开始到end_char字符结束的子串
 * (该子串必须为数字字符组成的串)，
 * 并其整个转化为int类型并保存到*result中。
 * 并将end_char字符的下标保存在*end_i中。
 */
void charArrayToInt(int* result, unsigned char* subPayload, int start_i, char end_char, int* end_i){
  int i = start_i;
  for(; (char)subPayload[i] != end_char; i++){
    *result = *result * 10 + (char)subPayload[i] - '0';
  }
  *end_i = i;
}

/**
 * 在char型数组*subPayload中，
 * 截取从第*start_i个字符开始到end_char字符结束的子串
 * (该子串必须为数字字符组成的串)，
 * 并其整个转化为float类型并保存到*result中。
 * 并将end_char字符的下标保存在*end_i中。
 */
void charArrayToFloat(float* result, unsigned char* subPayload, int start_i, char end_char, int* end_i){
  bool dotFlag = false;
  int decimalTimes = 10;
  int i = start_i;
  for(; (char)subPayload[i] != end_char; i++){
    if(!dotFlag){ // 整数部分
        if((char)subPayload[i] != '.'){
          *result = *result * 10 + (char)subPayload[i] - '0';
        } else {
          dotFlag = true;
        }
      } else { // 小数部分
        *result = *result + ((float)((char)subPayload[i] - '0'))/decimalTimes;
        decimalTimes = decimalTimes * 10;
      }
  }
  *end_i = i;
}

/**
 * 在char型数组*subPayload中，
 * 截取从第start_i个字符开始到第end_i个字符结束的子串
 * (该子串必须为数字字符组成的串)，
 * 并其整个转化为float类型并保存到*result中。
 */
void charArrayToFloat(float* result, unsigned char* subPayload, int start_i, int end_i){
  bool dotFlag = false;
  int decimalTimes = 10;
  for(int i = start_i; i < end_i; i++){
    if(!dotFlag){ // 整数部分
        if((char)subPayload[i] != '.'){
          *result = *result * 10 + (char)subPayload[i] - '0';
        } else {
          dotFlag = true;
        }
      } else { // 小数部分
        *result = *result + ((float)((char)subPayload[i] - '0'))/decimalTimes;
        decimalTimes = decimalTimes * 10;
      }
  }
}

/*
 * 函数名：receiveCallback(char* topic, byte* payload, unsigned int length)
 * 函数功能：收到信息后的回调函数
 */
void receiveCallback(char* topic, unsigned char* payload, unsigned int length) {
  char token = (char)topic[9];
  if(token == '4'){
    Serial.println("--from esp01s--");
    // 角度
    if((char)payload[0] == '0'){e_d.sw520d = false;}
    else{e_d.sw520d = true;}
    // 尿床
    if((char)payload[2] == '1'){e_d.mhrc = false;}
    else{e_d.mhrc = true;}
    int i = 4;
    // 体温
    e_d.bodytemp = 0;
    charArrayToFloat(&e_d.bodytemp, payload, i, ',', &i);
    Serial.print("bodytemp:");Serial.println(e_d.bodytemp);
    // 温度
    e_d.envtemp = 0;
    charArrayToInt(&e_d.envtemp, payload, i+1, ',', &i);
    Serial.print("envtemp:");Serial.println(e_d.envtemp);
    // 湿度
    e_d.envhemi = 0;
    charArrayToInt(&e_d.envhemi, payload, i+1, ',', &i);
    Serial.print("envhemi:");Serial.println(e_d.envhemi);
    // 气体
    e_d.mq135 = 0;
    charArrayToFloat(&e_d.mq135, payload, i+3, ',', &i);
    Serial.print("mq135:");Serial.println(e_d.mq135);
    // 声音
    e_d.czn15e = 0;
    charArrayToFloat(&e_d.czn15e, payload, i+3, length);
    Serial.print("czn15e:");Serial.println(e_d.czn15e);
    
  } else if(token == '2'){
    Serial.println("from server");
    s_d.face = (char)payload[0] - '0';
    Serial.print("state:");Serial.println(s_d.face);
    
  } else{
    Serial.print("received strange message:");
    for (int i = 0; i < length; i++) {
      Serial.print((char)payload[i]);
    }
    Serial.println();
  }
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  pinMode(BLUE_LED, OUTPUT);
  pinMode(BUZZ, OUTPUT);
  digitalWrite(BLUE_LED,LOW);
  digitalWrite(BUZZ,HIGH);
  display.initialize();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  connectWifi();
  display.println("WiFi connected!");
  mqttClient.setServer(mqttServer, 1883);
  mqttClient.setCallback(receiveCallback);
  connectMQTTserver();
  //delay(1000);
}

void loop() {
  display.clear();
  //display.setTextSize(1);
  //display.setTextColor(WHITE);
  display.setCursor(0,0);
  
  if (mqttClient.connected()) {
    // 床倒了
    if(e_d.sw520d){
      digitalWrite(BUZZ, LOW);
      display.setTextColor(BLACK, WHITE);
      display.println(">>bed fall down!");
      display.setTextColor(WHITE);
    }else{
      digitalWrite(BUZZ, HIGH);
    }
    // 发烧了吗
    display.print("body temp:");display.println(e_d.bodytemp);
    if(e_d.bodytemp > BODY_TEMP_MAX){
      display.setTextColor(BLACK, WHITE);
      display.println(">>have a fever!");
      display.setTextColor(WHITE);
      // 报警
      if(!buzz_flag[0]){
        digitalWrite(BUZZ, LOW);digitalWrite(BLUE_LED, HIGH);
        delay(50);
        digitalWrite(BUZZ, HIGH);digitalWrite(BLUE_LED, LOW);
        buzz_flag_count[0]++;
        if(buzz_flag_count[0]==3){
          buzz_flag[0] = true;
          buzz_flag_count[0] = 0;
        }
      }
    }else{
      buzz_flag[0] = false;
    }
    // 尿了吗
    if(e_d.mhrc){
      display.setTextColor(BLACK, WHITE);
      display.println(">>pee!");
      display.setTextColor(WHITE);
      // 报警
      if(!buzz_flag[1]){
        digitalWrite(BUZZ, LOW);digitalWrite(BLUE_LED, HIGH);
        delay(50);
        digitalWrite(BUZZ, HIGH);digitalWrite(BLUE_LED, LOW);
        buzz_flag_count[1]++;
        if(buzz_flag_count[1]==3){
          buzz_flag[1] = true;
          buzz_flag_count[1] = 0;
        }
      }
    }else{
      buzz_flag[1] = false;
    }
    // 哭了吗
    switch(s_d.face){
      case 0:
        display.setTextColor(BLACK, WHITE);
        display.println(">>wake up and crying");
        display.setTextColor(WHITE);
        // 打开LED灯
        digitalWrite(BLUE_LED, HIGH);
        // 蜂鸣器报警
        // 报警
        digitalWrite(BUZZ, LOW);delay(100);digitalWrite(BUZZ, HIGH);
        break;
      case 1:
        display.setTextColor(BLACK, WHITE);
        display.println(">>wake up!");
        display.setTextColor(WHITE);
        // 打开LED灯
        digitalWrite(BLUE_LED, HIGH);
        break;
      case 2:
        display.setTextColor(BLACK, WHITE);
        display.println(">>sleeping.");
        display.setTextColor(WHITE);
        // 关闭LED灯
        digitalWrite(BLUE_LED, LOW);
        break;
      default:
        break;
    }
    // 环境温度
    display.print("temp:");display.print(e_d.envtemp);
    if(e_d.envtemp > ENV_TEMP_MAX){
      display.setTextColor(BLACK, WHITE); // 设置白底黑字
      display.println(":too hot!");
      display.setTextColor(WHITE); // 设置黑底白字
      // 报警
      if(!buzz_flag[3]){
        digitalWrite(BUZZ, LOW);digitalWrite(BLUE_LED, HIGH);
        delay(50);
        digitalWrite(BUZZ, HIGH);digitalWrite(BLUE_LED, LOW);
        buzz_flag_count[3]++;
        if(buzz_flag_count[3]==3){
          buzz_flag[3] = true;
          buzz_flag_count[3] = 0;
        }
      }
    } else if(e_d.envtemp < ENV_TEMP_MIN){
      display.setTextColor(BLACK, WHITE);
      display.println(":too cold!");
      display.setTextColor(WHITE);
      // 报警
      if(!buzz_flag[3]){
        digitalWrite(BUZZ, LOW);digitalWrite(BLUE_LED, HIGH);
        delay(50);
        digitalWrite(BUZZ, HIGH);digitalWrite(BLUE_LED, LOW);
        buzz_flag_count[3]++;
        if(buzz_flag_count[3]==3){
          buzz_flag[3] = true;
          buzz_flag_count[3] = 0;
        }
      }
    } else{
      display.println(".");
      buzz_flag[3] = false;
    }
    // 环境湿度
    display.print("humidity:");display.print(e_d.envhemi);
    if(e_d.envhemi > ENV_HUMI_MAX){
      display.setTextColor(BLACK, WHITE);display.println(":too wet!");display.setTextColor(WHITE);
      if(!buzz_flag[4]){
        digitalWrite(BUZZ, LOW);digitalWrite(BLUE_LED, HIGH);
        delay(50);
        digitalWrite(BUZZ, HIGH);digitalWrite(BLUE_LED, LOW);
        buzz_flag_count[4]++;
        if(buzz_flag_count[4]==3){
          buzz_flag[4] = true;
          buzz_flag_count[4] = 0;
        }
      }
    } else if(e_d.envhemi < ENV_HUMI_MIN){
      display.setTextColor(BLACK, WHITE);display.println(":too dry!");display.setTextColor(WHITE);
      if(!buzz_flag[4]){
        digitalWrite(BUZZ, LOW);digitalWrite(BLUE_LED, HIGH);
        delay(50);
        digitalWrite(BUZZ, HIGH);digitalWrite(BLUE_LED, LOW);
        buzz_flag_count[4]++;
        if(buzz_flag_count[4]==3){
          buzz_flag[4] = true;
          buzz_flag_count[4] = 0;
        }
      }
    } else{
      display.println(".");
      buzz_flag[4] = false;
    }
    // 环境声音
    display.print("sound:");display.println(e_d.czn15e);
    // 环境空气
    if(e_d.mq135 > ENV_MQ){
      display.setTextColor(BLACK, WHITE);display.println("formaldehyde exceeds!");display.setTextColor(WHITE);
      // 报警
      if(!buzz_flag[5]){
        digitalWrite(BUZZ, LOW);digitalWrite(BLUE_LED, HIGH);
        delay(50);
        digitalWrite(BUZZ, HIGH);digitalWrite(BLUE_LED, LOW);
        buzz_flag_count[5]++;
        if(buzz_flag_count[5]==3){
          buzz_flag[5] = true;
          buzz_flag_count[5] = 0;
        }
      }
    } else {
      buzz_flag[5] = false;
    }
    
    //display.setTextSize(2);
    //display.print("0x"); display.println(0xDEADBEEF, HEX);
    display.update();
    delay(2000);
    //display.clear();
    mqttClient.loop();
  } else {
    display.println("connecting mqtt server...");
    display.update();
    connectMQTTserver();
  }
}
