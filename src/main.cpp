#include <Arduino.h>
#include <WiFi.h>
#include <ESPmDNS.h>
#include <WebServer.h>

const char* ssid = "IoT518";
const char* password = "iot123456";

WebServer server(80);

void handleRoot() {
    String message = (server.method() == HTTP_GET)?"GET":"POST";
    message += " " + server.uri() + "\n";
    for (uint8_t i=0; i < server.args(); i++){
        message += " " + server.argName(i) + " : " + server.arg(i) + "\n";
    }
    message += "\nHello from ESP32!\n";
    server.send(200, "text/plain", message);
}

void handleNotFound() {
    String message = "File Not Found\n\n";
    server.send(404, "text/plain", message);
}

void setup(void) {
    Serial.begin(115200);
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    Serial.println("");

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.printf("\nConnected to %s, and Server IP : %s\n",ssid, WiFi.localIP().toString().c_str());
    MDNS.begin("miniWeb");

    server.on("/", handleRoot);

    // This is an example of inline function for a handler
    server.on("/inline", [](){
        server.send(200, "text/plain", "Hello from the inline function\n");
    });
 
    server.onNotFound(handleNotFound);

    server.begin();
    Serial.println("HTTP server started");
}

void loop(void) {
    server.handleClient();
}