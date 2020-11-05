

void turn_bulb(int s) {
  if(s==1) {
    digitalWrite(relayPin,LOW);
  }
  else if(s==0) {
    digitalWrite(relayPin,HIGH);
  }
}
