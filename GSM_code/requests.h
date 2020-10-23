#include "convert_json.h"


void GET_request(int);
void POST_request(int);

DynamicJsonDocument make_GET_request(int depth) {

  while(actionState != AS_IDLE);
  GET_request(depth);
  while(actionState != AS_RECEIVED_RESPONSE);
  
  if(actionState == AS_RECEIVED_RESPONSE) {
    DynamicJsonDocument json = JSONIFY(response);
    actionState = AS_IDLE;
    return json;
  }
}

DynamicJsonDocument make_POST_request(int depth) {
  
  while(actionState != AS_IDLE);
  POST_request(depth);
  while(actionState != AS_RECEIVED_RESPONSE);
  if(actionState == AS_RECEIVED_RESPONSE) {
    DynamicJsonDocument json = JSONIFY(response);
    
    actionState = AS_IDLE;
    return json;
  }
}
  

void GET_request(int depth) {
  if(actionState != AS_IDLE)
    return; 
  Serial.println("GET request:  ");
  actionState = AS_WAITING_FOR_RESPONSE;
  response = "";
  String para = "AT+HTTPPARA=\"URL\"";
  String url = "\"http://jayashankar.pythonanywhere.com/abcdef/depth?d=";
  
  String comand = para + ","+url + String(depth) + "\"";
  sendGSM(comand);
  sendGSM("AT+HTTPACTION=0");
  checkGSM();
  
  

}




void POST_request(int depth) {
  
  if(actionState != AS_IDLE)
    return; 
  Serial.println("POST request:  ");
  actionState = AS_WAITING_FOR_RESPONSE;
  response = "";
  String para = "AT+HTTPPARA=\"URL\"";
  String url = "\"http://jayashankar.pythonanywhere.com/abcdef/depth";
  String body = "{ \"depth\" :" +String(depth)+ "}";
  
  String comand = para + ","+url;
  
  sendGSM(comand);
  sendGSM("AT+HTTPPARA=\"CONTENT\",\"application/json\"");
  sendGSM("AT+HTTPDATA=" + String(body.length()) + ",100000");
  sendGSM(body);

  sendGSM("AT+HTTPACTION=1");
  checkGSM();
}
