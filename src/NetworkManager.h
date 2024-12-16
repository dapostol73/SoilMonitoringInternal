#ifndef _NETWORK_MANAGER_H_
#define _NETWORK_MANAGER_H_

#include <Arduino.h>
#include <WiFi.h>

#include "WiFiConnection.h"

class NetworkManager
{
    private:
        WiFiConnection* wifiOptions;
        uint16_t numberOptions = 0;

    public:
        NetworkManager();
        void init(WiFiConnection* wifiSelections, uint16_t selectionSize);
};

#endif