#ifndef GLOBALS_H
#define GLOBALS_H
 
#define ETH_LED_PIN 32
#define ETH_LED2_PIN 17

#define RAIN_HALL_PIN 35
#define WIND_HALL_PIN 25

// Timer for deepsleep
#define uS_TO_S_FACTOR 1000000 /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP 100      // in use uS_TO_S_FACTOR*TIME_TO_SLEEP(in sec)
#define TIME_TO_SLEEP_LOW_BATTERY = 300

#define WDT_TIMEOUT 60 // watchdog timer
void print_wakeup_reason();


// Metric or Imperial measurements
#define METRIC

// Sensor enable
#define BH1750Enable
#define BMP085Enable
#define AHTX0Enable

#define RSSI_INVALID -9999

// Battery calibration
// batteryCalFactor = measured battery voltage/ADC reading //.0011804
#define batteryCalFactor .0010773
#define batteryLowVoltage 3.3

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

#endif
