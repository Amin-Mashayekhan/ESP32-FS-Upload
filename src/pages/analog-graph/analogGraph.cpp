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
unsigned long sendIntervalAGP = 3; // Initial interval in milliseconds
// Array for storing analog readings
const int BUFFER_SIZE = 80;
int analogValues[BUFFER_SIZE];
int analogIndex = 0;

void notifyAnalogGraph()
{
  // Collect analog data every 1 ms
  if (currentMillis - lastSendTimeAGP >= sendIntervalAGP)
  {
    lastSendTimeAGP = currentMillis;
    analogValues[analogIndex++] = analogRead(32); // Read analog from pin 32
    if (analogIndex >= BUFFER_SIZE)
    {
      analogIndex = 0; // Reset index after filling buffer

      // Convert the buffer to a JSON array
      String jsonData = "[";
      for (int i = 0; i < BUFFER_SIZE; i++)
      {
        jsonData += analogValues[i];
        if (i < BUFFER_SIZE - 1)
          jsonData += ",";
      }
      jsonData += "]";

      // Broadcast the JSON data
      if (analogGraphWebSocket.clientCount() > 0)
      {
        analogGraphWebSocket.broadcastMessage(jsonData);
      }
    }
  }
}
