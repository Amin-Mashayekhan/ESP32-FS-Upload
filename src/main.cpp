#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <LittleFS.h>
#include "pages/analog-graph/analogGraph.h"
#include "pages/web-socket-test/webSocketTest.h"
#include "modules/web-socket/webSocketShared.h"
#include "modules/static-files/StaticFileHandler.h"

#define LED_BUILTIN 2 // Assuming GPIO 2 is the built-in LED pin

const char *ssid = "Maad";
const char *password = "Mm-12345";
int wifi_timer = 0;

AsyncWebServer server(80);

void setup()
{
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);

  if (!LittleFS.begin(true))
  {
    Serial.println("An Error has occurred while mounting LittleFS");
    return;
  }
  Serial.println("LittleFS mounted successfully");

  // Handle the analog Graph WebSocket page
  setupAnalogGraphWebSocketPage(&server);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(999.9);
    wifi_timer++;
    Serial.println("Connecting to WiFi.. \\ " + String(wifi_timer));
    digitalWrite(LED_BUILTIN, HIGH);
    delay(0.1);
    digitalWrite(LED_BUILTIN, LOW);
  }
  // WiFi successfully connected
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000);
  digitalWrite(LED_BUILTIN, LOW);
  Serial.println(WiFi.localIP());

  setupStaticFileRoutes(&server);
  setupWebSocketTestPage(&server);

  server.begin();
}

unsigned long serverStartPreviousMillis = 0;

void loop()
{
  fastCounter++;
  currentMillis = millis();
  if (currentMillis - serverStartPreviousMillis >= 1000)
  {
    serverStartPreviousMillis = currentMillis;
    serverStartSecond++;
  }
  notifyAnalogGraph();
  notifyClients();
}
