#include <AUnit.h>
#include "common_RK_fillings.h"


test(setupFlowSensor) {
  assertEqual(digitalRead(flowsensor), HIGH);
}

test(intializeDigitalPumpPin) {
  assertEqual(digitalRead(pumpPin), MOTOROFF);
}

test(intializeTanks) {
  for (int tank = 0; tank < NUM_OF_TANKS; tank++) {
    assertEqual(digitalRead(pipePins[tank]), PIPEOFF);
  }
}

test(analyzeTanks) {
  if (!isMunicipalWaterTime()) {
    assertEqual(digitalRead(pumpPin),MOTOROFF); 
    
    for (int tank = 0; tank < NUM_OF_TANKS; tank++) {
       assertEqual(digitalRead(pipePins[tank]), PIPEOFF);
    }
    return;
  }
  if (digitalRead(floatSwitches[0]) == LOW) {
    assertEqual(digitalRead(pipePins[0]),PIPEON); 
    assertEqual(digitalRead(pumpPin),MOTORON); 
    return;
  }
  if (digitalRead(floatSwitches[1]) == LOW) {
    assertEqual(digitalRead(pipePins[1]),PIPEON); 
    assertEqual(digitalRead(pumpPin),MOTORON); 
    return;
  }
  if (digitalRead(floatSwitches[2]) == LOW) {
    assertEqual(digitalRead(pipePins[2]),PIPEON); 
    assertEqual(digitalRead(pumpPin),MOTORON);  
    return;
  }
}

test(motor_on){
  if(digitalRead(pumpPin) == MOTOROFF){
    if(digitalRead(floatSwitches[0]) == LOW){
      assertEqual(digitalRead(pipePins[0]), PIPEON);
      assertEqual(digitalRead(pumpPin),MOTORON);

      return;
    }
    if(digitalRead(floatSwitches[1]) == LOW){
      assertEqual(digitalRead(pipePins[1]), PIPEON);
      assertEqual(digitalRead(pumpPin),MOTORON);

      return;
    }
    if(digitalRead(floatSwitches[2]) == LOW){
      assertEqual(digitalRead(pipePins[2]), PIPEON);
      assertEqual(digitalRead(pumpPin),MOTORON);

      return;
    }
  }
  
}

test(motor_off){
  if(digitalRead(floatSwitches[0]) == LOW
     && digitalRead(floatSwitches[1]) == LOW
     && digitalRead(floatSwitches[2]) == LOW){
      
      assertEqual(digitalRead(pumpPin),MOTOROFF);
      for (int tank = 0; tank < NUM_OF_TANKS; tank++) {
        assertEqual(digitalRead(pipePins[tank]), PIPEOFF);
      }
      return; 
  }
}


void setup() {

  setupFlowSensor();

  sei(); // Enable interrupts
  reset_flowrate();

  intializeDigitalPumpPin();
  initializeTanks();

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
