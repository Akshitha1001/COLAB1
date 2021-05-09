 #include "../common/common_RK_fillings.c"


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
  int isTesting = 1;
  delay(5000);
  calc_lph();
  printTimeStamp();

  autoSwitchOverheadTanks();

  analyzeTanks(isTesting);
}
