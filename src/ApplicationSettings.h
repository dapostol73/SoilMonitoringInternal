#ifndef _APP_SETTINGS_
#define _APP_SETTINGS_

#include "WiFiConnection.h"
#include "ThingSpeakInfo.h"

struct ApplicationSettings
{
    WiFiConnection WifiSettings;
    ThingSpeakInfo ThingSpeakSettings;

    ApplicationSettings() = default;

    ApplicationSettings(WiFiConnection wifiSettings, ThingSpeakInfo thingSpeakSettings)
    {
        WifiSettings = wifiSettings;
        ThingSpeakSettings = thingSpeakSettings;
    }
};

#endif