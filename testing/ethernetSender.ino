#include <SoftwareSerial.h>

const static byte eRx = 4;
const static byte eTx = 5;
SoftwareSerial eCom(eRx, eTx);



void setup(){
    Serial.begin(57600);
    eCom.begin(57600);
}

void loop(){
    eCom.println("i am clinically insane");
    Serial.println("i am clinically insane");
}