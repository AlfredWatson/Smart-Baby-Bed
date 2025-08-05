#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#define BODY_TEMP_MAX 37.3
#define ENV_TEMP_MAX 26
#define ENV_TEMP_MIN 20
#define ENV_HUMI_MAX 60
#define ENV_HUMI_MIN 30
#define ENV_MQ 5

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
  short face;
  bool eye;
  bool cover;
}serverData;

const char* ssid = "ciwater";
const char* password = "20001210";
const char* mqttServer = "test.mosquitto.org";
const String topic1 = "SenToSer:48:3F:DA:9B:E8:C7";
const String topic2 = "SerToSen:24:D7:EB:CC:CB:A7";

WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);
esp01sData e_d = {false,false,36.5,27,40,2.0,1.5};
serverData s_d = {-1,false,true};

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  connectWifi();
  mqttClient.setServer(mqttServer, 1883);
  mqttClient.setCallback(receiveCallback);
  connectMQTTserver();
}

void loop() {
  if (mqttClient.connected()) {
    Serial.println("--in main--");
    if(e_d.sw520d){
      Serial.println("床倒了");
    }
    if(e_d.mhrc){
      Serial.println("尿了");
    }
    if(e_d.bodytemp > BODY_TEMP_MAX){
      Serial.println("发烧了");
    }
    if(e_d.envtemp > ENV_TEMP_MAX){
      Serial.println("太热了");
    } else if(e_d.envtemp < ENV_TEMP_MIN){
      Serial.println("太冷了");
    }
    if(e_d.envhemi > ENV_HUMI_MAX){
      Serial.println("太湿了");
    } else if(e_d.envhemi < ENV_HUMI_MIN){
      Serial.println("太干了");
    }
    if(e_d.mq135 > ENV_MQ){
      Serial.println("漏了");
    }
    
    delay(1000);
    mqttClient.loop();
  } else {
    connectMQTTserver();
  }
}

/*
 * 函数名：connectWifi()
 * 函数功能：连接wifi
 */
void connectWifi() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected!");
}

/*
 * 函数名：connectMQTTserver()
 * 函数功能：连接MQTT服务器并订阅信息
 */
void connectMQTTserver() {
  // 根据ESP8266的MAC地址生成客户端ID
  String clientId = "esp8266nodemcu:" + WiFi.macAddress();
  // 连接MQTT服务器
  if (mqttClient.connect(clientId.c_str())) {
    Serial.println("MQTT server connected!");
    Serial.print("client id: ");Serial.println(clientId);
    Serial.print("server address: ");Serial.println(mqttServer);
    // 订阅指定主题
    subscribeTopic(topic1);
    subscribeTopic(topic2);
  } else {
    Serial.print("MQTT server connect failed! client state: ");
    Serial.println(mqttClient.state());
    delay(3000);
  }
}

/*
 * 函数名：subscribeTopic()
 * 函数功能：订阅指定主题
 */
void subscribeTopic(String topicString) {
  // 建立订阅主题 esp12e subscribe esp01s
//  String topicString = "SenToSer:48:3F:DA:9B:E8:C7";
  char subTopic[topicString.length() + 1];
  strcpy(subTopic, topicString.c_str());
  Serial.print("subscribe topic:");Serial.print(subTopic);
  if (mqttClient.subscribe(subTopic)) {
    Serial.println(",succeed!");
  } else {
    Serial.println(",failed!");
  }
}

/*
 * 
 */
void charArrayToInt(int* result, byte* subPayload, int start_i, char end_char, int* end_i){
  int i = start_i;
  for(; (char)subPayload[i] != end_char; i++){
    *result = *result * 10 + (char)subPayload[i] - '0';
  }
  *end_i = i;
}

/*
 * 
 */
void charArrayToFloat(float* result, byte* subPayload, int start_i, char end_char, int* end_i){
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

/*
 * 
 */
void charArrayToFloat(float* result, byte* subPayload, int start_i, int end_i){
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
void receiveCallback(char* topic, byte* payload, unsigned int length) {
  char token = (char)topic[9];
  if(token == '4'){
    Serial.println("--from esp01s--");
    // 角度
    if((char)payload[0] == '0'){
      e_d.sw520d = false;
    } else{
      e_d.sw520d = true;
    }
    // 尿床
    if((char)payload[2] == '1'){
      e_d.mhrc = false;
    } else{
      e_d.mhrc = true;
    }
    
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
    charArrayToFloat(&e_d.mq135, payload, i+1, ',', &i);
    Serial.print("mq135:");Serial.println(e_d.mq135);

    // 声音
    e_d.czn15e = 0;
    charArrayToFloat(&e_d.czn15e, payload, i+1, length);
    Serial.print("czn15e:");Serial.println(e_d.czn15e);
    
  } else if(token == '2'){
    Serial.println("from server");
    s_d.face = (char)payload[0] - '0';
    Serial.print(s_d.face);
    Serial.println("=======");
    
  } else{
    Serial.print("received strange message:");
    for (int i = 0; i < length; i++) {
      Serial.print((char)payload[i]);
    }
  }
  Serial.println();
}
