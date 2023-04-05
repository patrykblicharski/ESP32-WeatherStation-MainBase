
#include "leds.h"

void LedFade(int pin)
{
    int brightness = 0;
    int fadeAmount = 10;

    for (int i = 0; i <= 51; i++)
    {
        analogWrite(pin, brightness);
        brightness = brightness + fadeAmount;
        if (brightness <= 0 || brightness >= 255)
        {

            fadeAmount = -fadeAmount;
        }
        else

            delay(30);
    }
    analogWrite(pin, 0);
}
void LedFade(int pin1, int pin2)
{
    int brightness = 0;
    int fadeAmount = 10;

    for (int i = 0; i <= 51; i++)
    {
        analogWrite(pin1, brightness);
        analogWrite(pin2, brightness);
        brightness = brightness + fadeAmount;
        if (brightness <= 0 || brightness >= 255)
        {

            fadeAmount = -fadeAmount;
        }
        else

            delay(30);
    }
    analogWrite(pin1, 0);
    analogWrite(pin2, 0);
}

void LedState(int pin, bool state)
{
    if (state == true)
        analogWrite(pin, 255);
    else
        analogWrite(pin, 0);
}

void LedBlink(int pin)
{
    analogWrite(pin, 255);
    delay(300);
    analogWrite(pin, 0);
    delay(500);
    analogWrite(pin, 255);
}

void LedBlink(int pin, int times, int tdelay)
{
    for (int i = 0; i < times; i++)
    {
        analogWrite(pin, 255);
        delay(tdelay);
        analogWrite(pin, 0);
        delay(tdelay);
    }
}

void LedBlink2(int pin, int times, int endstate, int tdelay)
{
    for (int i = 0; i < times; i++)
    {
        analogWrite(pin, 255);
        delay(tdelay);
        analogWrite(pin, 0);
        delay(tdelay);
    }
    delay(500);
    digitalWrite(pin, endstate);
}