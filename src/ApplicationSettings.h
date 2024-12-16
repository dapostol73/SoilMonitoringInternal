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

ApplicationSettings Home(WiFiConnection("homessid", "homepw123"), ThingSpeakInfo("homeWriteAPIKey", "homeReadAPIKey"));
ApplicationSettings Office(WiFiConnection("officessid", "officepw123"), ThingSpeakInfo("officeWriteAPIKey", "officeReadAPIKey"));

ApplicationSettings AppSettings[] = { Home, Office };
uint8_t AppSettingsCount = sizeof(AppSettings) / sizeof(AppSettings[0]);

#endif