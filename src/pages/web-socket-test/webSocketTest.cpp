#include "webSocketTest.h"
#include "AsyncWebSocket.h"
#include <ESPAsyncWebServer.h>
#include "../../modules/web-socket/WebSocketModule.h"
#include "../../modules/web-socket/webSocketShared.h"
#include <SPIFFS.h>  // Use SPIFFS instead of LittleFS for ESP32

// Create WebSocket object
WebSocketModule testWebSocket("/web-socket-test-ws");

// Function to handle WebSocket events
void onWebSocketEventWST(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len)
{
  if (type == WS_EVT_CONNECT)
  {
    Serial.printf("Client connected to web socket test page: %u\n", client->id());
  }
  else if (type == WS_EVT_DISCONNECT)
  {
    Serial.printf("Client disconnected from web socket test page: %u\n", client->id());
  }

}

// Function to set up WebSocket on the test page
void setupWebSocketTestPage(AsyncWebServer *server)
{
  testWebSocket.onEvent(onWebSocketEventWST);  // Register the event handler
  testWebSocket.addToServer(server);
  
  // Serve the HTML file for the WebSocket test page
  server->on("/web-socket-test/", HTTP_GET, [](AsyncWebServerRequest *request)
             { request->send(SPIFFS, "/web-socket-test.html", "text/html"); });
}

// Variables for managing the sending interval and success/fail counts
unsigned long lastSendTimeWST = 0;
unsigned long sendInterval = 1000;
unsigned long successfulSends = 0;
unsigned int failedSends = 0;

void notifyClients()
{
  if (millis() - lastSendTimeWST >= sendInterval)
  {
    lastSendTimeWST = millis();

    // Create a JSON string to send over WebSocket
    String jsonDataWST = "{\"sendInterval\": \"" + String(sendInterval) + "\",";
    jsonDataWST += "\"freeHeap\": \"" + String(ESP.getFreeHeap()) + "\",";
    jsonDataWST += "\"successfulSends\": \"" + String(successfulSends) + "\",";
    jsonDataWST += "\"failedSends\": \"" + String(failedSends) + "\",";
    jsonDataWST += "\"preparationMoment\": \"" + String(currentMillis) + "\",";
    jsonDataWST += "\"serverStartSecond\": \"" + String(serverStartSecond) + "\",";
    jsonDataWST += "\"fastCounter\": \"" + String(fastCounter) + "\"}";
    
    bool sent = testWebSocket.broadcastMessage(jsonDataWST);  // Send data to all connected clients
    
    if (testWebSocket.clientCount() > 0)  // Check if clients are connected
    {
      if (sent)
      {
        successfulSends++;
        if (sendInterval > 100)
        {
          sendInterval -= 50;
        }
        else if (sendInterval > 34)
        {
          sendInterval -= 1;
        }
      }
      else
      {
        failedSends++;
        sendInterval += 100;
      }
    }
  }
}