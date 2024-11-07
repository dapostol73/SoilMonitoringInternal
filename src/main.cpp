
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
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

/* Uncomment the initialize the I2C address , uncomment only one, If you get a totally blank screen try the other*/
#define i2c_Address 0x3c //initialize with the I2C addr 0x3C Typically eBay OLED's
//#define i2c_Address 0x3d //initialize with the I2C addr 0x3D Typically Adafruit OLED's

#define SCREEN_WIDTH 64 // OLED display width, in pixels
#define SCREEN_HEIGHT 128 // OLED display height, in pixels
#define OLED_RESET -1   //   QT-PY / XIAO
Adafruit_SH1107 display = Adafruit_SH1107(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


#define BUILTIN_LED PC13
#define MS_SENSOR0 PA0

void setup()
{
    Serial.begin(115200);
    // initialize digital pin PB2 as an output.
    pinMode(BUILTIN_LED, OUTPUT);
    pinMode(MS_SENSOR0, INPUT);

    delay(250); // wait for the OLED to power up
    display.begin(i2c_Address, true); // Address 0x3C default
    //display.setContrast (0); // dim display

    display.display();
    delay(1000);
    display.invertDisplay(true);
    delay(1000);
    display.invertDisplay(false);
    delay(1000);
    display.clearDisplay();
    display.display();
    
    display.setRotation(1);
    display.setTextSize(1);
    display.setTextColor(SH110X_WHITE);
    display.setCursor(0, 0);
    display.println("Setup complete!");
    display.display();

    Serial.println("Setup complete!");
}

void loop()
{

    digitalWrite(BUILTIN_LED, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(100);               // wait for 100mS
    digitalWrite(BUILTIN_LED, LOW);    // turn the LED off by making the voltage LOW
    delay(100);               // wait for 100mS

    int mois = analogRead(MS_SENSOR0);

    Serial.print(mois);

    display.clearDisplay();
    display.display();
    display.setCursor(0, 0);

    if(mois >= 1000)
    {
        display.println("Sensor is not in the Soil or DISCONNECTED");
    }
    else if (mois >= 600)
    {
        display.println("Soil is DRY");
    }
    else if(mois >= 370)
    {
        display.println("Soil is HUMID"); 
    }
    else
    {
        display.println("Sensor in WATER");
    }

    display.println(mois);
    display.display();
    delay(2000);
}