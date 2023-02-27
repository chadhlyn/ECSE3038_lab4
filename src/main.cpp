#include <Arduino.h>
#include <Wifi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "env.h"

const char* host = "https://ecse-three-led-api.onrender.com/api/state";

const int LED1 = 25;
const int LED2 = 26;
const int LED3 = 27;

void setup() {
  Serial.begin(9600);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  
  Serial.println("");

  Serial.println("Loading");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  // put your main code here, to run repeatedly:
  if(WiFi.status()== WL_CONNECTED){
    HTTPClient http;
    
    String http_response;


    http.begin(host);
    http.addHeader("X-API-Key", API_KEY);

    int httpResponseCode = http.GET();

    Serial.println("");
    Serial.println("");

    if (httpResponseCode>0) {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);

        Serial.print("Response from server: ");
        http_response = http.getString();
        Serial.println(http_response);
      }
      else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
      }
      http.end();

       StaticJsonDocument<200> resp;

      DeserializationError error = deserializeJson(resp, http_response);

      if (error) {
        Serial.print("deserializeJson() failed: ");
        Serial.println(error.c_str());
        return;
      }
      
      bool switch1 = resp["light_switch_1"]; 
      bool switch2 = resp["light_switch_2"]; 
      bool switch3 = resp["light_switch_3"]; 
  
      Serial.println("");
      Serial.println(switch1);
      Serial.println(switch2);
      Serial.println(switch3);
      digitalWrite(LED1, switch1);
      digitalWrite(LED2,switch2);
      digitalWrite(LED3, switch3);
      Serial.println("LED states updated");
      
  }
  else {
    return;
  }
}