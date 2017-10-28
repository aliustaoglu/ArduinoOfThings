#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>

char ssid[] = "SPARK-PLWZV6";
char password[] = "LKYEM8VVH6";

WiFiClientSecure client;

void setup() {

  Serial.begin(115200);

  // Set WiFi to station mode and disconnect from an AP if it was Previously
  // connected
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  Serial.print("Connecting Wifi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  IPAddress ip = WiFi.localIP();
  Serial.println(ip);
}

String getSurfForecast(String surfPoint) {
  String title = "";
  String headers = "";
  String body = "";
  bool finishedHeaders = false;
  bool currentLineIsBlank = true;
  bool gotResponse = false;
  long now;

  char host[] = "ywxb69pi2h.execute-api.us-east-1.amazonaws.com";

  if (client.connect(host, 443)) {
    Serial.println("connected");
    
    String URL = "/prod/surfReport?surfpoint=" + surfPoint;

    Serial.println(URL);
    
    client.println("GET " + URL + " HTTP/1.1");
    client.print("Host: "); client.println(host);
    client.println("User-Agent: arduino/1.0");
    client.println("");

    now = millis();
    
    delay(2000); // delay needed for the client to be available
    while (client.available()) {
      char c = client.read();

      if (finishedHeaders) {
        body=body+c;
      } else {
        if (currentLineIsBlank && c == '\n') {
          finishedHeaders = true;
        }
        else {
          headers = headers + c;
        }
      }

      if (c == '\n') {
        currentLineIsBlank = true;
      }else if (c != '\r') {
        currentLineIsBlank = false;
      }

      gotResponse = true;

    }
    
  }

  return body;
}

void loop() {
  Serial.println("---------");
  Serial.println("Forecast");
  String body = getSurfForecast("Maori-Bay");
  Serial.println(body);

  delay(30000);
}