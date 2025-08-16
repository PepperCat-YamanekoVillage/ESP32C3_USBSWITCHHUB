#include <WiFi.h>
#include <ESPAsyncWebServer.h>

const char *ssid = "";
const char *password = "";
const IPAddress ip(192, 168, 1, 200);
const IPAddress gateway(192, 168, 1, 1);
const IPAddress dns1(192, 168, 1, 1);
const IPAddress subnet(255, 255, 255, 0);

#define CONTROL_PIN 4 // Low->Right / High->Left
#define ENABLE_PIN 5  // Low->enable

AsyncWebServer server(80);

void setup()
{
    Serial.begin(115200);

    pinMode(CONTROL_PIN, OUTPUT);
    pinMode(ENABLE_PIN, OUTPUT);
    digitalWrite(CONTROL_PIN, LOW);
    digitalWrite(ENABLE_PIN, LOW);

    if (!WiFi.config(ip, gateway, subnet, dns1))
    {
        Serial.println("Failed to configure!");
    }
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }
    Serial.println("Connected to WiFi");

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
              {
        if (request->hasParam("enable")) {
            String value = request->getParam("enable")->value();
            if (value == "L") {
                digitalWrite(CONTROL_PIN, HIGH);
                digitalWrite(ENABLE_PIN, LOW);
                Serial.println("GPIO9 set to HIGH");
                Serial.println("GPIO10 set to Low");
            }
            if (value == "R") {
                digitalWrite(CONTROL_PIN, LOW);
                digitalWrite(ENABLE_PIN, LOW);
                Serial.println("GPIO9 set to LOW");
                Serial.println("GPIO10 set to Low");
            }
            if (value == "X") {
                digitalWrite(ENABLE_PIN, HIGH);
                Serial.println("GPIO10 set to High");
            }
        }
        request->send(200, "text/plain", "OK"); });

    server.begin();
}

void loop()
{
}
