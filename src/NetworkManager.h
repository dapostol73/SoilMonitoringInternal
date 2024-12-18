#ifndef _NETWORK_MANAGER_H_
#define _NETWORK_MANAGER_H_

#include <Arduino.h>
#include <WiFi.h>

#include "ApplicationSettings.h"
#include "SensorData.h"

class NetworkManager
{
    private:
        WiFiClient wiFiClient;

    public:
        NetworkManager();
        bool init();
        bool isConnected();
        int scanSettingsID(ApplicationSettings* aSettings, uint16_t nSettings);
        bool connectWiFi(WiFiConnection wiFiConnection, uint16_t retryAttempts = 2, uint16_t retryDelay = 20);
        void uploadSensorData(ThingSpeakInfo* thingSpeakInfo, SensorData* sensorData, uint16_t sensorCount);
};

#endif