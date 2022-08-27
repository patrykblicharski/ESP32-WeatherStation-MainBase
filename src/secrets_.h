#ifndef SECRETS_H
#define SECRETS_H
// Pins def.

// WiFi connection
char ssid[] = " ";
char pass[] = "s";
 
int wificonCounter = 0;

// Timezone information
const char *ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 1 * 3600;
const int daylightOffset_sec = 3600;

// Set how often to wake and read sensors
const int WakeIntervalUpdateSec = 5 * 60;

#endif