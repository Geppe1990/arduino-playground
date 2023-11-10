#include "secrets.h"
#include <WiFiS3.h>

void wifiConnect() {
  WiFi.begin(MY_WIFI_SSID, MY_WIFI_PASS);
  Serial.println("Mi connetto a " + (String)MY_WIFI_SSID);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnesso a " + (String)MY_WIFI_SSID);
  Serial.print("IP: ");Serial.println(WiFi.localIP());
}

void setup() {
  char endpoint[] = "jsonplaceholder.typicode.com";
  char path[] = "/posts/1";
  int port = 80;
  Serial.begin(115200);
  wifiConnect();
  delay(1000);

  WiFiClient client;
  if (!client.connect(endpoint, port)) {
    Serial.println("[Endpoint "+ (String)endpoint +" non raggiunto]");
  } else {
    Serial.println("[Endpoint "+ (String)endpoint +" raggiunto]");
    client.println("GET " + (String)path +" HTTP/1.1");
    client.println("Host: " + (String)endpoint);
    client.println("Connection: close");
    client.println("");

    unsigned long t1 = millis();
    while (client.available() == 0) {
      if ((millis() - t1) > 5000) {
         Serial.println("timeout");
         client.stop();
      }
    }
    
    //ricezione dati 
    Serial.println("Stampo la risposta:\n");
    while (client.available()) {
      String line = client.readStringUntil('\r');
      Serial.print(line);
    }
    client.stop();
  }
}

void loop() {
}