#include "common_RK_fillings.h"


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
  delay(5000);
  calc_lph();
  printTimeStamp();

  bool isWaterTime;
  isWaterTime = isMuniWaterTime()
  if (isWaterTime) {
    analyzeTanks();
  }
}
