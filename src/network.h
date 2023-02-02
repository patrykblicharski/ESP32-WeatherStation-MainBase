#ifndef NETWORK_H
#define NETWORK_H

#include <Arduino.h>
#include <WiFiMulti.h>
#include <esp_log.h>
#include <WiFi.h>
#include <Wire.h>

#include <string>
#include <vector>


void scan_wifi();
//bool setup_wifi(char *siec, char *password);//function rejeted in future
bool setup_multi_wifi(char (*ssid)[20], char(*passwd)[20]);



#endif