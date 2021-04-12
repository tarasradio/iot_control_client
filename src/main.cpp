#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

/* Установите здесь свои SSID и пароль */
const char* ssid = "Lexa";       // SSID
const char* password = "web-zlo.py";  // пароль

uint8_t LED1pin = D7;
String LED1status = "off";

uint8_t LED2pin = D6;
String LED2status = "off";

unsigned long lastTime = 0;
unsigned long timerDelay = 200;

String httpGETRequest(const char* serverName);
void changeLEDStatus();

void setup() 
{
  Serial.begin(9600);
  pinMode(LED1pin, OUTPUT);
  pinMode(LED2pin, OUTPUT);

  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop() 
{
  // Send an HTTP GET request
  if ((millis() - lastTime) > timerDelay) {
    // Check WiFi connection status
    if(WiFi.status()== WL_CONNECTED){
      String led1Path = "http://192.168.126.174:5000/GetState/LED-1";
      String led2Path = "http://192.168.126.174:5000/GetState/LED-2";
      
      LED1status = httpGETRequest(led1Path.c_str());
      Serial.print(" led1 = ");
      Serial.println(LED1status);
      
      LED2status = httpGETRequest(led2Path.c_str());
      Serial.print(" led2 = ");
      Serial.println(LED2status);

      changeLEDStatus();
    }
    else {
      Serial.println("WiFi Disconnected");
    }
    lastTime = millis();
  }
}

String httpGETRequest(const char* serverName) {
  HTTPClient http;
    
  // Your IP address with path or Domain name with URL path 
  http.begin(serverName);
  
  // Send HTTP POST request
  int httpResponseCode = http.GET();
  
  String payload = "{}"; 
  
  if (httpResponseCode > 0) {
    Serial.print("HTTP Response code: ");
    Serial.print(httpResponseCode);
    payload = http.getString();
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  // Free resources
  http.end();

  return payload;
}

void changeLEDStatus() {
  if(LED1status == "on")
    digitalWrite(LED1pin, HIGH);
  else
    digitalWrite(LED1pin, LOW);
  
  if(LED2status == "on")
    digitalWrite(LED2pin, HIGH);
  else
    digitalWrite(LED2pin, LOW);
}