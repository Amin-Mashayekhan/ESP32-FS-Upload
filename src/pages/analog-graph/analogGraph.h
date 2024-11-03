#ifndef ANALOGGRAPH_H
#define ANALOGGRAPH_H

#include <ESPAsyncWebServer.h>
#include <LittleFS.h>

// Function declaration for handling the analog reading page
void setupAnalogGraphWebSocketPage(AsyncWebServer *server);
void notifyAnalogGraph();

#endif