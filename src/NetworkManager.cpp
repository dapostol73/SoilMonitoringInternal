#include "NetworkManager.h"


NetworkManager::NetworkManager()
{
}

bool NetworkManager::init(ApplicationSettings* aSettings, uint16_t nSettings)
{
    appSettings = aSettings;
    numOfSettings = nSettings;
    
	#ifdef SERIAL_LOGGING
    String intialMsg = "Intializing WiFi module.";
	Serial.println(intialMsg);
	#endif

	WiFi.mode(WIFI_MODE_STA);
	delay(1000);
	WiFi.disconnect();
	WiFi.setAutoConnect(true);

	if (WiFi.status() == WL_NO_SHIELD)
	{
		#ifdef SERIAL_LOGGING
        String initialErr = "Communication with WiFi module failed!";
		Serial.println(initialErr);
		#endif
		// don't continue
		return false;
	}

	return true;
}

/// @brief 
/// @param retryAttempts number of reconnect attempts if failed
/// @param retryDelay in seconds
/// @return 
bool NetworkManager::connectWiFi(uint16_t retryAttempts, uint16_t retryDelay)
{
	#ifdef SERIAL_LOGGING
	String scanMsg = "Scanning for WiFi SSID.";
	Serial.println(scanMsg);
	#endif
	scanWiFiSettings();
    
	if (!wiFiConnection.Avialable)
	{
		#ifdef SERIAL_LOGGING
		char connectErr[48] = "";
		sprintf(connectErr, "No WiFi connections found for %s!", wiFiConnection.SSID);
		Serial.println(connectErr);
		#endif
		return false;
	}

	WiFi.setSleep(false);
	WiFi.begin(wiFiConnection.SSID, wiFiConnection.Password);
	#ifdef SERIAL_LOGGING
	char infoMsg[] = "Waiting for connection to WiFi";
	Serial.println(infoMsg);
	#endif

	uint8_t attempts = 0;
	uint8_t attemptMax = 3;
	while (WiFi.status() != WL_CONNECTED && attempts < attemptMax)
	{
		delay(1000);
        //display.print('.');
        //display.display();
		#ifdef SERIAL_LOGGING
		Serial.print('.');
		#endif
		++attempts;
	}
    //display.println();
    //display.display();
	#ifdef SERIAL_LOGGING
	Serial.println();
	#endif

	uint8_t retry = 0;
	while(WiFi.status() != WL_CONNECTED)
	{
		if (retry < retryAttempts)
		{
			delay(1000L*retryDelay);
			connectWiFi(0, retryDelay);
			++retry;
		}
		else
		{
			return false;
		}
		
	}

	return true;
}

bool NetworkManager::isConnected()
{
	return WiFi.isConnected();
}

void NetworkManager::scanWiFiSettings()
{
	int8_t numNetworks = WiFi.scanNetworks();
	#ifdef SERIAL_LOGGING
	Serial.println("Number of networks found: " + String(numNetworks));
	#endif

	if (numNetworks == 0)
	{
		delay(2500);
		numNetworks = WiFi.scanNetworks();
	}

	for (uint8_t i=0; i<numNetworks; i++)
	{
		String ssid = WiFi.SSID(i);
		#ifdef SERIAL_LOGGING
		Serial.println(ssid + " (" + String(WiFi.RSSI(i)) + ")");
		#endif
		for (uint8_t j=0; j < numOfSettings; j++)
		{
			const char* appSsid = appSettings[j].WifiSettings.SSID;
			#ifdef SERIAL_LOGGING
			Serial.println("Checking: " + String(appSsid));
			#endif
			if (strcasecmp(appSsid, ssid.c_str()) == 0)
			{
				#ifdef SERIAL_LOGGING
				Serial.println("Found: " + String(ssid));
				#endif
				appSettings[j].WifiSettings.Avialable = true;
				appSettings[j].WifiSettings.Strength = WiFi.RSSI(i);

				if (appSettings[j].WifiSettings.Strength > wiFiConnection.Strength)
				{
					wiFiConnection = appSettings[j].WifiSettings;
				}
			}
		}
	}

	#ifdef SERIAL_LOGGING
	Serial.println("Using WiFi " + String(appSettings.WifiSettings.SSID));
	#endif
	WiFi.disconnect();
}
