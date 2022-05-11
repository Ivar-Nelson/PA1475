
#include <Wire.h>
#include <BH1750FVI.h>
#include <RH_ASK.h>
#include <avr/sleep.h>
#include <avr/power.h>
#include <avr/wdt.h>

RH_ASK driver(2000, 3, 4, 0); // creation of RF driver
BH1750FVI lightMeter;

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
    lightMeter.begin();
}

void loop()
{
    driver.setModeTx();
    // unsigned short int lux = lightMeter.readLightLevel(); // read lux from sensor
    unsigned short int lux = 200;
    unsigned short int *msg; // msg to be transmitted
    msg = &lux;
    driver.send((uint8_t *)msg, 2);
    driver.waitPacketSent();
    delay(1000);
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
