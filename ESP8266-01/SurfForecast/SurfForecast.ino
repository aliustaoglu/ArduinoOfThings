#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <Wire.h>
#include "SSD1306.h"
#include "ArduinoJson.h"

char ssid[] = "SPARK-PLWZV6";
char password[] = "LKYEM8VVH6";
String surfPoints[] = {"Maori-Bay", "North-Piha", "Te-Arai-Point", "Raglan-Manu-Bay"};
int dizi = 0;

WiFiClientSecure client;
SSD1306 display(0x3c, 0, 2); //0x3C OLED address, GPIO0-> tx, GPIO2-> rx

unsigned long previousMillis = 0;

void setup()
{
  Serial.begin(115200);

  Wire.pins(0, 2);
  Wire.begin(0, 2);

  display.init();
  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_10);

  // Set WiFi to station mode and disconnect from an AP if it was Previously
  // connected
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  Serial.print("Connecting Wifi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  IPAddress ip = WiFi.localIP();
  Serial.println(ip);

  //printSurfForPoint(surfPoints[0]);
  //dizi++;
}

String getSurfForecast(String surfPoint)
{
  String title = "";
  String headers = "";
  String body = "";
  bool finishedHeaders = false;
  bool currentLineIsBlank = true;
  bool gotResponse = false;
  long now;

  char host[] = "ywxb69pi2h.execute-api.us-east-1.amazonaws.com";

  if (client.connect(host, 443))
  {
    Serial.println("connected");

    String URL = "/prod/surfReport?surfpoint=" + surfPoint;

    Serial.println(URL);

    client.println("GET " + URL + " HTTP/1.1");
    client.print("Host: ");
    client.println(host);
    client.println("User-Agent: arduino/1.0");
    client.println("");

    now = millis();

    //delay(1000); // delay needed for the client to be available
    while (!client.available()) {
      delay(100); // or do other stuff
    }
    
    while (client.available())
    {
      char c = client.read();

      if (finishedHeaders)
      {
        body = body + c;
      }
      else
      {
        if (currentLineIsBlank && c == '\n')
        {
          finishedHeaders = true;
        }
        else
        {
          headers = headers + c;
        }
      }

      if (c == '\n')
      {
        currentLineIsBlank = true;
      }
      else if (c != '\r')
      {
        currentLineIsBlank = false;
      }

      gotResponse = true;
    }
    if (gotResponse)
      return body;
  }

  //return body;
}

void printSurfForPoint(String surfPoint) 
{
  StaticJsonBuffer<200> jsonBuffer;
  String surfJson = getSurfForecast(surfPoint);
  JsonObject &root = jsonBuffer.parseObject(surfJson);
  String rating = root["rating"];
  String winds = root["winds"];
  String swell = root["swell"];

  display.clear();

  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(ArialMT_Plain_16);

  display.drawString(0, 0, surfPoint);
  display.drawString(0, 15, "rating    :" + rating);
  display.drawString(0, 30, "winds   :" + winds);
  display.drawString(0, 45, "swell    :" + swell);

  display.display();
}

void loop()
{
  unsigned long currentMillis = millis();


  if (currentMillis - previousMillis >= 10000) {
    previousMillis = currentMillis;
    
    printSurfForPoint(surfPoints[dizi]);
    dizi = (dizi +1) % 4;
  }
}