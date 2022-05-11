

/*

Example of BH1750 library usage.

This example initialises the BH1750 object using the default high resolution
continuous mode and then makes a light level reading every second.

Connections

  - VCC to 3V3 or 5V
  - GND to GND
  - SCL to SCL (A5 on Arduino Uno, Leonardo, etc or 21 on Mega and Due, on
    esp8266 free selectable)
  - SDA to SDA (A4 on Arduino Uno, Leonardo, etc or 20 on Mega and Due, on
    esp8266 free selectable)
  - ADD to (not connected) or GND

ADD pin is used to set sensor I2C address. If it has voltage greater or equal
to 0.7VCC voltage (e.g. you've connected it to VCC) the sensor address will be
0x5C. In other case (if ADD voltage less than 0.7 * VCC) the sensor address
will be 0x23 (by default).

*/

#include <BH1750.h>
#include <RH_ASK.h>
#include <SPI.h>
#include <Wire.h>
#include "driver/gpio.h"
//#define uS_TO_S_FACTOR 1000000
#define TIME_TO_SLEEP 5000000

BH1750 lightMeter;
RH_ASK driver(2000, 11, 25, 0);
RTC_DATA_ATTR unsigned short int old_lux = 0;
//int i=0;
//RTC_DATA_ATTR int bootCount = 0;

void setup() {
  Serial.begin(9600);
  if(!driver.init())
    Serial.println("init failed");
  pinMode(33, INPUT); //SDA
  pinMode(32, OUTPUT); //SCL
  pinMode(25, OUTPUT);
  // Initialize the I2C bus (BH1750 library doesn't do this automatically)
  Wire.begin(33,32, 25);
  // On esp8266 you can select SCL and SDA pins using Wire.begin(D4, D3);
  // For Wemos / Lolin D1 Mini Pro and the Ambient Light shield use
  // Wire.begin(D2, D1);
  
  lightMeter.begin();
  //Serial.println(F("BH1750 Test begin"));
}

void loop() {
  Serial.println("1");
  driver.setModeTx();
  //Serial.println("Sleep");
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP);
  //Serial.println("Wake");
  unsigned short int lux = lightMeter.readLightLevel();
  unsigned short int *msg;
  //char test = 'f';
   // Serial.println(lux-old_lux );
   //delay(2000);
  if(lux - old_lux > 50){
    old_lux = lux;/*
    Serial.print("Old lux: ");
    Serial.println(old_lux );*/
    msg = &lux;
  if(!driver.send((uint8_t*)msg, 2))
    Serial.println("send failed");
  driver.waitPacketSent();/*
    i++;
    Serial.print("Number of times sent: ");
    Serial.println(i);
    Serial.print(lux);
    Serial.println("\n");*/
  }
  
  else if (lux-old_lux < -50) {
    //Serial.println(lux-old_lux );
    old_lux = lux;
    //Serial.print("Old lux: ");
    //Serial.println(old_lux );
    msg = &lux;
  if(!driver.send((uint8_t*)msg, 2))
    Serial.println("send failed");
  driver.waitPacketSent();
  //i++;
  //Serial.print("Number of times sent: ");
  //Serial.println(i);
    }
  //++bootCount;
  //Serial.println("Boot number: " + String(bootCount));
  Serial.println("2");
  esp_deep_sleep_start();
}
  
