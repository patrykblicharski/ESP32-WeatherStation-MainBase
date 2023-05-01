#include "sensors.h"
#include <Arduino.h>

SFE_BMP180 bmp180;
Adafruit_AHTX0 aht;
BH1750 bh_lux;
sensorStatus status;
sensorExist exist;

float a, b, hi, preasureDiv, lux;
int32_t preasure;

extern "C"
{
    uint8_t temprature_sens_read();
}

float convertFtoC(float f) { return (f - 32) * 0.55555; }

float convertCtoF(float c) { return c * 1.8 + 32; }

double computeDewPoint2(double celsius, double humidity)
{
    double RATIO = 373.15 / (273.15 + celsius); // RATIO was originally named A0, possibly confusing in Arduino context
    double SUM = -7.90298 * (RATIO - 1);
    SUM += 5.02808 * log10(RATIO);
    SUM += -1.3816e-7 * (pow(10, (11.344 * (1 - 1 / RATIO))) - 1);
    SUM += 8.1328e-3 * (pow(10, (-3.49149 * (RATIO - 1))) - 1);
    SUM += log10(1013.246);
    double VP = pow(10, SUM - 3) * humidity;
    double T = log(VP / 0.61078); // temp var
    return (241.88 * T) / (17.558 - T);
}

// delta max = 0.6544 wrt dewPoint()
// 5x faster than dewPoint()
// reference: http://en.wikipedia.org/wiki/Dew_point
double dewPointFast(double celsius, double humidity)
{
    double a = 17.271;
    double b = 237.7;
    double temp = (a * celsius) / (b + celsius) + log(humidity * 0.01);
    double Td = (b * temp) / (a - temp);
    return Td;
}

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

bool checkI2cTransmission(int address, bool *setStatus)
{
    Serial.print("\n#####################\n");
    Serial.print("##SENSOR I2c INITS##\n");
    Serial.println("#####################\n");

    Wire.beginTransmission(address);
    byte bErr = Wire.endTransmission();
    if (bErr == 0)
    {
        ESP_LOGI(SENSOR_I2C_CONN_TAG, "^^ Address:%i  -> OK", address);
        *setStatus = true;
        return true;
    }
    else
    {
        Serial.print("!!!!!!!!!!!!!");
        ESP_LOGE(SENSOR_I2C_CONN_TAG, " Address:%i  -> ERROR\n", address);
        *setStatus = false;
        return false;
    }
}

void sensorCheck(void)
{

    if ((BMP180Enable) && checkI2cTransmission(BMP180_ADDRESS, &exist.bmp))
    {
        status.bmp = bmp180.begin();
        ESP_LOGI(SENSOR_INITS_TAG, "# BME280 # status.exist=%i status.begin()=%i #", exist.bmp, status.bmp);
    }
    else
        ESP_LOGE(SENSOR_INITS_TAG, "# BME280 # status.exist=%i status.begin()=%i #", exist.bmp, status.bmp);

    // ATH20 sensor
    if ((AHTX0Enable) && checkI2cTransmission(AHT20X_ADDRESS, &exist.ath))
    {
        status.ath = aht.begin();
        ESP_LOGI(SENSOR_INITS_TAG, "# AHT20X # status.exist=%i status.begin()=%i #", exist.ath, status.ath);
        sensors_event_t humidity, temp;
        aht.getEvent(&humidity, &temp);
    }
    else
        ESP_LOGE(SENSOR_INITS_TAG, "# AHT20X # status.exist=%i status.begin()=%i #", exist.ath, status.ath);

    // BH1750 sensor
    if ((BH1750Enable) && checkI2cTransmission(BH1750_ADDRESS, &exist.lux))
    {
        status.lux = bh_lux.begin();
        ESP_LOGI(SENSOR_INITS_TAG, "# BH1750 LUX # status.exist=%i status.begin()=%i #", exist.lux, status.lux);
    }
    else

        ESP_LOGE(SENSOR_INITS_TAG, "# BH1750 LUX # status.exist=%i status.begin()=%i #", exist.lux, status.lux);
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
    Serial.println();
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
        environment->dewpoint1 = dewPointFast(environment->temperatureC, environment->humidity);
        environment->dewpoint2 = computeDewPoint2(environment->temperatureC,environment->humidity);
        sprintf_P(environment->temphum, PSTR("%.02f;%i"), environment->temperatureC, environment->humidity);
        ESP_LOGI(SENSOR_DATA_EVENTS, "SENSOR:ATH20X: Temp:%fC Hum:%f %  HeatIndex:%f", environment->temperatureC, environment->humidity, environment->HeatIndex);
    }
    if (exist.bmp == 1) // bmp180
    {
        char checkpoint;
        double tempbmp180, preasure, preasure_sea, relaltitude;

        checkpoint = bmp180.startTemperature();
        if (checkpoint != 0)
        {
            delay(checkpoint);

            checkpoint = bmp180.getTemperature(tempbmp180); // GET TEMPERATURE
            environment->BMEtemperature = tempbmp180;

            if (checkpoint != 0)
            {
                checkpoint = bmp180.startPressure(3); // The parameter is the oversampling setting, from 0 to 3 (highest res, longest wait).
                if (checkpoint != 0)
                {
                    delay(checkpoint); // If request is successful, the number of ms to wait is returned.

                    checkpoint = bmp180.getPressure(preasure, tempbmp180); // GET PREASUERE RELATIVE
                    if (checkpoint != 0)
                    {

                        environment->PressureSeaLevel = bmp180.sealevel(preasure, ALTITUDE);                 // GET PREASURE according to ALTITUDE
                        environment->RelAltitude = bmp180.altitude(preasure, environment->PressureSeaLevel); // GET Altitude from subtraction between preasure on sea level and relative
                    }
                    else
                        ESP_LOGE(SENSOR_DATA_EVENTS, "SENSOR:BME180: Step4 : error #retrieving pressure# measurement");
                }
                else
                    ESP_LOGE(SENSOR_DATA_EVENTS, "SENSOR:BME180: Step3 : error #starting pressure# measurement");
            }
            else
                ESP_LOGE(SENSOR_DATA_EVENTS, "SENSOR:BME180: Step2 : error #retrieving temperature# measurement");
        }
        else
            ESP_LOGE(SENSOR_DATA_EVENTS, "SENSOR:BME180: Step1 : error #starting temperature# measurement");

        ESP_LOGI(SENSOR_DATA_EVENTS, "SENSOR:BME180: Temp:%fC Preasure:%f %  Relative altitude:%f", environment->BMEtemperature, environment->PressureSeaLevel, environment->RelAltitude);
    }

    if (exist.lux == 1)
    {
        environment->lux = bh_lux.readLightLevel();
        ESP_LOGI(SENSOR_DATA_EVENTS, "SENSOR:BMP180: BMETemp:%f Preasure:%f SeaLevelPreasure:%f");
    }
}