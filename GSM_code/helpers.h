#include <SoftwareSerial.h>
#include "data_types.h"
#define MAX_LENGHT 75

//SoftwareSerial GSM(7, 8); // RX, TX

void parseATText(byte);
void add_to_response(String, String);
void reset_Arduino(void);

byte actionState = AS_IDLE;
unsigned long lastActionTime = 0;


byte parseState = PS_DETECT_MSG_TYPE;
char buffer[180];
byte pos = 0;
String response;
char response_arr[MAX_LENGHT];
int contentLength = 0;
String status_code;

void resetBuffer() {
  memset(buffer, 0, sizeof(buffer));
  pos = 0;
}



void sendGSM(String msg, int waitMs = 500) {
  GSM.println(msg);
  while(GSM.available()) {
    
    byte b = GSM.read();
    parseATText(b);
  }
  delay(waitMs);
}


void checkGSM() {
  lastActionTime = millis();
  while(actionState == AS_WAITING_FOR_RESPONSE) { 
    while(GSM.available()) {
      parseATText(GSM.read());
    }
    if(lastActionTime + 10000 <=millis()){
        actionState = AS_LOST_CONNECTION;
        return;
      }
  }
}


void parseATText(byte b) {
  if(test)
    Serial.print(char(b));
  buffer[pos++] = b;

  if ( pos >= sizeof(buffer) )
    resetBuffer(); // just to be safe

  switch (parseState) {
  case PS_DETECT_MSG_TYPE: 
    {
      if ( b == '\n' )
        resetBuffer();
      else {        
        if ( pos == 3 && strcmp(buffer, "AT+") == 0 ) {
          parseState = PS_IGNORING_COMMAND_ECHO;
        }
        else if ( b == ':' ) {

          if ( strcmp(buffer, "+HTTPACTION:") == 0 ) {
            parseState = PS_HTTPACTION_TYPE;
          }
          else if ( strcmp(buffer, "+HTTPREAD:") == 0 ) {          
            parseState = PS_HTTPREAD_LENGTH;
          }
          resetBuffer();
        }
      }
    }
    break;

  case PS_IGNORING_COMMAND_ECHO:
    {
//      Serial.println("PS_IGNORING_COMMAND_ECHO");
      if ( b == '\n' ) {
        parseState = PS_DETECT_MSG_TYPE;
        resetBuffer();
      }
    }
    break;

  case PS_HTTPACTION_TYPE:
    {
//      Serial.println("PS_HTTPACTION_TYPE");
      if ( b == ',' ) {
        parseState = PS_HTTPACTION_RESULT;
        status_code = "";
        resetBuffer();
      }
    }
    break;

  case PS_HTTPACTION_RESULT:
    {
//      Serial.println("PS_HTTPACTION_RESULT");
      if ( b == ',' ) {
        parseState = PS_HTTPACTION_LENGTH;
        resetBuffer();
      }
      else {
        status_code+=char(b);
      }
    }
    break;

  case PS_HTTPACTION_LENGTH:
    {
//      Serial.println("PS_HTTPACTION_LENGTH");
      if ( b == '\n' ) {
        // now request content
        

        GSM.print("AT+HTTPREAD=0,");
        
        GSM.println(buffer);
        
        parseState = PS_DETECT_MSG_TYPE;
        resetBuffer();
      }
    }
    break;

  case PS_HTTPREAD_LENGTH:
    {
//      Serial.println("PS_HTTPREAD_LENGTH");
      if ( b == '\n' ) {
        contentLength = atoi(buffer);
        parseState = PS_HTTPREAD_CONTENT;
        resetBuffer();
      }
    }
    break;

  case PS_HTTPREAD_CONTENT:
    {
//      Serial.println("PS_HTTPREAD_CONTENT");
      // for this demo I'm just showing the content bytes in the serial monitor
//      Serial.write(b);
      response += char(b);
      contentLength--;
      
      if ( contentLength <= 0 ) {

        // all content bytes have now been read
        add_to_response("status_code",status_code);
        parseState = PS_DETECT_MSG_TYPE;
        resetBuffer();
        response.toCharArray(response_arr, response.length());
        actionState = AS_RECEIVED_RESPONSE;
      }
    }
    break;
  }
}

void add_to_response(String key , String value) {
  response.trim();
  response.remove(response.length()-1,1);
  response = response + ",\"" + key + "\":" + value + "}";
  
}
