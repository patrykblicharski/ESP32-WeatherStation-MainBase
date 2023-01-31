#include "sensors.h"
#include <Arduino.h>
// #include "globals.h"

Adafruit_BMP085 bmp;
Adafruit_AHTX0 aht;
BH1750 bh_lux;
sensorStatus status;
sensorExist exist;

float a, b, hi, preasureDiv, lux;
int32_t preasure;

static const char *SENSOR_INITS_TAG = "SENSOR_INIT";
static const char *SENSOR_I2C_CONN_TAG = "I2C_INIT";
static const char *SENSOR_DATA_EVENTS = "SENSOR_DATA";
static const char *SENSOR_BATTERY_CTL = "BATTERY_CTL";

extern "C"
{
    uint8_t temprature_sens_read();
}

float convertFtoC(float f) { return (f - 32) * 0.55555; }

float convertCtoF(float c) { return c * 1.8 + 32; }

float computeHeatIndex(float temperature, float percentHumidity,
                       bool isFahrenheit)
{
    float hi;

    if (!isFahrenheit)
        temperature = convertCtoF(temperature);

    hi = 0.5 * (temperature + 61.0 + ((temperature - 68.0) * 1.2) +
                (percentHumidity * 0.094));

    if (hi > 79)
    {
        hi = -42.379 + 2.04901523 * temperature + 10.14333127 * percentHumidity +
             -0.22475541 * temperature * percentHumidity +
             -0.00683783 * pow(temperature, 2) +
             -0.05481717 * pow(percentHumidity, 2) +
             0.00122874 * pow(temperature, 2) * percentHumidity +
             0.00085282 * temperature * pow(percentHumidity, 2) +
             -0.00000199 * pow(temperature, 2) * pow(percentHumidity, 2);

        if ((percentHumidity < 13) && (temperature >= 80.0) &&
            (temperature <= 112.0))
            hi -= ((13.0 - percentHumidity) * 0.25) *
                  sqrt((17.0 - abs(temperature - 95.0)) * 0.05882);

        else if ((percentHumidity > 85.0) && (temperature >= 80.0) &&
                 (temperature <= 87.0))
            hi += ((percentHumidity - 85.0) * 0.1) * ((87.0 - temperature) * 0.2);
    }

    return isFahrenheit ? hi : convertFtoC(hi);
}

/* void testsensor(void)
{
    if (checkI2cTransmission(AHT20X_ADDRESS, &exist.ath))
    {
        Wire.end();
        status.ath = aht.begin();
        Serial.println(status.ath);
        Serial.println(exist.ath);

        sensors_event_t humidity, temp;
        aht.getEvent(&humidity, &temp);
    }
}*/

void sensorCheck(void)
{

    if (checkI2cTransmission(BMP180_ADDRESS, &exist.bmp))
    {
        status.bmp = bmp.begin();

        ESP_LOGI(SENSOR_INITS_TAG, "# BMP180 # status.exist=%i status.begin()=%i #", exist.bmp, status.bmp);
    }
    else
    {
        ESP_LOGE(SENSOR_INITS_TAG, "# BMP180 # status.exist=%i status.begin()=%i #", exist.bmp, status.bmp);
    }
    if (checkI2cTransmission(AHT20X_ADDRESS, &exist.ath))
    {
        status.ath = aht.begin();
        ESP_LOGI(SENSOR_INITS_TAG, "# AHT20X # status.exist=%i status.begin()=%i #", exist.ath, status.ath);
        sensors_event_t humidity, temp;
        aht.getEvent(&humidity, &temp);
    }
    else
    {
        ESP_LOGE(SENSOR_INITS_TAG, "# AHT20X # status.exist=%i status.begin()=%i #", exist.ath, status.ath);
    }
    if (checkI2cTransmission(LUX_ADDRESS, &exist.lux))
    {
        status.lux = bh_lux.begin();
        ESP_LOGI(SENSOR_INITS_TAG, "# BH1750 LUX # status.exist=%i status.begin()=%i #", exist.lux, status.lux);
    }
    else
    {
        Serial.println("\n!!!!!!!!!!!!!");
        ESP_LOGE(SENSOR_INITS_TAG, "# BH1750 LUX # status.exist=%i status.begin()=%i #", exist.lux, status.lux);
    }
}

bool checkI2cTransmission(int address, bool *setStatus)
{
    Serial.print("\n#####################\n");
    Serial.print("##SENSOR I2c INITS##\n");
    Serial.println("#####################\n");

    Wire.beginTransmission(address);
    byte bErr = Wire.endTransmission();
    if (bErr == 0)
    {
        ESP_LOGI(SENSOR_I2C_CONN_TAG, "^^ Address:%i  -> OK", exist.lux, status.lux);
        *setStatus = true;
        return true;
    }
    else
    {
        Serial.println("!!!!!!!!!!!!!");
        ESP_LOGE(SENSOR_I2C_CONN_TAG, " Address:%i  -> ERROR\n", exist.lux, status.lux);
        *setStatus = false;
        return false;
    }
}

void checkBatteryVoltage(struct sensorData *environment)
{
    Serial.print("\n#####################\n");
    Serial.print("###BATTERY CONTROL###\n");
    Serial.println("#####################\n");

    ESP_LOGI(SENSOR_BATTERY_CTL, "Battery probing");
    environment->batteryADC = analogRead(BATTERY_PIN);
    environment->batteryVoltage = environment->batteryADC * batteryCalFactor;
    ESP_LOGI(SENSOR_BATTERY_CTL, "Battery: ADC:%i Voltage:%f", environment->batteryADC, environment->batteryVoltage);
}

void readSensors(struct sensorData *environment)
{
    Serial.print("\n#####################\n");
    Serial.print("##SENSOR EVENTS##\n");
    Serial.println("#####################\n");

    if (exist.ath == 1)
    {
        sensors_event_t humidity, temp;
        if (aht.getEvent(&humidity, &temp))
            ESP_LOGI(SENSOR_DATA_EVENTS, "SENSOR:ATH20X: Found! Request for data ...");
        environment->temperatureC = temp.temperature;
        environment->humidity = humidity.relative_humidity;
        environment->HeatIndex = computeHeatIndex(temp.temperature, humidity.relative_humidity, false);
        sprintf_P(environment->temphum, PSTR("%.02f;%.02f"), environment->temperatureC, environment->humidity);
        ESP_LOGI(SENSOR_DATA_EVENTS, "SENSOR:ATH20X: Temp:%fC Hum:%f %  HeatIndex:%f", environment->temperatureC, environment->humidity, environment->HeatIndex);
    }
    if (exist.bmp == 1)
    {

        environment->BMEtemperature = bmp.readTemperature();
        ESP_LOGI(SENSOR_DATA_EVENTS, "SENSOR:BMP180: Found! Request for data ...");
        preasureDiv = bmp.readPressure() / 100.0;
        environment->barometricPressure = preasureDiv;
        environment->barometricSeaLevelPressure = (bmp.readSealevelPressure() / 100.0);
        ESP_LOGI(SENSOR_DATA_EVENTS, "SENSOR:BMP180: BMETemp:%fC Preasure:%fHPa SeaLevelPreasure:%fHPa");
    }
    if (exist.lux == 1)
    {
        environment->lux = bh_lux.readLightLevel();
        ESP_LOGI(SENSOR_DATA_EVENTS, "SENSOR:BMP180: BMETemp:%f Preasure:%f SeaLevelPreasure:%f");
    }
}