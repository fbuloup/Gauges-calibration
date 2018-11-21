// Include used for watch dog timer
#include <avr/wdt.h>

// Defines for setting and clearing register bits
// Used to configure sample clock frequency
#ifndef cbi
  #define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
  #define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

// Loop frequency in Hz
const unsigned long loopFrequency = 500;

// Loop period in microsecond
const unsigned long loopPeriod = (int)(1000000.0/(1.0*loopFrequency));

// Percents of loop period in microsecond to compute workload
const unsigned long loopPeriod_05 = (int)(05*1000000.0/(100.0*loopFrequency));
const unsigned long loopPeriod_10 = (int)(10*1000000.0/(100.0*loopFrequency));
const unsigned long loopPeriod_15 = (int)(15*1000000.0/(100.0*loopFrequency));
const unsigned long loopPeriod_20 = (int)(20*1000000.0/(100.0*loopFrequency));
const unsigned long loopPeriod_25 = (int)(25*1000000.0/(100.0*loopFrequency));
const unsigned long loopPeriod_30 = (int)(30*1000000.0/(100.0*loopFrequency));
const unsigned long loopPeriod_35 = (int)(35*1000000.0/(100.0*loopFrequency));
const unsigned long loopPeriod_40 = (int)(40*1000000.0/(100.0*loopFrequency));
const unsigned long loopPeriod_45 = (int)(45*1000000.0/(100.0*loopFrequency));
const unsigned long loopPeriod_50 = (int)(50*1000000.0/(100.0*loopFrequency));
const unsigned long loopPeriod_55 = (int)(55*1000000.0/(100.0*loopFrequency));
const unsigned long loopPeriod_60 = (int)(60*1000000.0/(100.0*loopFrequency));
const unsigned long loopPeriod_65 = (int)(65*1000000.0/(100.0*loopFrequency));
const unsigned long loopPeriod_70 = (int)(70*1000000.0/(100.0*loopFrequency));
const unsigned long loopPeriod_75 = (int)(75*1000000.0/(100.0*loopFrequency));
const unsigned long loopPeriod_80 = (int)(80*1000000.0/(100.0*loopFrequency));
const unsigned long loopPeriod_85 = (int)(85*1000000.0/(100.0*loopFrequency));
const unsigned long loopPeriod_90 = (int)(90*1000000.0/(100.0*loopFrequency));
const unsigned long loopPeriod_95 = (int)(95*1000000.0/(100.0*loopFrequency));

// Loop workload
byte workload;

// Loop start time
unsigned long startTime;

// Variables to run process periodically
unsigned long currentLoopTime, previousLoopTime;

// Loop time in microsecond
unsigned long loopTime;

// Start realtime loop when true
bool startLoop = false;

// Stop realtime loop when true
bool terminateProcess = false;

const byte acquireAnalog0 = 10;
const byte acquireAnalog1 = 10;
const byte acquireAnalog2 = 10;
const byte acquireAnalog3 = 10;
const byte acquireAnalog4 = 10;
const byte acquireAnalog5 = 10;

byte acquireAnalog0Index = acquireAnalog0;
byte acquireAnalog1Index = acquireAnalog1;
byte acquireAnalog2Index = acquireAnalog2;
byte acquireAnalog3Index = acquireAnalog3;
byte acquireAnalog4Index = acquireAnalog4;
byte acquireAnalog5Index = acquireAnalog5;

unsigned long lastAcquireTime0;
unsigned long lastAcquireTime1;
unsigned long lastAcquireTime2;
unsigned long lastAcquireTime3;
unsigned long lastAcquireTime4;
unsigned long lastAcquireTime5;

unsigned int analogValue0;
unsigned int analogValue1;
unsigned int analogValue2;
unsigned int analogValue3;
unsigned int analogValue4;
unsigned int analogValue5;

//String serialMessage;
char serialMessage[64];

void setup() {
  // Set prescale to 32 for ADC to decrease conversion time
  // 16MHz/32 = 500kHz - Max Sample freq = 500kHz/13 # 38kHz
  // Convertion time # 30us
  sbi(ADCSRA,ADPS2) ;
  cbi(ADCSRA,ADPS1) ;
  sbi(ADCSRA,ADPS0) ;

  Serial.begin(2000000);
  while (!startLoop) {
     if(Serial.available()) {
       startLoop = ((char)Serial.read()) == 'r';
     }
   }

   startTime =  micros();
   previousLoopTime = startTime - loopPeriod;

   lastAcquireTime0 = 0;
   lastAcquireTime1 = 0;
   lastAcquireTime2 = 0;
   lastAcquireTime3 = 0;
   lastAcquireTime4 = 0;
   lastAcquireTime5 = 0;
}


