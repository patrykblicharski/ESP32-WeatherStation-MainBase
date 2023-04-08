#include <Arduino.h>
#include <SPI.h>
#include "ArduinoOTA.h"
#include <ElegantOTA.h>
#include <WiFiClient.h>
#include <WebServer.h>

#include "network/network.h"   //wifi control
#include "mqtt/mqttServices.h" //mqtt functions
#include "leds/leds_watchdog_helper.h"
#include "sensors/sensors.h"
sensorData env;

#include "esp_log.h"
#include "configs/secrets.h" //wifi passwd
#include "configs/globals.h" //Global variables, defines
WiFiMulti wifiMulti;

#include <soc/soc.h>
#include <soc/rtc_cntl_reg.h>
#include <esp_task_wdt.h>
#include <esp_system.h>
#include <driver/rtc_io.h>
#include <esp_sleep.h>

// for maint check
const int httpPort = 1880;
WiFiClient client;
bool maintcheck = false;
void gosleep();
String maintpayload = "MAINT";

// @@@@@@@ add cpu temperature function
#ifdef __cplusplus
extern "C"
{
#endif
  uint8_t temprature_sens_read();
#ifdef __cplusplus
}
#endif
uint8_t temprature_sens_read();
// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

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

int runCount = 0;

void setup()
{
  esp_log_level_set("*", ESP_LOG_DEBUG);

  digitalWrite(19, HIGH);
  byte error, address;
  int nDevices;

  Serial.begin(115200);
  Serial.setDebugOutput(true); // Set debug output to Serial

  led_watchdog_progress_blink(exec_stage::WATCHDOG);

  Wire.begin();

  Serial.print("\n#################\n");
  Serial.print("  ## WATCHDOG ##\n");
  Serial.println("#################\n");

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

  Serial.print("\n#####################\n");
  Serial.print("  ## MULTIWIFI SETUP ##\n");
  Serial.println("#####################\n");

  wifiMulti.addAP(Wifi1ssid, Wifi1pass); // temporary
  wifiMulti.addAP(Wifi2ssid, Wifi1pass);
  // wifiMulti.addAP("x", "x");

  Serial.print("\n####################\n");
  Serial.print("## MULTIWIFI SCAN ##\n");

  Serial.println("####################\n");

  led_watchdog_progress_blink(exec_stage::WIFI_TRY);

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
              { server.send(200, "text/plain", "Hi! I am ESP32. Updated by Blacha Again and againss"); });
    // NETWORK_uSERVER_HANDLER

    ElegantOTA.begin(&server); // Start ElegantOTA
    ESP_LOGI(uServer_Handlers, "ElegantOTA HTTP server started");
    server.begin();
    ESP_LOGI(uServer_Handlers, "OutputLog HTTP server begin on port: %i", HttpLogServerPort);

    sensorCheck(); // to return
  }
  else
  {
    led_watchdog_progress_blink(exec_stage::WIFI_ERROR);
    ESP_LOGE(Network_Wifi_Conn, "!!!!!!!!!!!!\n\n\nWIFI problem\n\n\n!!!!!!!!!!!!!!!!!");
    ESP.restart();
  }
}
void loop()
{

  server.handleClient();

  // Uchwyt serwera HTTP do obslugi klienta

  if (runCount == 0)
  {
    int len;

    client.stop();
    if (client.connect(host, httpPort))
    {
      String url = "/hello.json";
      client.print(String("GET ") + url + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "User-Agent: NodeMCU\r\n" + "Connection: close\r\n\r\n");
      while (client.connected())
      {
        String line = client.readStringUntil('\n'); // HTTP HEADER
        if (line == "\r")
          break;
      }
      String payload = client.readString(); // payload
      len = payload.length();
      String maintdata = (payload.substring(len - 6, len)).substring(0, 5);

      if (maintpayload.equals(maintdata))
        maintcheck = true;
    }

    env.coreC = ((temprature_sens_read() - 32) / 1.8); // Read cpu temperature and convert to F->C
    readSensors(&env);                                 // Odczyt sensorów
    checkBatteryVoltage(&env);                         // Odczyt napięcia

    delay(1000);

    if (env.batteryVoltage <= batteryLowVoltage)
    {
      env.lowBattery = 1;
      ESP_LOGE(Battery, "BATTERY LOW: Measured:%fV !!!!!!!!!!!!!!!", env.batteryVoltage);
      {
        if (esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP_LOW_BATTERY * uS_TO_S_FACTOR) == ESP_OK)
          ESP_LOGE(Battery, "BATTERY LOW:DEEP SLEEP SET TO %fsec", TIME_TO_SLEEP_LOW_BATTERY);
      }
    }
    else
      env.lowBattery = 0;

    if (MQTTconnect(mqttClient) == true) // Wysłanie danych przez protokół MQTT
    {

      float tempC = env.temperatureC;
      MQTTPublish("TempC", env.temperatureC, false, mqttClient);
      MQTTPublish("Hum", env.humidity, false, mqttClient);
      MQTTPublish("TempHum", env.temphum, false, mqttClient);
      MQTTPublish("HeatIndex", env.HeatIndex, false, mqttClient);
      MQTTPublish("Preasure", env.PressureSeaLevel, false, mqttClient);
      MQTTPublish("RelAltitude", env.RelAltitude, false, mqttClient);
      MQTTPublish("BME", env.BMEtemperature, false, mqttClient);
      MQTTPublish("Lux", env.lux, false, mqttClient);
      MQTTPublish("BatteryADC", env.batteryADC, false, mqttClient);
      MQTTPublish("BatteryVoltage", env.batteryVoltage, false, mqttClient);
      MQTTPublish("LowBattery", env.lowBattery, false, mqttClient);
      MQTTPublish("CpuTemp", env.coreC, false, mqttClient);

      led_watchdog_progress_blink(exec_stage::SENT);
      delay(1000);
      runCount++;
    }

    if (maintcheck)
    {
      Serial.println("\n");
      ESP_LOGW(Service_Mode, "Stopped in MAINTENANCE MODE");
      ESP_LOGW(Service_Mode, "Going to restart after: %i sec.", WDT_TIMEOUT);
      ESP_LOGI(Network_Wifi_Conn, "IP: %s", WiFi.localIP().toString().c_str());
    }
    else
    {
      ESP_LOGW(Service_Mode, "Ended in NORMAL MODE, ZzZzZzZzZz....");
      gosleep();
    }
  }
  delay(1000);
}
// ##################################################################################################################
// ##################################################################################################################
void gosleep()
{
  ESP_LOGI(Deep_Sleep, "Deep sleep start in 5 sec");
  for (int i = 5; i >= 0; i--)
  {
    led_watchdog_progress_blink(exec_stage::DEEP);
    Serial.println(i);
  }
  ESP_LOGI(Deep_Sleep, "Starting Deep sleep!");
  esp_deep_sleep_start();
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