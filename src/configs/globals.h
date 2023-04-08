#ifndef GLOBALS_H
#define GLOBALS_H

// Timer for deepsleep
#define uS_TO_S_FACTOR 1000000 /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP 900      // in use uS_TO_S_FACTOR*TIME_TO_SLEEP(in sec)
#define TIME_TO_SLEEP_LOW_BATTERY 1800

#define WDT_TIMEOUT 190 // watchdog timer
void print_wakeup_reason();

#define METRIC

#define RSSI_INVALID -9999

// Global instantiation
bool lowBattery = false;
bool WiFiEnable = false;

long rssi = 0;

static const char *Network_Wifi_Conn = "NETWORK_STATUS_CONN";
static const char *uServer_Handlers = "NETWORK_uSERVER_HANDLER";
static const char *I2c_Scanner = "I2C_SCANNER";
static const char *Deep_Sleep = "ESP_DEEPSLEEP";
static const char *Watch_dog = "ESP_WATCHDOG";
static const char *Esp_parms = "ESP_SET_PARMS";
static const char *Esp_get_parms = "ESP_GET_PARMS";
static const char *Service_Mode = "SERVICE_MODE";

#endif
