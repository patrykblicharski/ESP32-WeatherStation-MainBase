#include <Arduino.h>
#include <SPI.h>

#include <BH1750.h>
#include <Adafruit_BMP085.h>
#include <Adafruit_AHTX0.h>

#include "network.h"      //wifi control
#include "mqttServices.h" //mqtt functions

#include "secrets.h" //wifi passwd
#include "globals.h" //Global variables, defines

//################################
//        MQTT
//#################################
WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

void setup()
{

  Serial.begin(115200);
  bool wifiStatus = setup_wifi(ssid, pass);
  
  MQTTPublish("temat", "test", true,mqttClient);
}

void loop()
{
}