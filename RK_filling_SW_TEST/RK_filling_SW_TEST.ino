#include <AUnit.h>

#include "common_RK_fillings.h"


test(setupFlowSensor) {
  assertEqual(setupFlowSensor(), HIGH);
}

test(intializeDigitalPumpPin) {
  assertEqual(intializeDigitalPumpPin(), MOTOROFF);
}

test(intializeTanks) {
  int tanksMode[] = {PIPEOFF, PIPEOFF, PIPEOFF};
  assertEqual(initializeTanks, tanksMode);
}

test(isMuniWaterTime) {
  assertEqual(isMuniWaterTime(), true);
}

test(analyzeTanks) {
  int tanksMode[] = {PIPEOFF, PIPEOFF, PIPEOFF};
  assertEqual(analyzeTanks(), tanksMode);

}

test(motor_off) {
  int pumpMode[] = {PIPEOFF, PIPEOFF, PIPEOFF};
  assertEqual(motor_off(), pumpMode);
}

test(motor_on) {
  int pumpMode[] = {PIPEON, PIPEON, PIPEON};
  assertEqual(motor_off(), pumpMode);
}

test(tanks) {
  for (int tank = 0; tank < NUM_OF_TANKS; tank++) {
    assertEqual(pipePins[tank], PIPEOFF);
    assertEqual(pipePins[tank], PIPEON);
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

  bool isWaterTime;
  isWaterTime = isMuniWaterTime();
  if (isWaterTime) {
    analyzeTanks();
  }
}
