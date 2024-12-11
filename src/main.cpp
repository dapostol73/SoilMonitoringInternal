
/*********************************************************************
  This is an example for our Monochrome OLEDs based on SH110X drivers

  This example is for a 128x64 size display using I2C to communicate
  3 pins are required to interface (2 I2C and one reset)

  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada  for Adafruit Industries.
  BSD license, check license.txt for more information
  All text above, and the splash screen must be included in any redistribution

  i2c SH1106 modified by Rupert Hirst  12/09/21
*********************************************************************/

#include <Arduino.h>
#include <Wire.h>
#include <WiFi.h>

#include "ApplicationSettings.h"
#include "displayMain.h"

#define SERIAL_LOGGING
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
DisplayMain displayMain;

/* Uncomment the initialize the I2C address , uncomment only one, If you get a totally blank screen try the other*/
#define i2c_Address 0x3c //initialize with the I2C addr 0x3C Typically eBay OLED's
//#define i2c_Address 0x3d //initialize with the I2C addr 0x3D Typically Adafruit OLED's

void configureWiFi();

void blinkLed(int times, int interval);

void plotLinear(char *label, int x, int y);

void setup()
{
    Serial.begin(115200);
    // initialize digital pin PB2 as an output.
    pinMode(BUILTIN_LED, OUTPUT);
    pinMode(MS_SENSOR0, INPUT);

    delay(250); // wait for the OLED to power up
    displayMain.init();
	configureWiFi();
    Serial.println("Setup complete!");
	blinkLed(3, 250);
	
	uint8_t d = 64;
	plotLinear("S0", 0 * d, 64);
	plotLinear("S1", 1 * d, 64);
	plotLinear("S2", 2 * d, 64);
	plotLinear("S3", 3 * d, 64);
	plotLinear("S4", 4 * d, 64);
	plotLinear("S5", 5 * d, 64);
}

void loop()
{
    int mois = analogRead(MS_SENSOR0);

	displayMain.DisplayGFX->fillRect(0, 0, 480, 48, BLACK);
	displayMain.printLineReset();
	char info[48] = "";
    if(mois >= 4000)
    {
		strcmp(info, "Sensor is not in the Soil or DISCONNECTED");
    }
    else if (mois >= 2400)
    {
        strcmp(info, "Soil is DRY");
    }
    else if(mois >= 1500)
    {
        strcmp(info, "Soil is HUMID"); 
    }
    else if(mois >= 50)
    {
        strcmp(info, "Sensor in WATER");
    }
    else
    {
        strcmp(info, "Sensor in DISCONNECTED?");
    }

	displayMain.printLine(info);
	Serial.println(info);
	
	sprintf(info, "Value: %d", mois);
	displayMain.printLine(info);
	Serial.println(info);

    delay(2000);
}

void blinkLed(int times, int interval)
{
	for (uint8_t i = 0; i < times; i++)
	{
		digitalWrite(BUILTIN_LED, HIGH);   // turn the LED on (HIGH is the voltage level)
		delay(interval);               // wait for 100mS
		digitalWrite(BUILTIN_LED, LOW);    // turn the LED off by making the voltage LOW
		delay(interval);               // wait for 100mS
	}
	
}

void printConnectInfo()
{
	// get your MAC address
	byte mac[6];
	WiFi.macAddress(mac);
	IPAddress ip = WiFi.localIP();
	
	// SSID
	char ssidInfo[34] = "";
	sprintf(ssidInfo, "WiFi SSID: %s", WiFi.SSID());

	// MAC address
	char macInfo[34] = "";
	sprintf(macInfo, "MAC address: %02X:%02X:%02X:%02X:%02X:%02X", mac[5], mac[4], mac[3], mac[2], mac[1], mac[0]);

	// IP address
	char ipInfo[34] = "";
	sprintf(ipInfo, "IP address: %u.%u.%u.%u", ip[0], ip[1], ip[2], ip[3]);

    displayMain.clearDisplay();
    displayMain.printLine(ssidInfo);
    displayMain.printLine(macInfo);
    displayMain.printLine(ipInfo);

	#ifdef SERIAL_LOGGING
	Serial.println(ssidInfo);
	Serial.println(ipInfo);	
	Serial.println(macInfo);
	#endif
}

