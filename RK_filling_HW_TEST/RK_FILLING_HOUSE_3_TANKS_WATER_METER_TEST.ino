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
  delay(30000);
  //calc_lph();
  int now_hour = getNowHour();

  for (int tank = 0; tank < NUM_OF_TANKS; tank++) {
    motor_on(tank);
    delay(30000);
  }
  motor_off();
}
