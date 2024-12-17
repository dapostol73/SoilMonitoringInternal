#ifndef _NETWORK_MANAGER_H_
#define _NETWORK_MANAGER_H_

#include <Arduino.h>
#include <WiFi.h>

#include "ApplicationSettings.h"

class NetworkManager
{
    private:
        WiFiConnection wiFiConnection;
        ApplicationSettings* appSettings;
        uint16_t numOfSettings = 0;

        void scanWiFiSettings();

    public:
        NetworkManager();
        bool init(ApplicationSettings* aSettings, uint16_t nSettings);
        bool connectWiFi(uint16_t retryAttempts = 2, uint16_t retryDelay = 20);
        bool isConnected();
};

#endif