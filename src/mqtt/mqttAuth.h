#ifndef MQTTAUTH_H
#define MQTTAUTH_H
// Pins def.

#define ExtraDebugMQTT 1; 


// MQTT broker connection
// const char* mqttServer = "test.mosquitto.org";
char mqttServer[] = "blacha14.ddns.net";
const int mqttPort = 1883;
char  mqttUser[] = "blacha14";
char   mqttPassword[] = "macstaticroot1";
char mainTopic[20] = "Meteotest/";

 
#endif