#include <WiFi.h>
#include <HTTPClient.h>
#include <LiquidCrystal.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Wire.h>
#include <Adafruit_BME280.h>
#include <Arduino.h>
#include <ArduinoJson.h>

#include "Led.h"
#include "Buzzer.h"

#define ONE_WIRE_BUS_TEMP 32
#define GAS_SENSOR 35


OneWire oneWire(ONE_WIRE_BUS_TEMP);
DallasTemperature sensors(&oneWire);
LiquidCrystal lcd(19, 23, 18, 17, 16, 15);
Adafruit_BME280 bme;

struct Config{
    String serverName = "http://c6c2fbd97d6e.eu.ngrok.io/add.php"; //todo: Nalepit na sebe cestu a soubor
    const char* ssid = "Sumbark.NET.Q7";
    const char* password = "555dRIVE";
    String apiKeyValue = "tPmAT5Ab3j7F9";
};


Config config;

struct Button{
    const uint8_t PIN;
    bool pressed;
};

Led led1(12);
Led led2(14);
Buzzer buzzer(27);

Button button1 = {26, false};

String ErrorLog(int httpResponseCode)
{
    if(httpResponseCode >= 400 || httpResponseCode <= 0){
        led1.on();
        led2.off();
    }else if(httpResponseCode == 200){
        led2.on();
    }
    switch(httpResponseCode){
        case 400: return "Bad Request"; break;
        case 401: return "Unauthorized"; break;
        case 403: return "Forbidden"; break;
        case 404: return "Not Found"; break;
        case 200: return "OK"; break;
        default: return "Error" + String(httpResponseCode); break;
    }
}

void IRAM_ATTR isr() {
    button1.pressed = true;
}

void setup() {
    pinMode(button1.PIN, INPUT_PULLUP);
    attachInterrupt(button1.PIN, isr, FALLING);
    Serial.begin(115200);
    WiFi.begin(config.ssid, config.password);
    lcd.begin(20,4);
    sensors.begin();

    for(int i=0;i<3;i++){
        buzzer.beep(100);
    }

    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to WiFi..");
    }
    Serial.println("Connected to the WiFi network");

    if (!bme.begin(0x76)) {
        Serial.println("Nebyl nalezen BME sensor !");
        while (1);
    }
}

void loop() {
    if (button1.pressed) {
        button1.pressed = false;
        led1.off();
    }

    if(WiFi.status()== WL_CONNECTED){
        sensors.requestTemperatures();

        DynamicJsonDocument data(2048);

        data["api_key"] = config.apiKeyValue;
        data["dallas_1"] = sensors.getTempCByIndex(0);
        data["dallas_2"] = sensors.getTempCByIndex(1);
        data["dallas_3"] = sensors.getTempCByIndex(2);
        data["bme_temp"] = bme.readTemperature();
        data["bme_pressure"] = bme.readPressure() / 100.0F;
        data["bme_approx"] = bme.readAltitude(1013.25);
        data["bme_humidity"] = bme.readHumidity();
        data["gas_sensor"] = analogRead(GAS_SENSOR);

        String json;
        serializeJson(data, json);

        HTTPClient http;
        http.begin(config.serverName);
        http.addHeader("Content-Type", "application/json");
        Serial.println(ErrorLog(http.POST(json)));
        Serial.println(http.getString());
        http.end();
    }
    else
        Serial.println("WiFi Disconnected");
    delay(15000);
}
