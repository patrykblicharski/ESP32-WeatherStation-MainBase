#ifndef GLOBALS_H
#define GLOBALS_H

#define LUX_ADDRESS 23 
#define BMP180_ADDRESS 77
#define AHT20X_ADDRESS 38

#define LED_READY_PIN 13
#define LED_BUSY_PIN 19

#define ETH_LED_PIN 32
#define ETH_LED2_PIN 17

#define RAIN_HALL_PIN 35
#define WIND_HALL_PIN 25
#define BATTERY_PIN 33

//Timer for deepsleep
#define uS_TO_S_FACTOR 1000000  /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP  30  //in use uS_TO_S_FACTOR*TIME_TO_SLEEP(in sec)

#define WDT_TIMEOUT 60   //watchdog timer

//Metric or Imperial measurements
#define METRIC

//Sensor enable
#define BH1750Enable
#define BMP085Enable
#define AHTX0Enable

//General defines
#define RSSI_INVALID -9999

//Battery calibration
//batteryCalFactor = measured battery voltage/ADC reading
#define batteryCalFactor .0011804
#define batteryLowVoltage 3.3


struct sensorExist{
    bool bh1750;
    bool bmp085;
    bool ahtx0;
    bool uv;
}SensorExist;

struct sensorStatus
{
    int i2c;
  int uv; 
  int bmp;
  int lux;
  int ath;
  //int temperature;
} Status ;




#endif
 