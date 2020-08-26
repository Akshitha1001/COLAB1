#include "pins.c"

float duration, distance,distance1;

void setup() {
  // put your setup code here, to run once:
  Serial.begin (9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(relayPin, OUTPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);
  pinMode(relayPin2, OUTPUT);
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
  distance = (duration /2)* 0.0343;
  distance1 =(duration /2)* 0.0343;
  // Send results to Serial Monitor:

  Serial.print("Distance = ");
  if (distance >= 400 || distance <=2)
      Serial.println("Out of range)");
  Serial.print("Distance1 = ");
  if (distance1 >= 400 || distance1 <=2){
      Serial.println("Out of range second sensor)");    
  }
  else{
    Serial.print(distance);
    Serial.println( " cm");
    delay(500);
    Serial.print(distance1);
    Serial.println( " cm");
    delay(500);
  }
  if (distance <= 25){
      Serial.println(" level lowest ");
      digitalWrite(relayPin, HIGH);
  }
  else{
    Serial.print(distance1);
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
  delay(600);

}
