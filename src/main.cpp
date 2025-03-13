//LIBRARIES
#include <HTTPClient.h>
#include <Arduino.h>
#include <WiFi.h>
#include <ArduinoJson.h>
#include "env.h"

//PINS
#define LED_PIN 2

void setup()
{
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);
  delay(1000);
  digitalWrite(LED_PIN, LOW);


  Serial.begin(115200);
  WiFi.begin(SSID, PASS);

  while(WiFi.status() != WL_CONNECTED)
  {
    delay(200);
    digitalWrite(LED_PIN, HIGH);
    Serial.print(".");
  }

  Serial.print("WiFi connect. IP address: ");
  Serial.println(WiFi.localIP());

}

void loop()
{
  if (WiFi.status() == WL_CONNECTED)
  {
    HTTPClient http;

    http.begin(ENDPOINT);

    int responseCode = http.GET();

    if (responseCode <= 0)
    {
      Serial.print("An error occured with the code: ");
      Serial.println(responseCode);

      http.end();
      return;
    }

    Serial.print("HTTP Response Code: ");
    Serial.println(responseCode);
    
    String response = http.getString();
    Serial.println(response);
    http.end();

    JsonDocument object;

    DeserializationError error = deserializeJson(object, response);

    if(error)
    {
      Serial.println("Deserialization Failed");
      Serial.println(error.c_str());
      return;
    }
    

    bool rainfall = object["rainfall"];

    if (rainfall > 0)
    {
      digitalWrite(LED_PIN, HIGH);
    }
    else
    {
      digitalWrite(LED_PIN, LOW);
    }   
    
  }
}