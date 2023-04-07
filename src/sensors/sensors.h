#ifndef SENSORS_H
#define SENSORS_H

#include <Arduino.h>
#include <BH1750.h>
#include <Adafruit_BMP085.h>
#include <Adafruit_AHTX0.h>
#include "configs/sensors_conf.h"


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
  bool lowBattery;
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