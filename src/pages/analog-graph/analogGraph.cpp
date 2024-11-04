#include "analogGraph.h"
#include "../../modules/web-socket/WebSocketModule.h"
#include "../../modules/web-socket/webSocketShared.h"
#include <LittleFS.h>

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
  analogGraphWebSocket.onEvent(onWebSocketEventAG); // Register the event handler
  analogGraphWebSocket.addToServer(server);

  // Serve the HTML page stored in LittleFS
  server->on("/analog/", HTTP_GET, [](AsyncWebServerRequest *request)
             {
    if (LittleFS.exists("/analog.html")) {  // Check for the file in LittleFS
      request->send(LittleFS, "/analog.html", "text/html");
    } else {
      request->send(404, "text/plain", "File Not Found");
    } });
}

unsigned long lastSendTimeAGP = 0;
unsigned long sendIntervalAGP = 100; // Initial interval in milliseconds

void notifyAnalogGraph()
{
  // Check if enough time has passed since the last message was sent
  if (currentMillis - lastSendTimeAGP >= sendIntervalAGP)
  {
    lastSendTimeAGP = currentMillis;

    int analogValue = analogRead(32); // Read analog input
    String jsonDataAG = "{\"analog\":" + String(analogValue) + "}";

    // Attempt to send message and check if it's successful
    analogGraphWebSocket.broadcastMessage(jsonDataAG);
  }
}

