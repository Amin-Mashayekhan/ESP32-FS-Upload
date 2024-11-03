#ifndef WEBSOCKETTEST_H
#define WEBSOCKETTEST_H

#include <ESPAsyncWebServer.h>  // Handles the server and WebSocket functionality

void setupWebSocketTestPage(AsyncWebServer *server);  // Initializes WebSocket on the test page
void notifyClients();  // Sends real-time data over the WebSocket

#endif