void loop() {

   while(true) {
    
     currentLoopTime = micros();

     if(currentLoopTime - previousLoopTime >= loopPeriod) {
       loopTime = currentLoopTime - startTime;
       //delayMicroseconds(900);
       if(acquireAnalog0Index == acquireAnalog0) {
         acquireAnalog0Index = 0;
         analogValue0 = acquireAnalogInput(0, &lastAcquireTime0);
       }
       acquireAnalog0Index += 1;

       if(acquireAnalog1Index == acquireAnalog1) {
         acquireAnalog1Index = 0;
         analogValue1 = acquireAnalogInput(1, &lastAcquireTime1);
       }

       acquireAnalog1Index += 1;
       if(acquireAnalog2Index == acquireAnalog2) {
         acquireAnalog2Index = 0;
         analogValue2 = acquireAnalogInput(2, &lastAcquireTime2);
       }
       acquireAnalog2Index += 1;

       if(acquireAnalog3Index == acquireAnalog3) {
         acquireAnalog3Index = 0;
         analogValue3 = acquireAnalogInput(3, &lastAcquireTime3);
       }
       acquireAnalog3Index += 1;

       if(acquireAnalog4Index == acquireAnalog4) {
         acquireAnalog4Index = 0;
         analogValue4 = acquireAnalogInput(4, &lastAcquireTime4);
       }
       acquireAnalog4Index += 1;

       if(acquireAnalog5Index == acquireAnalog5) {
         acquireAnalog5Index = 0;
         analogValue5 = acquireAnalogInput(5, &lastAcquireTime5);
       }
       acquireAnalog5Index += 1;

       if(loopTime > 5000000) {
         terminateProcess = true;
       }

       previousLoopTime = micros() - currentLoopTime;
       workload = computeWorkload(previousLoopTime);
       Serial.println("-1:" + String(workload));
       previousLoopTime = currentLoopTime;
     }

     if(terminateProcess) {
       Serial.println('s');
       wdt_enable(WDTO_15MS);
       while(true) {
         // Wait for board to restart
       }
     }
     
   }
}

byte computeWorkload(unsigned long computeTime) {
   if(computeTime <= loopPeriod_05) return 05;
   if(computeTime <= loopPeriod_10) return 10;
   if(computeTime <= loopPeriod_15) return 15;
   if(computeTime <= loopPeriod_20) return 20;
   if(computeTime <= loopPeriod_25) return 25;
   if(computeTime <= loopPeriod_30) return 30;
   if(computeTime <= loopPeriod_35) return 35;
   if(computeTime <= loopPeriod_40) return 40;
   if(computeTime <= loopPeriod_45) return 45;
   if(computeTime <= loopPeriod_50) return 50;
   if(computeTime <= loopPeriod_55) return 55;
   if(computeTime <= loopPeriod_60) return 60;
   if(computeTime <= loopPeriod_65) return 65;
   if(computeTime <= loopPeriod_70) return 70;
   if(computeTime <= loopPeriod_75) return 75;
   if(computeTime <= loopPeriod_80) return 80;
   if(computeTime <= loopPeriod_85) return 85;
   if(computeTime <= loopPeriod_90) return 90;
   if(computeTime <= loopPeriod_95) return 95;
   if(computeTime <= loopPeriod) return 100;
   if(computeTime > loopPeriod) return 200;
}


unsigned int acquireAnalogInput(byte inputNumber, unsigned long *lastAcquisitionTime) {
  //unsigned long dt = micros();
  unsigned long value = analogRead(inputNumber);
  //dt = micros() - dt;
  //Serial.println(dt);
  sprintf(serialMessage, "%d:%d:%d", inputNumber, (int)(loopTime - *lastAcquisitionTime),(int) value);
  //serialMessage = String(inputNumber) + ":" + String(loopTime - *lastAcquisitionTime) + ":" + String(value);
  *lastAcquisitionTime = loopTime;
  Serial.println(serialMessage);
  return value;
}



