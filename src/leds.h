#ifndef LEDS_H
#define LEDS_H

#include <Arduino.h>

void LedFade(int pin, String name);
void LedBlink(int pin);
void LedBlink(int pin, int times);
void LedBlink(int pin, int times, int type);

#endif