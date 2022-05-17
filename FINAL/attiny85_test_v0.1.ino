
#include <TinyWireM.h>
#include <BH1750.h>
#include <RH_ASK.h>
#include <avr/sleep.h>
#include <avr/wdt.h>

RH_ASK driver(2000, 3, 4, 0); // creation of RF driver
BH1750 lightMeter;

const byte txPin = 4;
byte saveADCSRA;
volatile byte counterWD = 0;

void setup()
{
    driver.init();
    pinMode(txPin, OUTPUT); // Transmitter
    delay(50);
    lightMeter.begin();
}

void loop()
{
    driver.setModeTx();
    uint16_t lux = lightMeter.getLightIntensity();
    unsigned short int *msg; // msg to be transmitted
    msg = &lux;
    driver.send((uint8_t *)msg, 2);
    driver.waitPacketSent();

    for (int i = 0; i < 1; i++){
      WatchdogEnable (0b100000);  // 4 seconds
    }
}

ISR(WDT_vect) 
  {
  wdt_disable();  
  }

void WatchdogEnable(const byte interval) 
  {
  wdt_reset();
    
  MCUSR = 0;                          // reset various flags
  WDTCR |= 0b00011000;               // see docs, set WDCE, WDE
  WDTCR =  0b01000000 | interval;    // set WDIE, and appropriate delay
  
  ADCSRA &= ~_BV(ADEN);

  set_sleep_mode (SLEEP_MODE_PWR_DOWN); 
  sleep_bod_disable();
  sei();
  sleep_mode();     
  
  
  ADCSRA |= _BV(ADEN);
  
  } 

// sleep bit patterns:
//  1 second:  0b000110
//  2 seconds: 0b000111
//  4 seconds: 0b100000
//  8 seconds: 0b100001
