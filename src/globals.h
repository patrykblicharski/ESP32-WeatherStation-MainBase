#ifndef GLOBALS_H
#define GLOBALS_H

#define LED_READY_PIN 13
#define LED_BUSY_PIN 19

#define ETH_LED_PIN 32
#define ETH_LED2_PIN 17

#define RAIN_HALL_PIN 35
#define WIND_HALL_PIN 25

// Timer for deepsleep
#define uS_TO_S_FACTOR 1000000 /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP 600      // in use uS_TO_S_FACTOR*TIME_TO_SLEEP(in sec)

#define WDT_TIMEOUT 60 // watchdog timer

// Metric or Imperial measurements
#define METRIC

// Sensor enable
#define BH1750Enable
#define BMP085Enable
#define AHTX0Enable

#define RSSI_INVALID -9999

// Battery calibration
// batteryCalFactor = measured battery voltage/ADC reading
#define batteryCalFactor .0011804
#define batteryLowVoltage 3.3

// Global instantiation
bool lowBattery = false;
bool WiFiEnable = false;

long rssi = 0;

#endif
