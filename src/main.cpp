#include <Arduino.h>
#include <SPI.h>
// #include "ArduinoOTA.h"
#include <ElegantOTA.h>
#include <WiFiClient.h>
#include <WebServer.h>

#include "network.h"      //wifi control
#include "mqttServices.h" //mqtt functions
#include "leds.h"
#include "sensors.h"
sensorData env;

#include "esp_log.h"
#include "secrets.h" //wifi passwd
#include "globals.h" //Global variables, defines
WiFiMulti wifiMulti;

static const char *Network_Wifi_Conn = "NETWORK_STATUS_CONN";
static const char *uServer_Handlers = "NETWORK_uSERVER_HANDLER";
static const char *I2c_Scanner = "I2C_SCANNER";

// ################################
//         MQTT
// #################################
WiFiClient wifiClient;
int timer = 0;
int HttpLogServerPort = 80;

WebServer server(HttpLogServerPort); // for ota

PubSubClient mqttClient(wifiClient);
byte bErrorBMP, bErrorLux, bErrorAHT;
byte bErrorAll = 0;

void setup()
{
  // Wire.begin();

  esp_log_level_set("*", ESP_LOG_DEBUG);
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
  Wire.begin();
  Serial.begin(115200);
  byte error, address;
  int nDevices;

  Serial.print("\n#################\n");
  Serial.print("  ## I2C SCANNER ##\n");
  Serial.println("#################\n");

  ESP_LOGI(I2c_Scanner, "Scanning...");

  nDevices = 0;
  for (address = 1; address < 127; address++)
  {

    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0)
    {
      ESP_LOGI(I2c_Scanner, "Found device address %i", int(address));
      nDevices++;
    }
    else if (error == 4)
    {
      Serial.print("Unknow error at address 0x");
      if (address < 16)
        Serial.print("0");
      Serial.print(address, HEX);
      Serial.print("  ->");
      Serial.println(address);
    }
  }
  if (nDevices == 0)
    Serial.println("No I2C devices found\n");
  else
    Serial.println("done\n");

  // ESP_LOGE("I2C device list scan", "Nie wszystkie czujniki sa gotowe, go next part...");

  // scan_wifi();
  // bool wifiStatus =setup_multi_wifi(mWifiPASS,mWifiPASS);

  Serial.print("\n#####################\n");
  Serial.print("  ## MULTIWIFI SETUP ##\n");
  Serial.println("#####################\n");

  wifiMulti.addAP("x", "x");
  wifiMulti.addAP("x", "x");
  wifiMulti.addAP("x", "x");

  Serial.print("\n####################\n");
  Serial.print("## MULTIWIFI SCAN ##\n");
  Serial.println("####################\n");
  if (wifiMulti.run() == WL_CONNECTED)
  {
    Serial.println("");
    ESP_LOGI(Network_Wifi_Conn, "WiFi connected");

    ESP_LOGI(Network_Wifi_Conn, "IP: %s", WiFi.localIP().toString().c_str());
    ESP_LOGI(Network_Wifi_Conn, "MAC: %s", WiFi.BSSIDstr().c_str());
    ESP_LOGI(Network_Wifi_Conn, "Channel: %d \n", WiFi.channel());

    // bool wifiStatus = setup_wifi(ssid, pass);

    // MQTTPublish("temat", "test", true,mqttClient);
    // //sensorCheck();
    // Wire.begin();
    // delay(200);
    Serial.print("\n####################\n");
    Serial.print("# uSERVER HANDLERS #\n");
    Serial.println("####################\n");
    server.on("/", []()
              { server.send(200, "text/plain", "Hi! I am ESP32. Updated by Blacha Again and again"); });
    // NETWORK_uSERVER_HANDLER

    ElegantOTA.begin(&server); // Start ElegantOTA
    ESP_LOGI(uServer_Handlers, "ElegantOTA HTTP server started");
    server.begin();
    ESP_LOGI(uServer_Handlers, "OutputLog HTTP server begin on port: %i", HttpLogServerPort);

    sensorCheck();
    // testsensor();
  }
}

void loop()
{
  server.handleClient();     // Uchwyt serwera HTTP do obslugi klienta
  readSensors(&env);         // Odczyt sensorów
  checkBatteryVoltage(&env); // Odczyt napięcia

  if (MQTTconnect(mqttClient) == true) // Wysłanie danych przez protokół MQTT
  {
    float tempC = env.temperatureC;
    MQTTPublish("TempC", env.temperatureC, false, mqttClient);
    MQTTPublish("Hum", env.humidity, false, mqttClient);
    MQTTPublish("TempHum", env.temphum, false, mqttClient);
    MQTTPublish("HeatIndex", env.HeatIndex, false, mqttClient);
    MQTTPublish("Preasure", env.barometricPressure, false, mqttClient);
    MQTTPublish("BME", env.BMEtemperature, false, mqttClient);
    MQTTPublish("Lux", env.lux, false, mqttClient);
    MQTTPublish("bADC", env.batteryADC, false, mqttClient);
    MQTTPublish("bVOLT", env.batteryVoltage, false, mqttClient);

    Serial.printf(" %i ", timer);
  }
  delay(5000);

  esp_deep_sleep_start(); // Uśpienie układu
}