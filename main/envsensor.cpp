#include <M5StickC.h>
#include "envsensor.h"
#include "Adafruit_Sensor.h"
#include <Adafruit_BMP280.h>
//#include <Wire.h> //The DHT12 uses I2C comunication.
#include "DHT12.h"

//#include <OneWire.h>
//#include <DallasTemperature.h>


DHT12 dht12; //Preset scale CELSIUS and ID 0x5c.
Adafruit_BMP280 bme;

//DallasTemperature sensors(&oneWire);

void EnvSensorScreen::onSetup()
{
    int sclPin = GPIO_NUM_33;
    int sdaPin = GPIO_NUM_32;
    if( ! Wire.begin( sdaPin, sclPin, 0) ) {
      Serial.println("Could not find a valid BMP280 sensor, check wiring!");
      M5.Lcd.println("Could not find a valid BMP280 sensor, check wiring!");
    }

    Serial.println(F("ENV Unit(DHT12 and BMP280) test..."));

    if (!bme.begin(0x76)){  
      Serial.println("Could not find a valid BMP280 sensor, check wiring!");
      M5.Lcd.println("Could not find a valid BMP280 sensor, check wiring!");
    }



    //sensors.begin();


    startUITimer(100);
}

void EnvSensorScreen::onLongPress()
{
    //OneWire *pOneWire = new OneWire();
    //pOneWire->begin(GPIO_NUM_36);
    //uint8_t resp = pOneWire->reset();
    //Serial.printf("Reset response: %d\n", resp);
    //M5.Lcd.printf("Reset response: %d\r\n", resp);

}

void EnvSensorScreen::onRepaint()
{
//    float tmp = dht12.readTemperature();
//    float hum = dht12.readHumidity();
//    float pressure = bme.readPressure();
    //Serial.printf("Temperatura: %2.2f*C  Humedad: %0.2f%%  Pressure: %0.2fPa\r\n", tmp, hum, pressure);



    M5.Lcd.setCursor(0, 0);
    M5.Lcd.println("DHT12:");
    M5.Lcd.printf("Temp: %2.1f\r\n", dht12.readTemperature() );
    M5.Lcd.printf("Humi: %2.0f%%", dht12.readHumidity() );
    M5.Lcd.println("");
    M5.Lcd.println("BMP280:");
    M5.Lcd.printf("Temp: %2.1f\r\n", bme.readTemperature() );
    M5.Lcd.printf("Pressure:%2.0fPa\r\n", bme.readPressure() );

    //sensors.requestTemperatures(); 
    //float temperatureC = sensors.getTempCByIndex(0);
    M5.Lcd.println("");
    M5.Lcd.println("DS18B20:");
    //M5.Lcd.printf("Temp: %2.1f\r\n", temperatureC );
    //M5.Lcd.printf("Device cnt: %d\r\n", sensors.getDeviceCount());
}
