#ifndef SENSORS_H
#define SENSORS_H
#define BH1750Enable
#include <Arduino.h>
#include <BH1750.h>
#include <Adafruit_BMP085.h>
#include <Adafruit_AHTX0.h>
#define LUX_ADDRESS 35
#define BMP180_ADDRESS 119
#define AHT20X_ADDRESS 56
#define BATTERY_PIN 33
#define batteryCalFactor .0011804
#define batteryLowVoltage 3.3

struct sensorExist
{
  bool wire;
  bool bmp;
  bool lux;
  bool ath;
};
struct sensorStatus
{
  int wire;
  int uv;
  int bmp;
  int lux;
  int ath;
};
struct sensorData
{
  float temperatureC;
  char temphum[30];
  // float windSpeed;
  // float windDirection;
  // char windCardinalDirection[5];
  float HeatIndex;
  float barometricPressure;
  float barometricSeaLevelPressure;
  float BMEtemperature;
  float humidity;
  // float UVIndex;
  float lux;
  float batteryVoltage;
  int batteryADC;
  unsigned int coreC;
};


bool checkI2cTransmission(int address, bool *setStatus);
void sensorCheck(void);
void readSensors(struct sensorData *environment);

void testsensor(void);

float convertFtoC(float f);
float convertCtoF(float c);
float computeHeatIndex(float temperature, float percentHumidity,
                       bool isFahrenheit);
void checkBatteryVoltage(struct sensorData *environment);
#endif