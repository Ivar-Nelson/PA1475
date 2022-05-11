
// HAS NOT BEEN TESTED
// Experimental code, not tested on new ATtiny85 microcontroller.
// Based on code used with ESP32. 
// NOTE: In later versions of the code, if-statement on line 48
// may be removed entirely, for power saving reasons.
//#include "driver/gpio.h"
//#include <configat.h>
//#include <BH1750FVI.h>
#include <Wire.h>
#include <BH1750FVI.h>
#include <RH_ASK.h>
#include <avr/sleep.h>
#include <avr/power.h>
#include <avr/wdt.h>
//#define TIME_TO_SLEEP 5000000
//BH1750 lightMeter; // creation of light-sensor device
//configmaster WIRE;
// RTC_DATA_ATTR unsigned short int old_lux = 0; 

RH_ASK driver(2000, 3, 4, 0); // creation of RF driver
//configat WIRE; // create reciever
BH1750FVI WIRE;

const byte txPin = 4;
const byte sdaPin = 0;
const byte sclPin = 2; // init of pins
byte saveADCSRA;
volatile byte counterWD = 0;

void setup()
{
    driver.init();
    //resetWatchDog();
    pinMode(sdaPin, INPUT);  // SDA for sensor
    pinMode(sclPin, OUTPUT); // SCL for sensor
    pinMode(txPin, OUTPUT); // Transmitter
    WIRE.begin();
    //lightMeter.begin(); // init of light senso    lightMeter.powerOn();
}

void loop()
{
    driver.setModeTx();
    // esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP);
    unsigned short int lux = WIRE.readLightLevel();//lightMeter.readLightLevel(); // read lux from sensor
    //unsigned short int lux = 200;
    unsigned short int *msg; // msg to be transmitted
    msg = &lux;
    driver.send((uint8_t *)msg, 2);
    driver.waitPacketSent();
    // esp_deep_sleep_start();
    //sleepNow(); // enter deep sleep
}

/*
void sleepNow(){
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    // save ADCSRA = ADCSRA;
    ADCSRA = 0;
    power_all_disable();

    noInterrupts();
    resetWatchDog();
    sleep_enable();
    interrupts();

    sleep_cpu();

    sleep_disable();
    power_all_enable();

    // ADCSRA = saveADCSRA;
}

// Function to reset ATtiny85's watchdog timer used during deep sleep. 
void resetWatchDog(){
    MCUSR = 0;
    WDTCR = bit (WDCE) | bit (WDE) | bit (WDIF);
    WDTCR = bit (WDIE) | bit (WDP2) | bit (WDP1);

    wdt_reset();
}
*/
