#ifndef MQTTAUTH_H
#define MQTTAUTH_H
// Pins def.

 

// MQTT broker connection
// const char* mqttServer = "test.mosquitto.org";
char mqttServer[] = "x";
const int mqttPort = 1883;
char  mqttUser[] = "username";
char   mqttPassword[] = "password";
char mainTopic[20] = "MainTopic/";

 
#endif