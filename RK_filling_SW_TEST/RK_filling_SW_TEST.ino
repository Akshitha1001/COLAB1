#include <AUnit.h>
#include "common_RK_fillings.h"


test(setupFlowSensor) {
  assertEqual(flowsensor, HIGH);
}

test(intializeDigitalPumpPin) {
  assertEqual(pumpPin, MOTOROFF);
}

test(intializeTanks) {
  for (int tank = 0; tank < NUM_OF_TANKS; tank++) {
    assertEqual(pipePins[tank], PIPEOFF);
  }
}

test(analyzeTanks) {
  if (!isMunicipalWaterTime()) {
    assertEqual(floatSwitches[0], PIPEOFF);
    assertEqual(floatSwitches[1], PIPEOFF);
    assertEqual(floatSwitches[2], PIPEOFF);
    return;
  }
  if (pipePins[0] == PIPEON) {
    assertEqual(pipePins[1], PIPEOFF);
    assertEqual(pipePins[2], PIPEOFF);
    return;
  }
  if (pipePins[1] == PIPEON) {
    assertEqual(pipePins[0], PIPEOFF);
    assertEqual(pipePins[2], PIPEOFF);
    return;
  }
  if (pipePins[2] == PIPEON) {
    assertEqual(pipePins[0], PIPEOFF);
    assertEqual(pipePins[1], PIPEOFF);
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
