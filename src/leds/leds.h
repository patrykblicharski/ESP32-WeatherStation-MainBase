#ifndef LEDS_H
#define LEDS_H
#include <Arduino.h>

#define LED_GREEN 13
#define LED_BLUE 19

void LedState(int pin, bool state);
void LedFade(int pin);
void LedFade(int pin1, int pin2);
void LedBlink(int pin);

void LedBlink(int pin, int times, int tdelay = 300);
void LedBlink2(int pin, int times, int endstate, int tdelay = 300);
#endif