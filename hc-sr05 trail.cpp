#define trigPin 10
#define echoPin 12
#define sumpPin 4
#define trigPin2 3
#define echoPin2 7
#define mncplPin 9
#define mncplSolPin 8 // solenoid for Muncipal Water
#define sumpSolPin 6 // Solenoid for Over head tank

int ovmax = 304; // Overhead tank maximum height minus 4 cms:
int ovmin = 10; // Over head tank minimum height 10 cms  plus bottom:
int sumPmax = 245; // sump tank maximum height minus 4 cms:
int sumPmin = 10;  // sump tank bottom plus 10 cms

float duration, ovdistance, sumdistance;

void setup() {
    // put your setup code here, to run once:
    Serial.begin(9600);
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);
    pinMode(sumpPin, OUTPUT);
    pinMode(trigPin2, OUTPUT);
    pinMode(echoPin2, INPUT);
    pinMode(mncplPin, OUTPUT);
    pinMode(mncplSolPin, OUTPUT);
    pinMode(sumpSolPin, OUTPUT);
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
    sumdistance =(duration /2)* 0.0343;
    // Send results to Serial Monitor:

    Serial.print("Distance in Over Head Tank = ");
    if (ovdistance >= 400 || ovdistance <=2)
        Serial.println("Out of range)");
    else{
        Serial.print(ovdistance);
        Serial.println( " cm");
        delay(500);
    }

    Serial.print("sumPdistance = ");
    if (sumdistance >= 400 || sumdistance <=2)
        Serial.println("Out of range second sensor)");    
    else
    {
        Serial.print(sumdistance);
        Serial.println( " cm");
        delay(500);
    }
    if (ovdistance <= ovmin && sumdistance <= sumPmin)
    {
        digitalWrite(mncplPin, HIGH); // Motor On  drawing water from Muncipality:
        digitalWrite(mncplSolPin, HIGH); // activates muncipal solenoid:
    }
    else if (ovdistance <= ovmin && sumdistance >= sumPmax) // if over head tank is minimal and sump tank is full:
    {
    digitalWrite(mncplSolPin, LOW); // Muncipal Solenoid gets inactivated:
    digitalWrite(sumpSolPin,HIGH);// over head tank solenoid gets activated:
    }

    else if (ovdistance >= ovmax && sumdistance >= sumPmax) // if over head tank is full and sump tank is full)
    {
        digitalWrite(sumpPin, LOW); // Stops motor to over head tank:
        digitalWrite(sumpSolPin, LOW);// inactivate solenoid of overhead tank:
    }
    // if (ovdistance <= 10){
    //    Serial.println(" level lowest ");
    //   digitalWrite(relayPin, HIGH);
    //}
    // else{
    // Serial.print(distance1);
    // digitalWrite(relayPin, LOW);
    //}
    // if (distance1 <= 25){
    //    Serial.println(" level lowest ");
    //   digitalWrite(relayPin2, HIGH);
    //}

    //else{
    //Serial.print(distance1);
    //digitalWrite(relayPin2, LOW);
    //}
    delay(500);
}