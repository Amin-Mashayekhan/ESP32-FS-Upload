#include "analogGraph.h"
#include "../../modules/web-socket/WebSocketModule.h"
#include "../../modules/web-socket/webSocketShared.h"
#include <SPIFFS.h>  // Use SPIFFS for ESP32

// Create WebSocket object
WebSocketModule analogGraphWebSocket("/analog-graph-ws");

// Function to handle WebSocket events
void onWebSocketEventAG(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len)
{
  if (type == WS_EVT_CONNECT)
  {
    Serial.printf("Client connected to analog graph: %u\n", client->id());
  }
  else if (type == WS_EVT_DISCONNECT)
  {
    Serial.printf("Client disconnected from analog graph: %u\n", client->id());
  }
}

// Function to set up the WebSocket on the analog graph page
void setupAnalogGraphWebSocketPage(AsyncWebServer *server)
{
  analogGraphWebSocket.onEvent(onWebSocketEventAG);  // Register the event handler
  analogGraphWebSocket.addToServer(server);

  // Serve the HTML page stored in SPIFFS
  server->on("/analog/", HTTP_GET, [](AsyncWebServerRequest *request)
             {
    if (SPIFFS.exists("/analog.html")) {
      request->send(SPIFFS, "/analog.html", "text/html");
    } else {
      request->send(404, "text/plain", "File Not Found");
    } });
}

unsigned long lastSendTimeAGP = 0;
void notifyAnalogGraph()
{
  if (currentMillis - lastSendTimeAGP >= 76)
  {
    lastSendTimeAGP = currentMillis;

    // Serve the analog reading data as JSON
    int analogValue = analogRead(32);  // ESP32 ADC pin (change to any valid ADC pin)
    String jsonDataAG = "{\"analog\":" + String(analogValue) + "}";
    analogGraphWebSocket.broadcastMessage(jsonDataAG);  // Send data to all connected clients
  }
}