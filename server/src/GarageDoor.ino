#include <Arduino.h>
#include <ESP8266WiFi.h>            // ESP8266 Core WiFi Library (you most likely already have this in your sketch)
#include <DNSServer.h>              // Local DNS Server used for redirecting all requests to the configuration portal
#include <ESP8266WebServer.h>       // Local WebServer used to serve the configuration portal
#include <WiFiManager.h>            // https://github.com/tzapu/WiFiManager WiFi Configuration Magic
#include <ESP8266WiFi.h>
#include <FS.h>                     // Needed for SPIFFS

#include "Led.h"
#include "Button.h"

const char AP_NAME[] = "Garage Door Config";
const char HOSTNAME[] = "GarageDoorServer";

// Instantiate Status LED
Led statusLed(BUILTIN_LED);
// Instantiate Garage Button
Button garageButton(BUILTIN_LED);
// Instantiate web server
ESP8266WebServer server(80);


void wifiManagerInit() {
    // Initialize Wifi Manager
    WiFiManager wifiManager;
    // wifiManager.autoConnect("Garage Door Opener");
    wifiManager.setAPCallback(configModeCallback);
    if (!wifiManager.autoConnect(AP_NAME)) {
        Serial.println("failed to connect and hit timeout");
        //reset and try again, or maybe put it to deep sleep
        ESP.reset();
        delay(1000);
    }
    WiFi.hostname(HOSTNAME);
}

void configModeCallback(WiFiManager *myWiFiManager) {
    Serial.println("Entered my config mode");
    Serial.println(WiFi.softAPIP());

    //if you used auto generated SSID, print it
    Serial.println(myWiFiManager->getConfigPortalSSID());
    // Entered config mode, make led toggle faster
    statusLed.flash(50);
}

void webServerInit() {
    // Start SPIFFS for file access
    SPIFFS.begin();

    server.serveStatic("/", SPIFFS, "/index.html");

    server.on("/api/status", HTTP_GET, [](){
        Serial.println("API Status GET called");

        String jsonString = "{ \"success\": true, \"method\": \"GET\" }";

        Serial.println(jsonString);
        server.send(200, "application/json", jsonString);
    });

    server.on("/api/status", HTTP_POST, [](){
        Serial.println("API Status POST called");

        String jsonString = "{ \"success\": true, \"method\": \"POST\" }";

        Serial.println(jsonString);
        server.send(200, "application/json", jsonString);
    });

    server.on("/api/button", HTTP_POST, [](){
        Serial.println("Push Button API called");
        garageButton.tap();

        String jsonString = "{ \"success\": true, \"method\": \"POST\" }";
        server.send(200, "application/json", jsonString);
    });

    server.begin();
    Serial.println("Server started");
    Serial.print("Use this URL to connect: ");
    Serial.print(WiFi.localIP());
    Serial.println("/");

}


// This only runs once at startup
void setup() {

    statusLed.init();

    garageButton.init();

    // Flash status LED to indicate booting up
    statusLed.flash(500);

    // Start Serial output
    Serial.begin(921600);

    // Start WifiManager
    wifiManagerInit();

    // Start the web server
    webServerInit();

    // === Clean Up ====
    // Stop flashing status LED
    statusLed.stopFlash();

    // Set status LED to indicate boot up completed
    statusLed.on();
}

void loop() {
    // webserver to handle clients
    server.handleClient();
}
