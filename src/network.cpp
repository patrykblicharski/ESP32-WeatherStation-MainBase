#include <Arduino.h>
#include "network.h"

IPAddress local_IP(192, 168, 1, 57);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);

static const char *NETWORK_SCAN = "NETWORK_SCAN";
static const char *NETWORK_SETUP = "NETWORK_SCAN";
// static const char *s = "SENSOR_DATA";
// static const char *SENSOR_BATTERY_CTL = "BATTERY_CTL";

void scan_wifi()
{
  Serial.print("\n#####################\n");
  Serial.print("##### WIFI SCAN #####\n");
  Serial.println("#####################\n");

  ESP_LOGI(NETWORK_SCAN, "Wifi scan start");

  int n = WiFi.scanNetworks(); // number of networks found
  ESP_LOGI(NETWORK_SCAN, "Wifi scan end");

  if (n == 0)
  {
    ESP_LOGW(NETWORK_SCAN, "! No networks found! ");
  }
  else
  {
    ESP_LOGI(NETWORK_SCAN, "%i networks found! ...", n);

    for (int i = 0; i < n; ++i)
    {
      char encryption = (WiFi.encryptionType(i) == WIFI_AUTH_OPEN) ? 'N' : '*';
      String SSID = WiFi.SSID(i);
      int32_t RSSI = WiFi.RSSI(i);

      ESP_LOGI(NETWORK_SCAN, "%i -> %s (%i) Encryption: %c", n + 1, SSID, RSSI, encryption);
      /*
            Serial.print(": ");
            Serial.print();
            Serial.print(" (");
            Serial.print(WiFi.RSSI(i));
            Serial.print(")");
            */
      delay(10);
    }
  }
  Serial.println("");
}

bool setup_wifi(char *siec, char *password)
{
  /*Serial.print("\n#####################\n");
  Serial.print("  ### NETWORK SETUP ###\n");
  Serial.println("#####################\n");

  delay(10);
  ESP_LOGI(NETWORK_SETUP,"Trying to connect to %c",siec);

  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("* Connecting to \n ");
  Serial.print(siec);
  if (!WiFi.config(local_IP, gateway, subnet))
  {
    Serial.println("Static IP =  Failed to configure");
  }

  WiFi.mode(WIFI_STA);
  WiFi.begin(siec, password);
  int wificonCounter = 0;
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    wificonCounter++;
    Serial.print(".");
    if (wificonCounter > 15)
      esp_restart();
  }
  Serial.println("");

  Serial.println("** WiFi connected");

  Serial.printf("[WIFI] SSID: %s", WiFi.SSID().c_str());
  Serial.printf("[WIFI] IP: %s", WiFi.localIP().toString().c_str());
  Serial.printf("[WIFI] MAC: %s", WiFi.BSSIDstr().c_str());
  Serial.printf("[WIFI] Channel: %d", WiFi.channel());

  return true;*/
}

bool setup_multi_wifi(char (*ssid)[20], char (*passwd)[20])
{
  // std::vector<std::string> aa{"Blachowiczowie_RPT","najpierwuderzglowa!"};

  // wifiMulti.addAP();
  // return true;
}
