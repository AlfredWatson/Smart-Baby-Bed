/*
 * 使用PubSubClient库使用ESP8266向连接MQTT服务器
 */
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// 设置wifi接入信息
const char* ssid = "ohhhhh";
const char* password = "20001210";
// 设置MQTT服务器(然也物联)
const char* mqttServer = "test.ranye-iot.net";

WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

void setup() {
  Serial.begin(115200);
  // 设置ESP8266工作模式为无线终端模式
  WiFi.mode(WIFI_STA);
  // 连接WiFi
  connectWifi();
  // 设置MQTT服务器和端口号
  mqttClient.setServer(mqttServer, 1883);
  // 连接MQTT服务器
  connectMQTTServer();
}

void loop() {
  // 如果开发板成功连接服务器
  if (mqttClient.connected()) {
    // 保持客户端心跳
    mqttClient.loop();
  } else {// 如果开发板未能成功连接服务器
    // 再次连接服务器
    connectMQTTServer();
  }
}

void connectMQTTServer() {
  // 根据ESP8266的MAC地址生成客户端ID（避免与其它ESP8266的客户端ID重名）
  String clientId = "esp8266-" + WiFi.macAddress();

  // 连接MQTT服务器
  if (mqttClient.connect(clientId.c_str())) {
    Serial.println("MQTT Server 连接成功！");
    Serial.print("Server Address: ");Serial.println(mqttServer);
    Serial.print("ClientId:");Serial.println(clientId);
  } else {
    Serial.print("MQTT Server 连接失败！ Client State:");
    Serial.println(mqttClient.state());
    delay(3000);
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
