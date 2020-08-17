#define trigPin 10
#define echoPin 12
#define sumpPin 4
#define trigPin2 3
#define echoPin2 7
#define mncplPin 9
#define ovmax =  // Overhead tank maximum height minus 4 cms:
#define ovmin =   // Over head tank minimum height 10 cms inches plus bottom:
#define sumPmax =    // sump tank maximum height minus 4 cms:
#define sumPmin =    // sump tank bottom plus 10 cms
#define mncplSolPin 8 // solenoid for Muncipal Water
#define sumpSolPin 6 // Solenoid for Over head tank
float duration, ovdistance,sumdistance;

void setup() {
  // put your setup code here, to run once:
  Serial.begin (9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(sumpPin, OUTPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);
  pinMode(mncplPin, OUTPUT);
  pinMode(mncplSolPin, OUTPUT);
  pinMode(sumpSolPin, OUTPUT):
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(trigPin,LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  digitalWrite(trigPin2,LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin2, LOW);

  // Measure the response from the HC-SR04 ECHO Pin:

  duration = pulseIn(echoPin, HIGH);
  duration = pulseIn(echoPin2, HIGH);

  // Determine distace from duration.
  // Use 343 meters per second as speed of souns:
  ovdistance = (duration /2)* 0.0343;
  sumdistance = (duration /2)* 0.0343;
  // Send results to Serial Monitor:

  Serial.print("Distance in Over Head Tank = ");
  if (ovdistance >= 400 || ovdistance <=2){
    Serial.println("Out of range");
  }
  else {
    Serial.print(ovdistance);
    Serial.println( " cm");
    delay(500);

  }

  Serial.print("sumdistance = ");
  if (sumdistance >= 400 || sumdistance <=2){
    Serial.println("Out of range second sensor");    
  }
  else{
    Serial.print(sumdistance);
    Serial.println( " cm");
    delay(500);
  }
  switch (distance) {  // what is distance here it is not declared aboved
    case 1:
      //do something when var equals 1
      if  (ovdistance <= ovmin && sumdistance <= sumPmin){
        digitalWrite(mncplPin, HIGH); // drives motor drawing water from Muncipality:
        digitalWrite(mncplSolPin, HIGH); // activates muncipal solenoid:
      }
      else if(ovidistance <= ovmin && sumdistance >= summax){
        digitaWrite(sumpPin, HIGH); // drives motor to over head tank:
        digitalWrite(sumpSolPin, HIGH)// activates solenoid of overhead tank:
      }
      break;
    case 2:
      //do something when var equals 2

      break;
    default:
      // if nothing else matches, do the default
      // default is optional
      break;
  }
  if (distance <= 25){
    Serial.println(" level lowest ");
    digitalWrite(relayPin, HIGH);
  }
  else{
    Serial.print(distance1); // what is distance1 here it is not decalred anywhere above is it distance?
    digitalWrite(relayPin, LOW);
  }
  if (distance1 <= 25){
    Serial.println(" level lowest ");
    digitalWrite(relayPin2, HIGH);
  }

  else{
    Serial.print(distance1);
    digitalWrite(relayPin2, LOW);
  }
  delay(500);

}