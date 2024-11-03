#ifndef WEBSOCKETMODULE_H
#define WEBSOCKETMODULE_H

#include <ESPAsyncWebServer.h>

class WebSocketModule {
public:
  WebSocketModule(const char* url);                   // Constructor that initializes the WebSocket with a URL
  void onEvent(AwsEventHandler handler);              // Method to register an event handler
  bool broadcastMessage(const String& message);       // Broadcasts a message to all clients and returns true if sent
  void addToServer(AsyncWebServer* server);           // Adds WebSocket to the server
  size_t clientCount();                               // Returns the number of connected clients

private:
  AsyncWebSocket ws;                                  // WebSocket object for managing connections
};

#endif