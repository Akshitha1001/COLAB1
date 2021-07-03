#include <SoftwareSerial.h>
#include <ArduinoJson.h>
#include "pins.h"

SoftwareSerial GSM(recevier, transferer); // RX-pin7, TX-pin8
#include "helpers.h"    // GSM cmds parser
#include "requests.h"   // Functions for GET & POST
#include "hcsr04.h"     // HCSR04 sensor code
#include "relay.h"      // Relay Switch code

String APN_name = "airtelgprs.com";               // change the APN name as per the SIM card

bool test=true;
int swi,time_unit=1000;                           //units of time to delay
float depth;
long long int wait = 5;
const size_t capacity = JSON_OBJECT_SIZE(1) + 30;
DynamicJsonDocument doc(capacity);                // JSON datatype to store response from server


void setup()
{
//  ----- SETTING UP SIM900A & I/O------------
  GSM.begin(9600);
  Serial.begin(9600);
  
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(relayPin, OUTPUT);
  digitalWrite(resetPin,HIGH);
  char comand[50];
  String cmd = "AT+SAPBR=3,1,\"APN\",\"" + APN_name + "\"";   // Setup APN name of GSM code
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
    if(test)
      depth = 10;
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
      try
        wait = wait*wait;
      catch{}
      setup();
    }
    wait = 5;
//  ------ CONVERTING TO JSON ----   
    deserializeJson(doc, response_arr);
  
    if(doc["status_code"]==200  ) {
      swi = int(doc["switch"]);
      turn_bulb(swi);
        
    }
      
    Serial.print("Level of motor : " + String(depth) + " and switch is " + String(digitalRead(relayPin)));
    Serial.println("\n\n\n");
    delay(wait*time_unit);


}
