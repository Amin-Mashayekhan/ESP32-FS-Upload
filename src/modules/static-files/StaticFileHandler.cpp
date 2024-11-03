#include "StaticFileHandler.h"
#include <SPIFFS.h>  // Use SPIFFS for ESP32

// Global variables or constants for placeholders
extern String title;
extern String dynamicContent;

void setupStaticFileRoutes(AsyncWebServer* server) {
    // Serve index.html with dynamic content
    server->on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        String html = SPIFFS.open("/index.html", "r").readString();
        html.replace("{{title}}", title);
        html.replace("{{dynamicContent}}", dynamicContent);
        request->send(200, "text/html", html);
    });

    // Serve CSS and JS files
    server->on("/assets/bootstrap.rtl.min.css", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/assets/bootstrap.rtl.min.css", "text/css");
    });

    server->on("/assets/style.css", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/assets/style.css", "text/css");
    });

    server->on("/assets/bootstrap.bundle.min.js", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/assets/bootstrap.bundle.min.js", "text/javascript");
    });

    // Serve font files
    server->on("/assets/fonts/vazir-font/Vazir-Medium-FD.woff2", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/assets/fonts/vazir-font/Vazir-Medium-FD.woff2", "font/woff2");
    });

    server->on("/assets/fonts/vazir-font/Vazir-Medium-FD.woff", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/assets/fonts/vazir-font/Vazir-Medium-FD.woff", "font/woff");
    });

    server->on("/assets/fonts/vazir-font/Vazir-Medium-FD.ttf", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/assets/fonts/vazir-font/Vazir-Medium-FD.ttf", "font/ttf");
    });

    server->on("/assets/fonts/vazir-font/Vazir-Medium-FD.eot", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/assets/fonts/vazir-font/Vazir-Medium-FD.eot", "font/eot");
    });
}