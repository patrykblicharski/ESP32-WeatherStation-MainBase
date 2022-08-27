#include <Arduino.h>
#include "network.h"




bool setup_wifi(char *siec, char *password) {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("* Connecting to \n ");
  Serial.print(siec);

  WiFi.mode(WIFI_STA);
  WiFi.begin(siec, password);
    int wificonCounter = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    wificonCounter++;
    Serial.print(".");
    if (wificonCounter >15) esp_restart();
  }
  Serial.println("");
  

  Serial.println("** WiFi connected");
  Serial.println("*IP address: ");
  Serial.println(WiFi.localIP());

  return true;
}

bool checkI2cTransmission(int address, char name[])
{
    Wire.beginTransmission(address);
    bool bErr = Wire.endTransmission();
    if (bErr == true)
    {
        Serial.printf("%i %c OK",address,name);
        return true;
    }
    else
    {
        Serial.printf("%i %c ERROR",address,name);
        return false;
    }
}
 