
#include <LiquidCrystal.h>

LiquidCrystal lcd(19, 23, 18, 17, 16, 15);

#include <OneWire.h>
#include <DallasTemperature.h>

/* BME 280 */
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BME280 bme;

#define ONE_WIRE_BUS_TEMP A4
#define GAS_SENSOR A5
#define SCL 22
#define SDA 21

OneWire oneWire(ONE_WIRE_BUS_TEMP);

DallasTemperature sensors(&oneWire);

void setup() {

  lcd.begin(20,4);
  
  Serial.begin(115200);
  
  sensors.begin();
  if (!bme.begin(0x76)) {
		Serial.println("Nebyl pripojen BME 280 sensor !");
		while (1);
	}
}

void loop() {
  sensors.requestTemperatures();
  for(int i = 0;i<3;i++){
    Serial.println(sensors.getTempCByIndex(i));
  }
  Serial.println(analogRead(GAS_SENSOR));

  float bme_temp = bme.readTemperature();
  float bme_pressure = bme.readPressure() / 100.0F;
  float bme_aproxx = bme.readAltitude(SEALEVELPRESSURE_HPA);
  float bme_humidity = bme.readHumidity();

  delay(1000);
}
