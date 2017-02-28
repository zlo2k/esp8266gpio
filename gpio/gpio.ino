#include <ESP8266WiFi.h>
#include <WiFiManager.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPUpdateServer.h>
#include <FS.h>

#define DBG_OUTPUT_PORT Serial

// ---  CONFIG --- //

int gpio[] = {0, 2}; // GPIO list to control
int gpiostat[] = {1, 0}; // Default status of GPIO on start - 0 disabled, 1 - enabled
int gpio_count = 2; // total count of GPIO used

// ---  END CONFIG --- //

ESP8266WebServer server(80);
ESP8266HTTPUpdateServer httpUpdater;

#include "server.h"

void setup(void) {
  DBG_OUTPUT_PORT.begin(115200);
  DBG_OUTPUT_PORT.print("\n");
  DBG_OUTPUT_PORT.setDebugOutput(true);
  SPIFFS.begin();
  for (byte i = 0; i < gpio_count; i++) {
    pinMode(gpio[i], OUTPUT);
    digitalWrite(gpio[i], gpiostat[i]);
  }
  
  //WIFI INIT
  WiFiManager wifiManager;
  wifiManager.autoConnect("espGpio"); // Create open AP with this SSID
  //wifiManager.autoConnect("AP_SSID", "AP_PASSWORD");
  DBG_OUTPUT_PORT.print(WiFi.localIP());

  server.on("/gpio", HTTP_GET, []() {
    if (server.hasArg("gpio") && server.hasArg("status")) {
      int gp = server.arg("gpio").toInt();
      String st = server.arg("status");
      gpiostat[gp] = (st == "on" ? 1 : 0);
      digitalWrite(gp, gpiostat[gp]);
    }
    server.send(200, "text/plain", "");
  });
  server.onNotFound([]() {
    if (!handleFileRead(server.uri()))
      server.send(404, "text/plain", "FileNotFound");
  });

  server.on("/status", HTTP_GET, []() {
    String json = "{";
    for (byte i = 0; i < gpio_count; i++) {
      json += " \""+String(gpio[i]) + "\" :" + String(gpiostat[i]) + (i == gpio_count - 1 ? "" : ", ");
    }
    json += "}";
    server.send(200, "text/json", json);
    json = String();
  });
  httpUpdater.setup(&server);
  server.begin();
  DBG_OUTPUT_PORT.println("HTTP server started");

}

void loop(void) {
  server.handleClient();
}
