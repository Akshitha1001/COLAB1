#include <SoftwareSerial.h>

SoftwareSerial GSM(7, 8); // RX, TX

#include "helpers.h"
#include "requests.h"




void setup()
{
  GSM.begin(9600);
  Serial.begin(9600);
  
  sendGSM("AT+SAPBR=3,1,\"APN\",\"airtelgprs.com\"");  
  sendGSM("AT+SAPBR=1,1",3000);
  sendGSM("AT+HTTPINIT");  
  sendGSM("AT+HTTPPARA=\"CID\",1");
}

void loop()
{ 
    
    DynamicJsonDocument re_POST = make_POST_request(9);
    Serial.println(String(int(re_POST["switch"])));
    serializeJson(re_POST, Serial);
    Serial.println("\n\n\n");
    
    DynamicJsonDocument re_GET = make_GET_request(9);
    

    Serial.println(String(int(re_GET["switch"])));
    serializeJson(re_GET, Serial);
    Serial.println("\n\n\n");
    
    
   
}
