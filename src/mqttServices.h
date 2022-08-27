#ifndef MQTTSERVICES_H
#define MQTTSERVICES_H

#include <Arduino.h>
 #include <PubSubClient.h>

void MQTTconnect(PubSubClient &mqttClient);

void MQTTSend(char *topicBuffer, char *payload, bool retain, PubSubClient &mqttClient);
void MQTTPublish(const char topic[], char *value, bool retain, PubSubClient &mqttClient); //  MQTTPublishString
void MQTTPublish(const char topic[], int value, bool retain, PubSubClient &mqttClient) ;//  MQTTPublishInt
void MQTTPublish(const char topic[], long value, bool retain, PubSubClient &mqttClient); //  MQTTPublish Long
void MQTTPublish(const char topic[], float value, bool retain, PubSubClient &mqttClient); //  MQTTPublishFloat
void MQTTPublish(const char topic[], bool value, bool retain, PubSubClient &mqttClient); //  MQTTPublishBool


 #endif