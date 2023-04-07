#ifndef LEDS_WATCHDOG_HELPER_H
#define LEDS_WATCHDDOG_HELPER_H
#include "leds.h"
enum exec_stage
{
    WATCHDOG,
    I2CSCAN,
    WIFI_TRY,
    WIFI_CONNECTED,
    I2COK,
    I2CERR,
    SENT,
    DEEP
};
void led_watchdog_progress_blink(exec_stage stage);
#endif