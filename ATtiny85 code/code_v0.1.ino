// HAS NOT BEEN TESTED
// Experimental code, not tested on new ATtiny85 microcontroller.
// Based on code used with ESP32. 

// NOTE: In later versions of the code, if-statement on line 48
// may be removed entirely, for power saving reasons.

#include <BH1750.h>
#include <RH_ASK.h>
#include <SPI.h>
#include <Wire.h>
#include "driver/gpio.h"
#include <avr/sleep.h>
#include <avr/power.h>
#include <avr/wdt.h>
#define TIME_TO_SLEEP 5000000

BH1750 lightMeter; // creation of light-sensor device
RH_ASK driver(2000, 11, 25, 0); // creation of RF driver
RTC_DATA_ATTR unsigned short int old_lux = 0; 

const byte txPin = 3;
const byte sdaPin = 5;
const byte sclPin = 7; // init of pins
byte saveADCSRA;
volatile byte counterWD = 0;

void setup()
{
    Serial.begin(4800);
    if (!driver.init()) // init of RF driver
        Serial.println("init failed");
    resetWatchDog();
    pinMode(5, sdaPin);  // SDA for sensor
    pinMode(7, sclPin); // SCL for sensor
    pinMode(3, txPin); // Transmitter
    Wire.begin(33, 32, 25);

    lightMeter.begin(); // init of light sensor
}

void loop()
{
    driver.setModeTx();
    // esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP);
    unsigned short int lux = lightMeter.readLightLevel(); // read lux from sensor
    unsigned short int *msg; // msg to be transmitted
    if (lux / old_lux > 1.03 || lux / old_lux < 0.97 && lux > 50)
    {
        old_lux = lux;
        msg = &lux; // message points to lux value
        if (!driver.send((uint8_t *)msg, 2)) // sending package
            Serial.println("send failed");
        driver.waitPacketSent();
    }
    // esp_deep_sleep_start();
    sleepNow(); // enter deep sleep
}

// Sleep function to conserve power during downtime. 
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
