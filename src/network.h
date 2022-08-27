#ifndef NETWORK_H
#define NETWORK_H

#include <Arduino.h>
#include <WiFi.h>
#include <Wire.h>


bool setup_wifi(char *siec, char *password);
bool checkI2cTransmission(int address, char name[]);

#endif