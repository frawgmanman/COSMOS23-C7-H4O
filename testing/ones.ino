#include <SoftwareSerial.h>

const byte rxPin = 25;
const byte txPin = 33;
SoftwareSerial radioCom(rxPin, txPin);
const static int one 111111111111111111;

void setup(){
    Serial.begin(57600);
    radioCom.begin(57600);
}

void loop(){
    radioCom.print(one);
    radioCom.print("\n");
    delay(500); 
}