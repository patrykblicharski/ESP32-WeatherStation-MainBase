#include <Arduino.h>
#include "mqttServices.h"
#include "mqttAuth.h"



void MQTTconnect(PubSubClient &mqttClient)
{
  while (!mqttClient.connected())
  {
    mqttClient.setServer(mqttServer, mqttPort);
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (mqttClient.connect(mqttServer, mqttUser, mqttPassword))
    {
      Serial.println("Connected");
    }
    else
    {
      Serial.print("failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}


void MQTTSend(char *topicBuffer, char *payload, bool retain, PubSubClient &mqttClient) //  MQTTSend: MQTT send topic with value to broker
{
  int status = 0;
  int retryCount = 0;
#ifdef ExtendedMQTT
  Serial.print("%s: %s\n", topicBuffer, payload);
#endif
  while (!status && retryCount < 5)
  {
    status = mqttClient.publish(topicBuffer, payload, retain);
#ifdef ExtendedMQTT
    Serial.print("MQTT status: %i\n", status);
#endif
    delay(50);
    retryCount++;
  }
}




void MQTTPublish(const char topic[], char *value, bool retain, PubSubClient &mqttClient) //  MQTTPublishString
{
  char topicBuffer[256];
  char payload[256];

  strcpy(topicBuffer, mainTopic);
  strcat(topicBuffer, topic);
  if (!mqttClient.connected())
    MQTTconnect(mqttClient);
  mqttClient.loop();
  sprintf(payload, "%s", value);
  MQTTSend(topicBuffer, payload, retain,mqttClient);
}
void MQTTPublish(const char topic[], int value, bool retain, PubSubClient &mqttClient) //  MQTTPublishInt
{
  char topicBuffer[256];
  char payload[256];

  strcpy(topicBuffer, mainTopic);
  strcat(topicBuffer, topic);
  if (!mqttClient.connected())
    MQTTconnect(mqttClient);
  mqttClient.loop();
  sprintf(payload, "%i", value);
  MQTTSend(topicBuffer, payload, retain,mqttClient);
}
void MQTTPublish(const char topic[], long value, bool retain, PubSubClient &mqttClient) //  MQTTPublish Long
{
  char topicBuffer[256];
  char payload[256];

  strcpy(topicBuffer, mainTopic);
  strcat(topicBuffer, topic);
  if (!mqttClient.connected())
    MQTTconnect(mqttClient);
  mqttClient.loop();
  sprintf(payload, "%li", value);
  MQTTSend(topicBuffer, payload, retain,mqttClient);
}
void MQTTPublish(const char topic[], float value, bool retain, PubSubClient &mqttClient) //  MQTTPublishFloat
{
  char topicBuffer[256];
  char payload[256];

  strcpy(topicBuffer, mainTopic);
  strcat(topicBuffer, topic);
  if (!mqttClient.connected())
    MQTTconnect(mqttClient);
  mqttClient.loop();
  sprintf(payload, "%6.3f", value);
  MQTTSend(topicBuffer, payload, retain,mqttClient);
}
void MQTTPublish(const char topic[], bool value, bool retain, PubSubClient &mqttClient) //  MQTTPublishBool
{
  char topicBuffer[256];
  char payload[256];

  strcpy(topicBuffer, mainTopic);
  strcat(topicBuffer, topic);
  if (!mqttClient.connected())
    MQTTconnect(mqttClient);
  mqttClient.loop();
  if (value)
  {
    sprintf(payload, "true");
  }
  else
  {
    sprintf(payload, "false");
  }
  MQTTSend(topicBuffer, payload, retain,mqttClient);}
