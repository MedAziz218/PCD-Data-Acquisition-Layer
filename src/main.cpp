#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h> // Library for handling JSON data
const char *ssid = "TOPNET_0248";
const char *password = "pn18upt8333";

// Your Domain name with URL path or IP address with path
String serverName = "http://192.168.1.17:5000";
const char* MyHostName = "ESP32-Test";

// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastTime = 0;
// Timer set to 10 minutes (600000)
// unsigned long timerDelay = 600000;
// Set timer to 5 seconds (5000)
unsigned long timerDelay = 5000;

void setup()
{
  Serial.begin(115200);
  WiFi.setHostname(MyHostName);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
  Serial.print("ESP32 HostName: ");
  Serial.println(WiFi.getHostname());
  Serial.println("Timer set to 5 seconds (timerDelay variable), it will take 5 seconds before publishing the first reading.");
}
void sendPostRequest(String endpoint, String jsonPayload)
{
  if (WiFi.status() == WL_CONNECTED)
  {
    HTTPClient http;

    String serverPath = serverName + endpoint;

    http.begin(serverPath.c_str());
    http.addHeader("Content-Type", "application/json"); // Add Content-Type header
    http.addHeader("Content-Length", String(jsonPayload.length())); // Add Content-Length header
    // Serial.println(jsonPayload);
    
    // Send HTTP POST request with JSON data
    int httpResponseCode = http.POST(jsonPayload);

    if (httpResponseCode > 0)
    {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
      String payload = http.getString();
      Serial.println(payload);
    }
    else
    {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }
    http.end();
  }
  else
  {
    Serial.println("WiFi Disconnected");
  }
}

void loop()
{
  if ((millis() - lastTime) > timerDelay)
  {
    // Create JSON payload
    JsonDocument  jsonDoc; // Adjust size according to your JSON data 
    // jsonDoc["key1"] = "value1";
    // jsonDoc["key2"] = "value2";
    jsonDoc["url"] = "192.168.1.25";
    jsonDoc["payload"] = "pp";


    String jsonString;
    serializeJson(jsonDoc, jsonString);

    // Send JSON payload to "/receive" endpoint
    sendPostRequest("/receive", jsonString);

    lastTime = millis();
  }
}