#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#define LED 2

const char* ssid = "ciwater";
const char* password = "20001210";
const char* mqttServer = "broker.emqx.io";

WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);
String sensorData = "";

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  connectWifi();
  mqttClient.setServer(mqttServer, 1883);
  mqttClient.setCallback(receiveCallback);
  connectMQTTserver();
  pinMode(LED, OUTPUT);
  //pubMQTTmsg("esp-01s hello world!");
}

void loop() {
  // turn the LED on
  digitalWrite(LED, HIGH);
  if (mqttClient.connected()) {
    if(Serial.available() > 0){
      do{
        sensorData += char(Serial.read());
      }while(Serial.available() > 0);
      pubMQTTmsg(sensorData);
      sensorData = "";
    }
    delay(100);
    mqttClient.loop();
  } else {
    connectMQTTserver();
  }
  // turn the LED off
  digitalWrite(LED, LOW);
  ESP.wdtFeed();
}

/*
 * 函数名：connectWifi()
 * 函数功能：连接wifi
 */
void connectWifi() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    //Serial.print(".");
  }
  //Serial.println("WiFi connected!");
}

/*
 * 函数名：connectMQTTserver()
 * 函数功能：连接MQTT服务器并订阅信息
 */
void connectMQTTserver() {
  // 根据ESP-01S的MAC地址生成客户端ID
  String clientId = "esp01s:" + WiFi.macAddress();
  // 连接MQTT服务器
  if (mqttClient.connect(clientId.c_str())) {
    //Serial.println("MQTT server connected!");
    //Serial.print("client id: ");Serial.println(clientId);
    //Serial.print("server address: ");Serial.println(mqttServer);
    // 订阅指定主题
    subscribeTopic();
  } else {
    //Serial.print("MQTT server connect failed! client state: ");
    //Serial.println(mqttClient.state());
    delay(2000);
  }
}

/*
 * 函数名：subscribeTopic()
 * 函数功能：订阅指定主题
 */
void subscribeTopic() {
  // 建立订阅主题
  String topicString = "SerToSen:" + WiFi.macAddress();
  char subTopic[topicString.length() + 1];
  strcpy(subTopic, topicString.c_str());
  //Serial.print("subscribe topic:");
  //Serial.print(subTopic);
  mqttClient.subscribe(subTopic);
//  if (mqttClient.subscribe(subTopic)) {
//    Serial.println(",succeed!");
//  } else {
//    Serial.println(",failed!");
//  }
}

/*
 * 函数名：pubMQTTmsg(String sensorData)
 * 函数功能：发布信息
 */
void pubMQTTmsg(String sensorData) {
  // 发布主题
  String topicString = "SenToSer:" + WiFi.macAddress();
  char publishTopic[topicString.length() + 1];
  strcpy(publishTopic, topicString.c_str());

  // 建立需要发布信息
  String messageString = sensorData;
  
  // 将string转化为char数组
  char publishMsg[messageString.length() + 1];
  strcpy(publishMsg, messageString.c_str());

  // 向某主题发布信息
  if (mqttClient.publish(publishTopic, publishMsg)) {
    // 成功发布消息之后，通过串口给pico发送反馈信息
    //Serial.print("Publish Message. Topic:");
    //Serial.println(publishTopic);
    Serial.print(1101);
  } else {
    //Serial.println("Message Publish Failed!");
    Serial.print(1102);
  }
}

/*
 * 函数名：receiveCallback(char* topic, byte* payload, unsigned int length)
 * 函数功能：收到信息后的回调函数
 */
void receiveCallback(char* topic, byte* payload, unsigned int length) {
  //Serial.print("[");
  //Serial.print(topic);
  //Serial.print("]主题收到消息: ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  //Serial.println("");
  //Serial.print("消息长度(Bytes): ");
  //Serial.println(length);
}
