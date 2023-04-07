#ifndef SENSORS_CONF_H
#define SENSORS_CONF_H

#define ETH_LED_PIN 32
#define ETH_LED2_PIN 17

#define RAIN_HALL_PIN 35
#define WIND_HALL_PIN 25

#define BH1750_ADDRESS 35
#define BMP180_ADDRESS 119
#define AHT20X_ADDRESS 56
#define BATTERY_PIN 33

// #define uS_TO_S_FACTOR 1000000 /* Conversion factor for micro seconds to seconds */
// #define TIME_TO_SLEEP_LOW_BATTERY 300

// Sensor enable
#define BH1750Enable 1
#define BMP180Enable 1
#define ALTITUDE 202.0

#define AHTX0Enable 1
// Battery calibration
// batteryCalFactor = measured battery voltage/ADC reading //.0011804
#define batteryCalFactor .0010773
#define batteryLowVoltage 3.3

#endif