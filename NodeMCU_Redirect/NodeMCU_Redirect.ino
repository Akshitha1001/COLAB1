
#include<ESP8266WiFi.h>

#include <vector> 
using namespace std; 




/*  HTTPS on ESP8266 with follow redirects, chunked encoding support
 *  Version 3.0
 *  Author: Sujay Phadke
 *  Github: @electronicsguy
 *  Copyright (C) 2018 Sujay Phadke <electronicsguy123@gmail.com>
 *  All rights reserved.
 *
 *  Example Arduino program
 */
#include "HTTPSRedirect.h"
#include "DebugMacros.h"

// Fill ssid and password with your network credentials
const char* ssid = "Prasad";
const char* password = "7396967424";

const char* host = "script.google.com";
// Replace with your own script id to make server side changes
const char *GScriptId = "AKfycbzUJXzhatJXeGD4FGF9CCQUXAtB8olonB7bij1U";




const int httpsPort = 443;

// echo | openssl s_client -connect script.google.com:443 |& openssl x509 -fingerprint -noout
const char* fingerprint = "1E C8 2D 9C F6 52 6B 13 FA 04 A2 F3 5A B8 B0 DB 27 67 AA C8";
//const uint8_t fingerprint[20] = {};

// Write to Google Spreadsheet
int depth = 2;
String url = String("/macros/s/") + GScriptId + "/exec";


String payload_base =  "{\"command\": \"appendRow\", \
                    \"sheet_name\": \"Sheet1\", \
                    \"depth\": ";
String payload = "";

HTTPSRedirect* client = nullptr;

int ledpin=2;    // D2 in ModeMcu

// used to store the values of free stack and heap
// before the HTTPSRedirect object is instantiated
// so that they can be written to Google sheets
// upon instantiation
unsigned int free_heap_before = 0;
unsigned int free_stack_before = 0;



void setup() {
  Serial.begin(115200);
  Serial.flush();

  pinMode(ledpin,OUTPUT);
  digitalWrite(ledpin,LOW);
  
  free_heap_before = ESP.getFreeHeap();
  free_stack_before = ESP.getFreeContStack();
  Serial.printf("Free heap: %u\n", free_heap_before);
  Serial.printf("Free stack: %u\n", free_stack_before);
  
  Serial.println();
  Serial.print("Connecting to wifi: ");
  Serial.println(ssid);
  // flush() is needed to print the above (connecting...) message reliably, 
  // in case the wireless connection doesn't go through
  Serial.flush();

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

}

void loop() {
  static int error_count = 0;
  static int connect_count = 0;
  const unsigned int MAX_CONNECT = 20;
  static bool flag = false;
  //Serial.printf("Free heap: %u\n", ESP.getFreeHeap());
  //Serial.printf("Free stack: %u\n", ESP.getFreeContStack());
  
  if (!flag){
    free_heap_before = ESP.getFreeHeap();
    free_stack_before = ESP.getFreeContStack();
    client = new HTTPSRedirect(httpsPort);
    client->setInsecure();
    flag1 = true;
    client->setPrintResponseBody(true);
    client->setContentTypeHeader("application/json");
    for (int i=0; i<5; i++){
      int retval = client->connect(host, httpsPort);
      if (retval == 1) {
         flag1 = true;
         break;
      }
      else
        Serial.println("Connection failed. Retrying...");
      }

      if (!flag1){
        Serial.print("Could not connect to server: ");
        Serial.println(host);
        Serial.println("Exiting...");
        return;
      }

 
      if (client->setFingerprint(fingerprint)) {
        Serial.println("Certificate match.");
      } else {
        Serial.println("Certificate mis-match");
      }
    }
   
//  client->GET(url, host);
//  String response = client->getResponseBody();
//  depth=depth+2;
//  Serial.println(depth);
//  int state = response[1]-'0';
//  if(state ==1){
//    Serial.println("turn ON");
//    digitalWrite(ledpin,LOW);
//  }
//   else{
//    Serial.println("turn OFF");
//    digitalWrite(ledpin,HIGH);
//   }

  depth=depth+2;
  payload = payload_base + "\"" + depth + "\"}";
  client->POST(url, host, payload, false);
  String response = client->getResponseBody();
  int state = repsonse[1]-'0';
  if(state ==1){
    Serial.println("turn ON");
    digitalWrite(ledpin,LOW);
  }
  else{
    Serial.println("turn OFF");
    digitalWrite(ledpin,HIGH);
  }
  
  
   


 
  if (connect_count > MAX_CONNECT){
    
    //error_count = 5;
    connect_count = 0;
    flag = false;
    delete client;
    return;
  }



  /*
  if (!(client.reConnectFinalEndpoint())){
    ++error_count;
    DPRINT("Error-count while connecting: ");
    DPRINTLN(error_count);
  }
  else
    error_countunt = 0;
  */
  
  if (error_count > 3){
    Serial.println("Halting processor..."); 
    delete client;
    client = nullptr;
    Serial.printf("Final free heap: %u\n", ESP.getFreeHeap());
    Serial.printf("Final stack: %u\n", ESP.getFreeContStack());
    Serial.flush();
    ESP.deepSleep(0);
  }
  
  // In my testing on a ESP-01, a delay of less than 1500 resulted 
  // in a crash and reboot after about 50 loop runs.
  delay(10000);
                          
}
