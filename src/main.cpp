#include <Arduino.h>
#include <SPI.h>
// #include "ArduinoOTA.h"
#include <ElegantOTA.h>
#include <WiFiClient.h>
#include <WebServer.h>

#include "network.h"      //wifi control
#include "mqttServices.h" //mqtt functions
#include "leds_watchdog_helper.h"
#include "sensors.h"
sensorData env;

#include "esp_log.h"
#include "secrets.h" //wifi passwd
#include "globals.h" //Global variables, defines
WiFiMulti wifiMulti;

#include <soc/soc.h>
#include <soc/rtc_cntl_reg.h>
#include <esp_task_wdt.h>
#include <esp_system.h>
#include <driver/rtc_io.h>
#include <esp_sleep.h>

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
 digitalWrite(19,HIGH);
  byte error, address;
  int nDevices;

  Serial.begin(115200);
  Serial.setDebugOutput(true); // Set debug output to Serial

  led_watchdog_progress_blink(exec_stage::WATCHDOG);

  esp_log_level_set("*", ESP_LOG_DEBUG);

  Wire.begin();

  Serial.print("\n#################\n");
  Serial.print("  ## WATCHDOG ##\n");
  Serial.println("#################\n");
  //
  //
  // LedBlink2(LED_BUSY_PIN,3,0,500);
  //
  //
  //

  print_wakeup_reason();

  // Wake up timer set
  if (esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR) == ESP_OK)
    ESP_LOGI(Deep_Sleep, "Deep sleep timer set to %i sec", TIME_TO_SLEEP);
  else
    ESP_LOGE(Deep_Sleep, "Deep sleep set time ERROR");

  // INIT Watchdog
  if (esp_task_wdt_init(WDT_TIMEOUT, true) == ESP_OK)
  {
    esp_task_wdt_add(NULL); // add current thread to WDT watch
    ESP_LOGI(Esp_parms, "ESP32 watchdog set restart after %i sec on panic", TIME_TO_SLEEP);
  }
  else
    ESP_LOGE(Esp_parms, "ESP32 watchdog set panic ERROR");

  // Set CPU frequency
  if (setCpuFrequencyMhz(80))
    ESP_LOGI(Esp_parms, "CPU set to:80Mhz");
  else
    ESP_LOGE(Esp_parms, "CPU not set to 80Mhz back to default 240Mhz");

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

  led_watchdog_progress_blink(exec_stage::I2CSCAN);

  // ESP_LOGE("I2C device list scan", "Nie wszystkie czujniki sa gotowe, go next part...");

  // scan_wifi();
  // bool wifiStatus =setup_multi_wifi(mWifiPASS,mWifiPASS);
  //
  //
  // LedBlink2(LED_READY_PIN,5,0,300);
  //
  //
  //
  Serial.print("\n#####################\n");
  Serial.print("  ## MULTIWIFI SETUP ##\n");
  Serial.println("#####################\n");
  led_watchdog_progress_blink(exec_stage::WIFI_TRY);
  wifiMulti.addAP(Wifi1ssid, Wifi1pass); // temporary
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
    led_watchdog_progress_blink(exec_stage::WIFI_CONNECTED);
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
    //
    //
    // LedBlink2(LED_BUSY_PIN,6,1,200);
    //
    //
    //
    sensorCheck(); // to return
    led_watchdog_progress_blink(exec_stage::I2COK);
  }
}

void loop()
{
  server.handleClient();     // Uchwyt serwera HTTP do obslugi klienta
  readSensors(&env);         // Odczyt sensorów
  checkBatteryVoltage(&env); // Odczyt napięcia

  if (MQTTconnect(mqttClient) == true) // Wysłanie danych przez protokół MQTT
  {
    //
    //
    // LedBlink2(LED_BUSY_PIN,3,0,500);
    //
    //
    //
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
    led_watchdog_progress_blink(exec_stage::SENT);
    // Serial.printf(" %i ", timer);
  }
  delay(1000);

  ESP_LOGI(Deep_Sleep, "Deep sleep start in 5 sec");
  for (int i = 5; i >= 0; i--)
  {
    led_watchdog_progress_blink(exec_stage::DEEP);
    Serial.println(i);
    // LedFade(LED_BUSY_PIN,"LED_BUSY_PIN");
    // delay(1000);
    // LedFade(LED_READY_PIN,"LED_READY_PIN");
  }

  ESP_LOGI(Deep_Sleep, "Starting Deep sleep!");
  //
  //
  // LedBlink2(LED_READY_PIN,11,0,200);
  //
  //
  //
  esp_deep_sleep_start(); // Uśpienie układu
}

void print_wakeup_reason()
{
  esp_sleep_wakeup_cause_t wakeup_reason = esp_sleep_get_wakeup_cause();

  switch (wakeup_reason)
  {
  case ESP_SLEEP_WAKEUP_EXT0:
    ESP_LOGI(Esp_get_parms, "Wakeup caused by external signal using RTC_IO");
    break;
  case ESP_SLEEP_WAKEUP_EXT1:
    ESP_LOGI(Esp_get_parms, "Wakeup caused by external signal using RTC_CNTL");
    break;
  case ESP_SLEEP_WAKEUP_TIMER:
    ESP_LOGI(Esp_get_parms, "Wakeup caused by Deepsleep timer");
    break;
  case ESP_SLEEP_WAKEUP_TOUCHPAD:
    ESP_LOGI(Esp_get_parms, "Wakeup caused by touchpad");
    break;
  case ESP_SLEEP_WAKEUP_ULP:
    ESP_LOGI(Esp_get_parms, "Wakeup caused by ULP program");
    break;
  default:
    ESP_LOGI(Esp_get_parms, "Wakeup was not caused by deep sleep");
    break;
  }
}