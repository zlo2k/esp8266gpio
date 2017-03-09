#include <ESP8266WiFi.h>
#include <WiFiManager.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPUpdateServer.h>
#include <FS.h>

#define serial Serial

// ---  CONFIG --- //

int gpio[] = {0, 2}; // GPIO list to control
int gpiostat[] = {1, 0}; // Default status of GPIO on start - 0 disabled, 1 - enabled
int gpio_count = 2; // total count of GPIO used

// ---  END CONFIG --- //

ESP8266WebServer server(80);
ESP8266HTTPUpdateServer httpUpdater;

#include "server.h"

void setup(void) {
  serial.begin(115200);
  serial.print("\n");
  serial.setDebugOutput(true);
  SPIFFS.begin();
  for (byte i = 0; i < gpio_count; i++) {
    pinMode(gpio[i], OUTPUT);
    digitalWrite(gpio[i], gpiostat[i]);
  }

  //WIFI INIT
  WiFiManager wifiManager;
  wifiManager.autoConnect("espGpio"); // Create open AP with this SSID
  //wifiManager.autoConnect("AP_SSID", "AP_PASSWORD");
  serial.print(WiFi.localIP());

  server.on("/gpio", HTTP_GET, []() {
    int gp = -1;
    if (server.hasArg("gpio")) {
      int tmp = server.arg("gpio").toInt();
      for (int i = 0; i < gpio_count; i++) {
        if (gpio[i] == tmp) {
          gp = i;
          break;
        }
      }
      if (gp > -1) {
        if (server.hasArg("status")) {
          String st = server.arg("status");
          gpiostat[gp] = (st == "on" ? 1 : 0);
        } else {
          gpiostat[gp] = (gpiostat[gp] == 0 ? 1 : 0);
        }
        digitalWrite(gpio[gp], gpiostat[gp]);
      }
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
      json += " \"" + String(gpio[i]) + "\" :" + String(gpiostat[i]) + (i == gpio_count - 1 ? "" : ", ");
    }
    json += "}";
    server.send(200, "text/json", json);
    json = String();
  });
  httpUpdater.setup(&server);
  server.begin();
  serial.println("HTTP server started");

}

void loop(void) {
  server.handleClient();
}
