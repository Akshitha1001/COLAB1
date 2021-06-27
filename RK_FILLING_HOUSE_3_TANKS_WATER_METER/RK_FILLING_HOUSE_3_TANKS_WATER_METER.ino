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
  int now_hour = getNowHour();

  bool isWaterTime;
  isWaterTime = isMunicipalWaterTime(now_hour);
  if (isWaterTime) {
    analyzeTanks();
  }
}
