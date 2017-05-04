#include <Arduino.h>
#include <ESP8266WiFi.h>            // ESP8266 Core WiFi Library (you most likely already have this in your sketch)
#include <DNSServer.h>              // Local DNS Server used for redirecting all requests to the configuration portal
#include <ESP8266WebServer.h>       // Local WebServer used to serve the configuration portal
#include <WiFiManager.h>            // https://github.com/tzapu/WiFiManager WiFi Configuration Magic
#include <ESP8266mDNS.h>
#include <WebSocketsServer.h>
#include <Hash.h>

#include <FS.h>                     // Needed for SPIFFS

#include "Led.h"
#include "Button.h"


const char AP_NAME[] = "Garage Door Config";
const char HOSTNAME[] = "garage";

uint8_t socketNumber;

// Instantiate Status LED
Led statusLed(BUILTIN_LED);
// Instantiate Garage Button
Button garageButton(D1);
// Instantiate web server
ESP8266WebServer server(80);
WebSocketsServer webSocket(81);

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t lenght) {

    switch(type) {
        case WStype_DISCONNECTED:
            Serial.printf("[%u] Disconnected!\n", num);
            break;
        case WStype_CONNECTED:
            {
                IPAddress ip = webSocket.remoteIP(num);
                Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);

				// send message to client
				webSocket.sendTXT(num, "Connected");
                socketNumber = num;
            }
            break;
        case WStype_TEXT:
            Serial.printf("[%u] get Text: %s\n", num, payload);

            // send message to client
            // webSocket.sendTXT(num, "message here");

            // send data to all connected clients
            // webSocket.broadcastTXT("message here");
            break;
        case WStype_BIN:
            Serial.printf("[%u] get binary lenght: %u\n", num, lenght);
            hexdump(payload, lenght);

            // send message to client
            // webSocket.sendBIN(num, payload, lenght);
            break;
    }

}

void wifiManagerSetup() {
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

void webServerSetup() {
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

class Sensor {

public:
    Sensor(byte pin)
        : _pin(pin)
        , _previous_millis(0)
        , _interval_millis(500)
    {}

    void init() {
        pinMode(this->_pin, INPUT);
    }

    void debouncer() {
        if (millis() - this->_previous_millis >= this->_interval_millis) {
            bool currentState = digitalRead(this->_pin);
            if (this->_state != currentState) {
                this->_previous_millis = millis();
                this->_state = currentState;
            }
        }
    }

    // static void debouncer(Sensor* sensor) {
    //     sensor->debouncer();
    // }

    bool getState() {
        return this->_state;
    }

    void onChange(void (*callback)()) {
        // attachInterrupt(this->_pin, this->*debouncer(), CHANGE);
    }

private:

    unsigned long _previous_millis;
    uint16_t _interval_millis;
    const byte _pin;
    int _state;
    Ticker _ticker;

    void _setState(int state);

};

void blink() {
    Serial.println(digitalRead(D3));
    statusLed.blink();
}

Sensor garageSensor(D3);

// This only runs once at startup
void setup() {

    statusLed.init();
    garageButton.init();
    garageSensor.init();

    garageSensor.onChange([](){
        Serial.print("onChange: ");
        Serial.println(digitalRead(D3));
    });

    // Flash status LED to indicate booting up
    statusLed.flash(500);

    // Start Serial output
    Serial.begin(921600);

    // Start WifiManager
    wifiManagerSetup();

    if (MDNS.begin("garage", WiFi.localIP())) {
        Serial.println("MDNS responder started");
        MDNS.addService("http", "tcp", 80);
        MDNS.addService("ws", "tcp", 81);
    }
    else {
        Serial.println("MDNS.begin failed");
    }

    webSocket.begin();
    webSocket.onEvent(webSocketEvent);
    // Start the web server
    webServerSetup();





    // === Clean Up ====
    // Stop flashing status LED
    statusLed.stopFlash();

    // Set status LED to indicate boot up completed
    statusLed.setBrightness(50);
    statusLed.on();
}

void loop() {
    // webserver to handle clients
    server.handleClient();
    webSocket.loop();
    // webSocket.broadcastTXT("message here");

}
