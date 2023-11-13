#include "secrets.h"
#include <WiFiS3.h>
#include <SPI.h>
#include <HttpClient.h>

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
  int err = 0;
  const int networkTimeout = 30*1000;
  const int networkDelay = 1000;
  Serial.begin(115200);
  wifiConnect();
  delay(1000);

  WiFiClient client;
  HttpClient http(client);
  err = http.get(endpoint, path);

  if (err == 0)
  {
    Serial.println("startedRequest ok");

    err = http.responseStatusCode();
    if (err >= 0)
    {
      Serial.print("Got status code: ");
      Serial.println(err);

      // Usually you'd check that the response code is 200 or a
      // similar "success" code (200-299) before carrying on,
      // but we'll print out whatever response we get

      err = http.skipResponseHeaders();
      if (err >= 0)
      {
        int bodyLen = http.contentLength();
        Serial.print("Content length is: ");
        Serial.println(bodyLen);
        Serial.println();
        Serial.println("Body returned follows:");
      
        // Now we've got to the body, so we can print it out
        unsigned long timeoutStart = millis();
        char c;
        // Whilst we haven't timed out & haven't reached the end of the body
        while ( (http.connected() || http.available()) && ((millis() - timeoutStart) < networkTimeout) )
        {
          if (http.available())
          {
              c = http.read();
              // Print out this character
              Serial.print(c);

              bodyLen--;
              // We read something, reset the timeout counter
              timeoutStart = millis();
          }
          else
          {
              // We haven't got any data, so let's pause to allow some to
              // arrive
              delay(networkDelay);
          }
        }
      }
      else
      {
        Serial.print("Failed to skip response headers: ");
        Serial.println(err);
      }
    }
    else
    {    
      Serial.print("Getting response failed: ");
      Serial.println(err);
    }
  }
  else
  {
    Serial.print("Connect failed: ");
    Serial.println(err);
  }
  http.stop();

  // And just stop, now that we've tried a download
  while(1);
}

void loop() {
}