#include <ESP8266WiFi.h>
#include "FirebaseArduino.h" // 파이어베이스 라이브러리

#define FIREBASE_HOST "파이어베이스아이디"
#define FIREBASE_AUTH "비밀번호"
#define WIFI_SSID " "
#define WIFI_PASSWORD " "

#include "DHT.h"
#define DHTPIN 5     // what digital pin we're connected to
#define DHTTYPE DHT11  // DHT 22  (AM2302), AM2321
DHT dht(DHTPIN, DHTTYPE);

void displayTime() {
  
}

void displayAir() {

}

void setup() {
  Serial.begin(115200);

  // connect to wifi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);

  Serial.println("DHT11 test!");
  dht.begin();
}

int n = 0;

void loop() {
  delay(2000);
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  if (isnan(h) || isnan(t)) {
    Serial.print("Error");
    return;
  }
  Firebase.setFloat("Humidity",h);
  Firebase.setFloat("Temperature",t);

  
  n++;
  // set value
  Firebase.setFloat("number", n);
  // handle error
  if (Firebase.failed()) {
      Serial.print("setting /number failed:");
      Serial.println(Firebase.error());  
      return;
  }
  delay(1000); 
}
