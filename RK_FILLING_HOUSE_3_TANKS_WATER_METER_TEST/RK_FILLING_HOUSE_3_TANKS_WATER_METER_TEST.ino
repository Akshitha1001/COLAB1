 
 #include "RTClib.h"

RTC_DS3231 rtc;
int pumpPin = 12;
//#define NORMALLY_OPEN
#ifdef  NORMALLY_OPEN
  #define PIPEOFF LOW
  #define PIPEON  HIGH
#else
  #define PIPEOFF HIGH
  #define PIPEON  LOW
#endif

#define MOTORON   LOW
#define MOTOROFF  HIGH

#define NUM_OF_TANKS     3
#define NUM_ACTIVE_TANKS 3
int      pipePins[NUM_OF_TANKS] = {9, 10, 11};
int floatSwitches[NUM_OF_TANKS] = {3,  4,  5};

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

volatile int  flow_frequency; // Measures flow sensor pulses
unsigned int  lph;         // Calculated litres/hour
unsigned char flowsensor = 2; // Sensor Input
unsigned long currentTime;
unsigned long cloopTime;
void flow () // Interrupt function
{
   flow_frequency++;
}

#define DEBUG
#ifndef DEBUG
#define      TANKS_FULL_WAIT_TIME 1000UL * 60 *  5 //  5 minutes
#define   MUNI_NO_WATER_WAIT_TIME 1000UL * 60 * 30 // 30 minutes
#define  MUNI_OFF_HOURS_WAIT_TIME 1000UL * 60 * 30 //  5 minutes
#define LPH_MOTOR_CHECK_WAIT_TIME 1000UL * 60 *  3 //  3 min liters per hour check time

#else
#define      TANKS_FULL_WAIT_TIME 1000UL * 60 * 1 // 1 minute
#define   MUNI_NO_WATER_WAIT_TIME 1000UL * 60 * 1 // 1 minute
#define  MUNI_OFF_HOURS_WAIT_TIME 1000UL * 60 * 1 // 1 minute
#define LPH_MOTOR_CHECK_WAIT_TIME 1000UL * 60 * 1 // 1 min liters per hour check time
#endif

#define LPH_MIN  10 // liters per hour minimum required

void reset_flowrate() {
  currentTime = millis();
  cloopTime   = currentTime;
  lph      = LPH_MIN;
}

void setup() {
  int tank;
//Serial.begin(57600);
   pinMode(     flowsensor, INPUT);
   digitalWrite(flowsensor, HIGH); // Optional Internal Pull-Up
   Serial.begin(9600);
   attachInterrupt(digitalPinToInterrupt(flowsensor), flow, RISING); // Setup Interrupt
   sei(); // Enable interrupts
   reset_flowrate();

  // initialize digital pump pin as an output.
   pinMode(     pumpPin, OUTPUT); // motor on/off relay
   digitalWrite(pumpPin, LOW);// MOTOR OFF  // before it was low
   delay(1000);

   for (tank = 0; tank < NUM_OF_TANKS; tank++) {
    pinMode(     pipePins[tank], OUTPUT);       // SOL-TANK
    digitalWrite(pipePins[tank], PIPEOFF);
    pinMode(floatSwitches[tank], INPUT_PULLUP); // over head tank sensor
   }
  
  #ifndef ESP8266
  while (!Serial); // wait for serial port to connect. Needed for native USB
#endif

  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    abort();
  }

  if (rtc.lostPower()) {
    Serial.println("RTC lost power, let's set the time!");
    
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));    
  }
}

int motor_state = MOTOROFF;
void motor_on(int tank) {
  int i;
  
  digitalWrite(pipePins[tank], PIPEON);
  delay(1000);
  for (i = 0; i < NUM_ACTIVE_TANKS; i++) {
    if (i == tank) {
      continue;
    }
    digitalWrite(pipePins[i], PIPEOFF);
  }
  delay(1000);
 
  /*
  if (motor_state == MOTORON) {
    //  motor is already on checking for water flow
    if (lph < LPH_MIN) {
      motor_off();
      delay(MUNI_NO_WATER_WAIT_TIME);
      Serial.print("Woke up from no muni water ");
    }
    return;
  }
  */
  // switching on motor 
  motor_state = MOTORON;
  reset_flowrate();
  digitalWrite(pumpPin, MOTORON);
  Serial.print("Time for filling up Tank ");
  return;
}

void motor_off() {
  int tank;
  motor_state = MOTOROFF;
  digitalWrite(pumpPin, MOTOROFF);
  delay(30000); // 30 Seconds

  for (tank = 0; tank < NUM_OF_TANKS; tank++) {
    digitalWrite(pipePins[tank], PIPEOFF);
  }

  Serial.println("TURNED MOTOR OFF"); 
}

void calc_lph() {
  currentTime = millis();
  // Every second, calculate and print litres/hour
  if (currentTime >= (cloopTime + LPH_MOTOR_CHECK_WAIT_TIME)) { 
    cloopTime      = currentTime; // Updates cloopTime
    // Pulse frequency (Hz) = 7.5Q, Q is flow rate in L/min.
    // (Pulse frequency x 60 min) / 7.5Q = flowrate in L/hour --- is for .5 inch sensor
    lph            = (flow_frequency * 60 / 7.5) / (LPH_MOTOR_CHECK_WAIT_TIME/1000);
    flow_frequency = 0;           // Reset Counter
    Serial.println(   lph, DEC);       // Print litres/hour
    Serial.println(" L/hour");
    return;
  }
  Serial.println(" in calc");
}

void loop() {
  calc_lph();
  DateTime now = rtc.now();

    Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.print(" (");
    Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
    Serial.print(") ");
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.println();


    int tank;
    for (tank = 0; tank < NUM_OF_TANKS; tank++) {
        motor_on(tank);
        Serial.print("Tank ");
        Serial.println(tank, DEC);
        delay(1000UL * 60);
    }
    // all tanks are full
    motor_off();
    delay(1000UL * 60);
}