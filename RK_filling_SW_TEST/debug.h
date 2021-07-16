
int digi_val[32];
void digitalWrite(int pin, int value) {
    digi_val[pin] = value;
}
int digitalReadDebug(int pin) {
    return digi_val[pin];
}
