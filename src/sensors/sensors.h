#ifndef SENSORS_H
#define SENSORS_H

#include <Arduino.h>
#include <BH1750.h>
#include <Adafruit_AHTX0.h>
#include "configs/sensors_conf.h"

#include <SFE_BMP180.h>
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
  float humidity;
  char temphum[30];
  // float windSpeed;
  // float windDirection;
  // char windCardinalDirection[5];
  float HeatIndex;
  float Pressure;
  float PressureSeaLevel;
  float BMEtemperature;
  float RelAltitude;
  // float UVIndex;
  float lux;
  float batteryVoltage;
  int batteryADC;
  int lowBattery;
  float coreC;
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

static const char *SENSOR_INITS_TAG = "SENSOR_INIT";
static const char *SENSOR_I2C_CONN_TAG = "I2C_INIT";
static const char *SENSOR_DATA_EVENTS = "SENSOR_DATA";
static const char *SENSOR_BATTERY_CTL = "BATTERY_CTL";
static const char *Battery = "LOW_BATTERY_PROBLEM";

#endif