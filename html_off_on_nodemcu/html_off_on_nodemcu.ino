
#include <ESP8266WiFi.h>
const char* ssid = "Kishore";
const char* password = "9440448974";

int ledpin=2;    // D4 in ModeMcu
WiFiServer server(80);

void setup() 
{
  Serial.begin(115200);
  delay(10);
  pinMode(ledpin,OUTPUT);
  digitalWrite(ledpin,LOW);
  //connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

 WiFi.begin (ssid,password);

  while (WiFi.status()!=WL_CONNECTED)
  {  delay(500);
     Serial.print(".");
  }


  Serial.println("");
  Serial.println("WiFi connected");
  // Start the server
  server.begin();
  Serial.println("Server started");

  //Print the IP Address

  Serial.print("Use this url to connect: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
  
}

void loop() 
  {
// Check if a client has connected
WiFiClient client = server.available();
if (!client)
  { 
    return;
  }
  //Wait until the client sends some data

  Serial.println("new client");
  while (!client.available())
  {
    delay(1);
  }

  // Read the first line of the request
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();
  //Match the request

  int value=LOW;
  if (request.indexOf(" /LED=ON")!=-1)
  {
    digitalWrite(ledpin,1);
    value=HIGH ; 
  }

  if (request.indexOf(" /LED=OFF")!=-1)
  {
    digitalWrite(ledpin,0);
    value=LOW ;
  }
  // Set ledpin according to the request
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("");// do not forget this one
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  client.println("<title>switch</title>");

  client.print("Led pin is now: ");

  if (value==HIGH)
  {
    client.print("OFF");

  }
  else
  {
    client.print("ON");
  }


  client.println("<br><br>");
  client.println("<a href=\"/LED=OFF\"\"><button>Turn <b>ON</b></button></a>");
  client.println("<a href=\"/LED=ON\"\"><button>Turn <b>OFF</b></button></a></br>");
  client.println("</html>");

  delay(1);

  Serial.println("Client disconnected");
  Serial.println("");
  }
