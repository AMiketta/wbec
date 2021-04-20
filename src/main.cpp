// Copyright (c) 2021 steff393

#include <Arduino.h>
#include <ArduinoOTA.h>
#include "globalConfig.h"
#include "LittleFS.h"
#include "mbComm.h"
#include "SPIFFSEditor.h"
#include "wlan_key.h"
#define WIFI_MANAGER_USE_ASYNC_WEB_SERVER
#include <WiFiManager.h>
#include "webServer.h"

bool _handlingOTA = false;


void setup() {
  Serial.begin(115200);

  if(!LittleFS.begin()){ 
    Serial.println("An Error has occurred while mounting LittleFS");
    return;
  }

  WiFiManager wifiManager;
  wifiManager.autoConnect("wbec", "cebw1234");

  // setup the Webserver
  initWebserver();
  server.begin();

  // setup the OTA server
  ArduinoOTA.setHostname(otaHost);
  ArduinoOTA.begin();

  ArduinoOTA.onStart([]() 
  {
    _handlingOTA = true;
  });

  mb_setup();
  Serial.print("Boot time: ");Serial.println(millis());
}


void loop() {
  ArduinoOTA.handle();
  if(!_handlingOTA) {
    mb_handle();
  }
}