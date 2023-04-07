#ifndef SECRETS_H
#define SECRETS_H
// Pins def.

// WiFi connection
char mWifiSSID[10][20] = {"Blachowiczowie_RPT", "Blachowiczowie", "Blachowiczowie_Ext", "Blachowiczowie_RPT", "Blachowiczowie", "Blachowiczowie_Ext", "Blachowiczowie_RPT", "Blachowiczowie", "Blachowiczowie_Ext"};
char mWifiPASS[3][20] = {"najpierwuderzglowa!", "najpierwuderzglowa!", "najpierwuderzglowa!"};
// char ssid[] = "Blachowiczowie_RPT";
// char pass[] = "najpierwuderzglowa!";
//  const char* ssid = "Blachowiczowie_RPT";
//  const char* pass  = "najpierwuderzglowa!";
int wificonCounter = 0;

// Set your Static IP address

// Timezone information
const char *ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 1 * 3600;
const int daylightOffset_sec = 3600;

// Set how often to wake and read sensors
const int WakeIntervalUpdateSec = 5 * 60;
static const char *Wifi1ssid = "Blachowiczowie";
static const char *Wifi2ssid = "Blachowiczowie_RPT";
static const char *Wifi1pass = "najpierwuderzglowa!";
#endif