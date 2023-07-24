#include <Servo.h>
#include <SoftwareSerial.h>
#include <TinyGPSPlus.h>

//ethernet
const static byte eRx = 4;
const static byte eTx = 5;
SoftwareSerial ethernetCom(eRx, eTx);
//gps

//radio

//servo



//ethernet
String ethernetRead(){
    if(ethernetCom.available()){
        char data = (char)ethernetCom.read();
        return data;
    }
    else{
    return "check wiring"; 
    }
}


void debug(){
    
}



void setup(){
Serial.begin(57600);
}

void loop(){

}