#include <Arduino.h>
#include "mqttServices.h"
#include "mqttAuth.h"

static const char *MQTT_Connect_TAG = "MQTT";
static const char *MQTT_Send_TAG = "MQTT_SEND";
static const char *MQTT_GotRequest_TAG = "MQTT_DATA";

bool MQTTconnect(PubSubClient &mqttClient)
{
  Serial.print("#####################\n");
  Serial.print("#MQTT SERVICE MODULE#\n");
  Serial.println("#####################\n");

  int tries = 1; // How many mqttClient try to connect after fail
  while (!mqttClient.connected())
  {
    mqttClient.setServer(mqttServer, mqttPort);
    ESP_LOGI(MQTT_Connect_TAG, "Connection attempt %i...", tries);
    // Attempt to connect
    if (mqttClient.connect(mqttServer, mqttUser, mqttPassword))
    {
      ESP_LOGI(MQTT_Connect_TAG, "Connection attempt %i ... Connected", tries);
      return true;
    }
    else
    {
      int mqtt_rc_state = mqttClient.state();
      ESP_LOGE(MQTT_Connect_TAG, "Connection attempt %i/5 failed return code=%i, Retrying in 5 seconds...", tries, mqtt_rc_state);

      delay(5000);
      tries++;
      if (tries > 6)
      {
        ESP_LOGE(MQTT_Connect_TAG, "ERROR! Connection failed after 5 attempts (RC=%i) . STOP, Going to deep sleep", mqtt_rc_state);
        return false;
      }
    }
  }
  // Serial.println();

  return true;
}

void MQTTSend(char *topicBuffer, char *payload, bool retain, PubSubClient &mqttClient) //  MQTTSend: MQTT send topic with value to broker
{
  int status = 0;
  int retryCount = 0;
#ifdef ExtraDebugMQTT
  ESP_LOGD(MQTT_GotRequest_TAG, "Got request -> '%s':'%s'", topicBuffer, payload);
#else
  ESP_LOGI(MQTT_Send_TAG, "Sending request status=%i retryCount=%i ", status, retryCount);
#endif

  while (!status && retryCount < 5)
  {
    status = mqttClient.publish(topicBuffer, payload, retain);
    if (status)
      ESP_LOGI(MQTT_Send_TAG, "Sending request, status=%i retryCount=%i [OK]", status, retryCount);
    else
      ESP_LOGI(MQTT_Send_TAG, "Retrying request, status=%i retryCount=%i [Retry]", status, retryCount);
    delay(50);
    retryCount++;
  }

  if (retryCount >= 5)
  {
    ESP_LOGE(MQTT_Send_TAG, "Request FAILED , in status=%i after retryCount=%i", status, retryCount);
    ESP_LOGE(MQTT_Send_TAG, "^^^^^^^ on '%s':'%s'", topicBuffer, payload);
  }
}

void MQTTPublish(const char topic[], char *value, bool retain, PubSubClient &mqttClient)
{
  char topicBuffer[256];
  char payload[256];

  strcpy(topicBuffer, mainTopic);
  strcat(topicBuffer, topic);
  if (!mqttClient.connected())
    MQTTconnect(mqttClient);
  mqttClient.loop();
  sprintf(payload, "%s", value);
  MQTTSend(topicBuffer, payload, retain, mqttClient);
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
  MQTTSend(topicBuffer, payload, retain, mqttClient);
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
  MQTTSend(topicBuffer, payload, retain, mqttClient);
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
  MQTTSend(topicBuffer, payload, retain, mqttClient);
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
  MQTTSend(topicBuffer, payload, retain, mqttClient);
}
