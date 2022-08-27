#include <Arduino.h>

void LedFade(int pin, String name)
{
    int brightness = 0;
    int fadeAmount = 10;
    Serial.print("**Led '");
    Serial.print(name);
    Serial.print("' at GPIO:");
    Serial.println(pin);
    Serial.print(" -");
    for (int i = 0; i <= 51; i++)
    {
        analogWrite(pin, brightness);
        brightness = brightness + fadeAmount;
        if (brightness <= 0 || brightness >= 255)
        {
            Serial.print("@");
            fadeAmount = -fadeAmount;
        }
        else
            Serial.print(">");
        delay(30);
    }
    analogWrite(pin, 0);
}

void LedBlink(int pin)
{
    digitalWrite(pin, HIGH);
    delay(300);
    digitalWrite(pin, LOW);
    delay(500);
    digitalWrite(pin, HIGH);
}

void LedBlink(int pin, int times)
{
    for (int i = 0; i < times; i++)
    {
        digitalWrite(pin, HIGH);
        delay(300);
        digitalWrite(pin, LOW);
        delay(500);
    }
}
void LedBlink(int pin, int times, int endstate)
{
    for (int i = 0; i < times; i++)
    {
        digitalWrite(pin, HIGH);
        delay(300);
        digitalWrite(pin, LOW);
        delay(500);
    }
    delay(500);
    digitalWrite(pin, endstate);
}