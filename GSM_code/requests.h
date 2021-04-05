

void GET_request(float);
void POST_request(float);



void make_GET_request(float depth,JsonDocument& doc) {

  if(actionState != AS_IDLE) return;
  
  GET_request(depth);
  if(actionState != AS_RECEIVED_RESPONSE) return;
  
  if(actionState == AS_RECEIVED_RESPONSE) {
    actionState = AS_IDLE;
    return;
  }
  
}

void make_POST_request(float depth,JsonDocument& doc) {
  
  if(actionState != AS_IDLE) return;
  
  POST_request(depth);
  if(actionState != AS_RECEIVED_RESPONSE) return;
  
  if(actionState == AS_RECEIVED_RESPONSE) {
    actionState = AS_IDLE;
    return;
  }
}
  

void GET_request(float depth) {
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




void POST_request(float depth) {
  
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
