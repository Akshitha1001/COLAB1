#include <AUnit.h>
#include "common_RK_fillings.h"


test(setupFlowSensor) {
  setupFlowSensor();
  assertEqual(digitalReadDebug( flowsensor), HIGH);
}

test(intializeDigitalPumpPin) {
  intializeDigitalPumpPin();
  assertEqual(digitalReadDebug( pumpPin), MOTOROFF);
}

test(intializeTanks) {
  initializeTanks();
  for (int tank = 0; tank < NUM_OF_TANKS; tank++) {
    assertEqual(digitalReadDebug( pipePins[tank]), PIPEOFF);
  }
}

test(analyzeTanks) {
  if (!isMunicipalWaterTime()) {
    assertEqual(digitalReadDebug( pumpPin), MOTOROFF); 
    
    for (int tank = 0; tank < NUM_OF_TANKS; tank++) {
       assertEqual(digitalReadDebug( pipePins[tank]), PIPEOFF);
    }
    return;
  }
  if (digitalReadDebug(floatSensors[0]) == LOW) {
    assertEqual(digitalReadDebug( pipePins[0]), PIPEON); 
    assertEqual(digitalReadDebug( pumpPin)    , MOTORON); 
    return;
  }
  if (digitalReadDebug(floatSensors[1]) == LOW) {
    assertEqual(digitalReadDebug( pipePins[1]), PIPEON); 
    assertEqual(digitalReadDebug( pumpPin)    , MOTORON); 
    return;
  }
  if (digitalReadDebug(floatSensors[2]) == LOW) {
    assertEqual(digitalReadDebug( pipePins[2]), PIPEON); 
    assertEqual(digitalReadDebug( pumpPin)    , MOTORON);  
    return;
  }
}

test(motor_on){
  if(digitalReadDebug(pumpPin) == MOTOROFF){
    if(digitalReadDebug(floatSensors[0]) == LOW){
      assertEqual(digitalReadDebug(pipePins[0]), PIPEON);
      assertEqual(digitalReadDebug(pumpPin),MOTORON);

      return;
    }
    if(digitalReadDebug(floatSensors[1]) == LOW){
      assertEqual(digitalReadDebug(pipePins[1]), PIPEON);
      assertEqual(digitalReadDebug(pumpPin),MOTORON);

      return;
    }
    if(digitalReadDebug(floatSensors[2]) == LOW){
      assertEqual(digitalReadDebug(pipePins[2]), PIPEON);
      assertEqual(digitalReadDebug(pumpPin),MOTORON);

      return;
    }
  }
  
}

test(motor_off){
  if(digitalReadDebug(floatSensors[0]) == LOW
     && digitalReadDebug(floatSensors[1]) == LOW
     && digitalReadDebug(floatSensors[2]) == LOW){
      
      assertEqual(digitalReadDebug(pumpPin),MOTOROFF);
      for (int tank = 0; tank < NUM_OF_TANKS; tank++) {
        assertEqual(digitalReadDebug(pipePins[tank]), PIPEOFF);
      }
      return; 
  }
}


void setup() {


  sei(); // Enable interrupts
  reset_flowrate();


  waitForSerialPortConnection();

  checkRTC();
  checkRTCPowerStatus();
}


void loop() {

  aunit::TestRunner::run();

  delay(5000);
  calc_lph();
  printTimeStamp();

  bool isWaterTime = isMunicipalWaterTime();
  if (isWaterTime) {
    analyzeTanks();
  }
}
