#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>

#define WIFI_NAME "ohhhhh"
#define WIFI_PASSWORD "20001210"

const char* host = "192.168.43.129"; //需要访问的域名
const int httpsPort = 8080;  // 需要访问的端口
const String url = "/test01";   // 需要访问的地址

void setup() {
  Serial.begin(115200);    
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WIFI_NAME);
  WiFi.begin(WIFI_NAME, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED){
      delay(1000);
      Serial.print("...Please wait...");
      Serial.println();
  }
  Serial.println("WiFi Connected!");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  Serial.print("connecting to ");
  Serial.println(host);
  
  WiFiClient client;
  
  String postRequest =(String)("GET ") + url 
                      + " HTTP/1.1\r\n" 
                      + "Content-Type: text/html;charset=utf-8\r\n" 
                      + "Host: " + host + "\r\n" 
                      + "User-Agent: BuildFailureDetectorESP8266\r\n" 
                      + "Connection: close\r\n\r\n";
                      //+ "Connection: Keep Alive\r\n\r\n"; 
                       
  if (!client.connect(host, httpsPort)){
    Serial.println("connection failed"); 
    return;
  } else{
    delay(10);
  }
  Serial.println();
  Serial.print("posting request...");
  Serial.println();
  Serial.println(postRequest);
  // 发送HTTP请求
  client.print(postRequest);
  
  String line = client.readStringUntil('\n');
  while(line.length() != 0){
    Serial.println(line);
    line = client.readStringUntil('\n');
  }
  Serial.println(line);
  client.stop();
  delay(3000);
}