void resolveAppSettings()
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
		for (uint8_t j=0; j < AppSettingsCount; j++)
		{
			const char* appSsid = AppSettings[j].WifiSettings.SSID;
			#ifdef SERIAL_LOGGING
			Serial.println("Checking: " + String(appSsid));
			#endif
			if (strcasecmp(appSsid, ssid.c_str()) == 0)
			{
				#ifdef SERIAL_LOGGING
				Serial.println("Found: " + String(ssid));
				#endif
				AppSettings[j].WifiSettings.Avialable = true;
				AppSettings[j].WifiSettings.Strength = WiFi.RSSI(i);

				if (AppSettings[j].WifiSettings.Strength > appSettings.WifiSettings.Strength)
				{
					appSettings = AppSettings[j];
				}
			}
		}
	}

	#ifdef SERIAL_LOGGING
	Serial.println("Using WiFi " + String(appSettings.WifiSettings.SSID));
	#endif
	WiFi.disconnect();	
}

bool connectToWiFi(uint8_t retries)
{
	String scanMsg = "Scanning for WiFi SSID.";
    displayMain.clearDisplay();
    displayMain.printLine(scanMsg);
	#ifdef SERIAL_LOGGING
	Serial.println(scanMsg);
	#endif
	resolveAppSettings();

    char infoMsg[] = "Waiting for connection to WiFi";
	if (!appSettings.WifiSettings.Avialable)
	{
		char connectErr[48] = "";
		sprintf(connectErr, "No WiFi connections found for %s!", appSettings.WifiSettings.SSID);
		#ifdef SERIAL_LOGGING
		Serial.println(connectErr);
		#endif
		displayMain.fillScreen(ERROR_COLOR);
		displayMain.drawString(connectErr, 400, 240, TEXT_CENTER_MIDDLE, BLACK, ERROR_COLOR);
		return false;
	}

	WiFi.setSleep(false);
	WiFi.begin(appSettings.WifiSettings.SSID, appSettings.WifiSettings.Password);
    displayMain.printLine(infoMsg);
	#ifdef SERIAL_LOGGING
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
		if (retry < retries)
		{
			connectToWiFi(0);
			++retry;
		}
		else
		{
			return false;
		}
		
	}

	return true;
}

void configureWiFi()
{
	String intialMsg = "Intializing WiFi module.";
    displayMain.clearDisplay();
    displayMain.printLine(intialMsg);
	#ifdef SERIAL_LOGGING
	Serial.println(intialMsg);
	#endif

	displayMain.printLine("Setting mode...");
	//WiFi.begin("a12studios", "apollo1973");
	WiFi.mode(WIFI_MODE_STA);
	delay(5000);
	displayMain.printLine("Disconnecting...");
	WiFi.disconnect();
	//WiFi.endAP(true);
	
	//WiFi.setAutoConnect(true);
	//WiFi.setPersistent(false);

	if (WiFi.status() == WL_NO_SHIELD)
	{
		String initialErr = "Communication with WiFi module failed!";
        displayMain.printLine(initialErr);
		#ifdef SERIAL_LOGGING
		Serial.println(initialErr);
		#endif
		// don't continue
		while (true)
			;
	}

	if (!connectToWiFi(2))
	{
		// don't continue
		while (true)
			;
	}
		
	printConnectInfo();
	delay(2000);
}


// #########################################################################
//  Draw a linear meter on the screen
// #########################################################################
void plotLinear(char *label, int x, int y)
{
    int w = 60;
    displayMain.DisplayGFX->drawRect(x+1, y, w, 256, GRAY);
    displayMain.DisplayGFX->fillRect(x+3, y + 24, w-3, 256 - 48, WHITE);

    displayMain.drawString(label, x + 32, y + 12, TEXT_CENTER_MIDDLE, CYAN);

    for (int i = 0; i < 11; i ++)
    {
        displayMain.DisplayGFX->drawFastHLine(x + 32, y + 38 + (i*18), 16, BLACK);
    }

    for (int i = 0; i < 3; i ++)
    {
        displayMain.DisplayGFX->drawFastHLine(x + 32, y + 38 + (i*90), 24, BLACK);
    }

    displayMain.DisplayGFX->fillTriangle(x + 6, y + 218 + 8, x + 6 + 20, y + 218, x + 6, y + 218 - 8, RED);

	displayMain.drawString("---", x + 32, y + 256 - 12, TEXT_CENTER_MIDDLE, CYAN);
}