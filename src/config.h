#ifndef CONFIG_H
#define CONFIG_H


//WiFi connection
char ssid[]   = "...";
char pass[]  = "...";

int wificonCounter =0;

//MQTT broker connection
//const char* mqttServer = "test.mosquitto.org";
const char* mqttServer = "192.168.5.74";
const int mqttPort = 1883;
const char* mqttUser = "username";
const char* mqttPassword = "password";
const char mainTopic[20] = "MainTopic/";



//Timezone information
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 1 * 3600;
const int   daylightOffset_sec = 3600;

//Set how often to wake and read sensors
const int WakeIntervalUpdateSec = 5 * 60;

#endif