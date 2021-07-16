#include <SoftwareSerial.h>
#include <ArduinoJson.h>
#include "pins.h"

SoftwareSerial GSM(recevier, transferer); // RX-pin7, TX-pin8
  
#include "helpers.h"    // GSM cmds parser
#include "requests.h"   // Functions for GET & POST
#include "hcsr04.h"     // HCSR04 sensor code
#include "relay.h"      // Relay Switch code

String APN_name = "bsnlnet";                            // change the APN name as per the SIM card

bool test=true, alt = true;
int swi = -1,time_unit=1000;                            //units of time to delay
float depth;
unsigned int wait = 2;
const size_t capacity = JSON_OBJECT_SIZE(1) + 30;
DynamicJsonDocument doc(capacity);                      // JSON datatype to store response from server

void changeFilter(String key);
StaticJsonDocument<200> filter;

void setup()
{
//  ----- SETTING UP SIM900A & I/O------------
  GSM.begin(9600);
  Serial.begin(9600);
//  
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(relayPin, OUTPUT);
  digitalWrite(resetPin,HIGH);
  response = "";
  char comand[50];
  String cmd = "AT+SAPBR=3,1,\"APN\",\"" + APN_name + "\"\"";   // Setup APN name of GSM code
  cmd.toCharArray(comand , cmd.length());
  sendGSM(comand);  
  sendGSM("AT+SAPBR=1,1",3000);
  sendGSM("AT+HTTPINIT");                                     // Turn on mobile data
  sendGSM("AT+HTTPPARA=\"CID\",1");
  actionState = AS_IDLE;                                      // actionState is user-defined datatype enum
  Serial.println("Set-up Done");
}

void loop()
{ 
// ----- GETTING DEPTH FROM THE SENSOR -------------------
    if(test){
      if(alt){
        depth = 10;
        alt = false;
      }
      else{
        depth = 2;
        alt = true;
      }
    }
    else
      depth = findDepth(); 

    Serial.println("Depth : "+String(depth));
    
//  ----- SENDING DATA TO SERVER VIA GET REQUEST ---------
//  ------ FOR GET METHOD ----

    make_GET_request(depth,doc);
  
//  ------ FOR POST METHOD ----

//     make_POST_request(depth,doc);


//   ----- CHECKING ACTION STATE ---------
     if(actionState == AS_LOST_CONNECTION){
      Serial.println("Some Error Occured , Resetting");
      if ( wait >= 32768 )
        wait = wait;
      else
        wait = wait+wait;
      delay(wait*time_unit);
      setup();
    }
    wait = 5;

//  ------ CONVERTING TO JSON ----   

    changeFilter("status_code");
    DeserializationError error = deserializeJson(doc, response,DeserializationOption::Filter(filter));
    if (error) {
      Serial.print(F("deserializeJson() failed: "));
      Serial.println(error.c_str());
      return;
    }

    serializeJsonPretty(doc, Serial);
    Serial.println();
    if(doc["status_code"]==200  ) {
      changeFilter("s");
      error = deserializeJson(doc, response,DeserializationOption::Filter(filter));
      if (error) {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.c_str());
        return;
      }
      serializeJsonPretty(doc, Serial);
      Serial.println();
      swi = int(doc["s"]);
      turn_bulb(swi);
      changeFilter("a");
      error = deserializeJson(doc, response,DeserializationOption::Filter(filter));
      if (error) {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.c_str());
        return;
      }
      serializeJsonPretty(doc, Serial);
        
    }
    else{
      delay(wait*time_unit);
      return;
    }
      
    Serial.print("Level of motor : " + String(depth) + " and switch is " + String(digitalRead(relayPin)));
    Serial.println("\n\n\n");
    
    delay(wait*time_unit);


}

void changeFilter(String key){
  filter.clear();
  filter[key] = true;
}
 
