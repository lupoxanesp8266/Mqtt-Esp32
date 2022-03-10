#include "Ota_updater_esp8266.h"
#include "Arduino.h"

#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ESP8266HTTPUpdateServer.h>

const char* host = "luzmesa";
const char* update_path = "/update";
const char* update_username = "admin";
const char* update_password = "admin";

ESP8266WebServer httpServer(80);
ESP8266HTTPUpdateServer httpUpdater;

Ota_updater_esp8266::Ota_updater_esp8266(){
}

void Ota_updater_esp8266::begin(){
  MDNS.begin(host);

  httpUpdater.setup(&httpServer, update_path, update_username, update_password);
  httpServer.begin();

  MDNS.addService("http", "tcp", 80);

  Serial.printf("\nHTTPUpdateServer ready! Open http://%s.local%s in your browser and login with username '%s' and password '%s'\n", host, update_path, update_username, update_password);
}

void Ota_updater_esp8266::myhandleClient(){
  httpServer.handleClient();
  MDNS.update();
}
