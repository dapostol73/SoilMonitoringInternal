
//*****************************************************************************
// Copyright (c) 2014 A12 Studios Inc. and Demetrius Apostolopoulos.
// All rights reserved.
//
// If Serial Logging support is required add this build flag to platformio.ini
//   build_flags = -D SERIAL_LOGGING
//*****************************************************************************
#include <Arduino.h>
#include <Wire.h>
#include <WiFi.h>

#include "UserSettings.h"
#include "NetworkManager.h"
#include "DisplayMain.h"
#include "SensorData.h"

#ifndef SERIAL_LOGGING
// disable Serial output
#define Serial KillDefaultSerial
static class {
public:
    void begin(...) {}
    void print(...) {}
    void println(...) {}
} Serial;
#endif

#define MS_SENSOR0 A3
#define SERIAL_BAUD_RATE 115200

ApplicationSettings appSettings; //change to pointer
NetworkManager netManager;
DisplayMain displayMain;

const uint8_t SENSORS_COUNT = 8;
const uint16_t SENSORS_UPDATE_SECS = 1; // Sensor query every minute
const uint16_t SENSORS_UPLOAD_SECS = 5; // Sensor query every minute
const uint8_t sensorPins[SENSORS_COUNT] = { A3, A4, A5, A6, A14, A15, A16, A17 };
SensorData sensorData[SENSORS_COUNT];
long sensorTimeLastUpdate = LONG_MIN;
long sensorTimeLastUpload = LONG_MIN;

const uint16_t WIFI_UPDATE_SECS = 120; // wait 2 minutes to reconnect
long wiFiTimeLastUpdate = LONG_MIN;

void blinkLed(uint8_t times, uint8_t interval);
void configureSensors();
void updateSensors();

void setup()
{
    Serial.begin(SERIAL_BAUD_RATE);
	#ifdef SERIAL_LOGGING
		while(!Serial);
	#endif

    // initialize digital pin PB2 as an output.
    pinMode(BUILTIN_LED, OUTPUT);

    delay(250); // wait for the OLED to power up
    displayMain.init();
	netManager.init();
    uint8_t appSetID = netManager.scanSettingsID(AppSettings, AppSettingsCount);
    appSettings = AppSettings[appSetID];
	netManager.connectWiFi(appSettings.WifiSettings);
	displayMain.printWiFiInfo();
    wiFiTimeLastUpdate = millis();
	configureSensors();
	blinkLed(3, 250);	
	Serial.println("Setup complete!");
}

void loop()
{
	if (!netManager.isConnected() && millis() - wiFiTimeLastUpdate > (1000L*WIFI_UPDATE_SECS))
    {
        #ifdef SERIAL_LOGGING
        Serial.println("Attempting to connect to WiFi");
        #endif
		if (!netManager.connectWiFi(appSettings.WifiSettings));
        {
            //If a connection failed, rescan for new settings.
            uint8_t appSetID = netManager.scanSettingsID(AppSettings, AppSettingsCount);
            appSettings = AppSettings[appSetID];
        }
        wiFiTimeLastUpdate = millis();
    }

    if (millis() - sensorTimeLastUpdate > (1000L*SENSORS_UPDATE_SECS))
    {
        #ifdef SERIAL_LOGGING
        Serial.println("Updating Sensor data.");
        #endif
        updateSensors();
        sensorTimeLastUpdate = millis();
    }

    if (millis() - sensorTimeLastUpload > (1000L*SENSORS_UPLOAD_SECS))
    {
        #ifdef SERIAL_LOGGING
        Serial.println("Uploading Sensor data to ThinkSpeak.");
        #endif
        netManager.uploadSensorData(&appSettings.ThingSpeakSettings, sensorData, SENSORS_COUNT);
        sensorTimeLastUpload = millis();
        blinkLed(4, 100);
    }

    TouchPoint touch = displayMain.DisplayTouch->getTouch();

	// Display touches that have a pressure value (Z)
	if (touch.zRaw != 0)
    {
        #ifdef SERIAL_LOGGING
        Serial.print("Touch at X: ");
        Serial.print(touch.x);
        Serial.print(", Y: ");
        Serial.println(touch.y);
        #endif
        char info[48] = "";
        sprintf(info, "Last Touch at X: %d Y: %d", touch.x, touch.y);
        displayMain.DisplayGFX->fillRect(0, 0, 480, 32, BACKGROUND_COLOR);
        displayMain.drawString(info, 4, 4, TEXT_LEFT_TOP, TEXT_MAIN_COLOR);
		//uint16_t x = clamp(touch.x, 5, 475);
		//uint16_t y = clamp(touch.y, 5, 315);
		//displayMain.DisplayGFX->fillCircle(x, y, 4, GREEN);
    }

	displayMain.drawWiFiSignal(452, 4, 2);
}

void configureSensors()
{
    uint16_t w = 60;
    for (uint16_t i = 0; i < SENSORS_COUNT; i++)
    {
		sensorData[i].init(sensorPins[i], i, (i * w), 32);
        displayMain.updateMositureMeter(&sensorData[i], true);
    }
}

void updateSensors()
{
    for (uint16_t i = 0; i < SENSORS_COUNT; i++)
    {
        sensorData[i].readValue();
        displayMain.updateMositureMeter(&sensorData[i]);
    }
}

void blinkLed(uint8_t times, uint8_t interval)
{
    for (uint8_t i = 0; i < times; i++)
    {
        digitalWrite(BUILTIN_LED, HIGH);   // turn the LED on (HIGH is the voltage level)
        delay(interval);               // wait for 100mS
        digitalWrite(BUILTIN_LED, LOW);    // turn the LED off by making the voltage LOW
        delay(interval);               // wait for 100mS
    }
}

