#include <SoftwareSerial.h>

const static byte eRx = 4;
const static byte eTx = 5;
SoftwareSerial eCom(eRx, eTx);

void setup(){
    Serial.begin(9600);
    eCom.begin(9600);
}

void loop(){
  
    if(eCom.available()){
        String thing = eCom.readStringUntil('\n');
        Serial.println(thing);
        delay(800);
    }
    else{
        Serial.println('\n');
        delay(800);
    }
}
