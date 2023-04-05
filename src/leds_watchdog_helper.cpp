
#include "leds_watchdog_helper.h"


void led_watchdog_progress_blink(exec_stage stage)
{
    switch (stage)
    {
    case WATCHDOG:
        LedState(LED_BLUE, true);
        delay(1000);
        break;
    case I2CSCAN:
        LedState(LED_GREEN, true);
        delay(1000);
        break;
    case WIFI_TRY:
        LedFade(LED_BLUE, LED_GREEN);
        break;
    case WIFI_CONNECTED:
        LedState(LED_BLUE, true);
        LedState(LED_GREEN, false);
        delay(1000);
        LedBlink2(LED_GREEN, 4, false, 500);
        break;
    case I2COK:
        LedState(LED_BLUE, false);
        LedState(LED_GREEN, false);
        delay(1000);
        LedState(LED_GREEN, true);
        break;
    case I2CERR:
        LedState(LED_BLUE, false);
        LedState(LED_GREEN, false);
        delay(1000);
        for (int i = 0; i < 5; i++)
        {
            delay(1000);
            LedState(LED_BLUE, false);
            LedState(LED_GREEN, true);
            delay(1000);
            LedState(LED_BLUE, true);
            LedState(LED_GREEN, false);
        }
        LedState(LED_BLUE, false);
        LedState(LED_GREEN, false);
        delay(1000);
        break;
    case SENT:
        LedFade(LED_GREEN);
        break;
    case DEEP:
        LedFade(LED_BLUE);
        LedFade(LED_BLUE, LED_GREEN);
        break;
    default:
        break;
    }
}

/*
WATCHDOG = full blue
I2CSCAN = full green and blue
WIFI_TRY = fade blue
WIFI_CONNECTED blue up and blink 3 times green
I2COK = green blik 3
,I2CERR = blue blik 3
,SENT fade green
,DEEP fade blue

void LedState(int pin, bool state);
void LedFade(int pin);
void LedBlink(int pin);
void LedBlink(int pin, int times);
void LedBlink(int pin, int times, int tdelay = 300);
void LedBlink2(int pin, int times, int endstate, int tdelay = 300);
*/