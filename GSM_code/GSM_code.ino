#include <SoftwareSerial.h>
#include <ArduinoJson.h>
#include "pins.h"

SoftwareSerial GSM(recevier, transferer); // RX-pin7, TX-pin8
#include "helpers.h"
#include "requests.h"
#include "hcsr04.h"
#include "relay.h"

String APN_name = "airtelgprs.com"; // change the APN name as per the SIM card

int swi;
float depth;
const size_t capacity = JSON_OBJECT_SIZE(1) + 30;
DynamicJsonDocument doc(capacity);


void setup()
{
//  ----- SETTING UP SIM900A & I/O------------
  GSM.begin(9600);
  Serial.begin(9600);
  
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(relayPin, OUTPUT);
  char comand[50];
  String cmd = "AT+SAPBR=3,1,\"APN\",\"" + APN_name + "\"";
  cmd.toCharArray(comand , cmd.length());
  sendGSM(comand);  
  sendGSM("AT+SAPBR=1,1",3000);
  sendGSM("AT+HTTPINIT");  
  sendGSM("AT+HTTPPARA=\"CID\",1");
  Serial.println("Set-up Done");
}

void loop()
{ 
// ----- GETTING DEPTH FROM THE SENSOR -------------------
    depth = findDepth();
    Serial.println("Depth : "+String(depth));
    
//  ----- SENDING DATA TO SERVER VIA GET REQUEST ---------
//  ------ FOR GET METHOD ----

//    make_GET_request(depth,doc);
  
//  ------ FOR POST METHOD ----

    make_POST_request(depth,doc);

//  ------ CONVERTING TO JSON ----   
    deserializeJson(doc, response_arr);

//   ----- CHANGING RELAY STATE USING RESPONSE ---------
      if(doc["status_code"]!=200) {
          Serial.println("Some Error Occured");
//        handle_error();
      }
      else {
            swi = int(doc["switch"]);
            turn_bulb(swi);    
      }
    Serial.print("Level of motor : " + String(depth) + " and switch is " + String(digitalRead(relayPin)));
    Serial.println("\n\n\n");
    delay(5000);


}