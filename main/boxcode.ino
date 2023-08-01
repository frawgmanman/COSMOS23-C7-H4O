#include <SoftwareSerial.h>
#include <TinyGPSPlus.h>

#include <SimpleEncoder.h>
#define in1 14 //HIGH CLOCKWISE
#define in2 15 //HIGH COUNTERCLOCKWISE
SimpleEncoder justin(0,34,35);

//wire
const static byte eRx = 4;
const static byte eTx = 5;
SoftwareSerial eCom(eRx, eTx);

//radio
const static byte rRx = 25;
const static byte rTx = 33;
SoftwareSerial radioCom(rRx, rTx);

//gps
static const int RXPin = 14, TXPin=15;
SoftwareSerial ss(RXPin, TXPin);
static const uint32_t GPSBaud = 9600;
TinyGPSPlus gps;


void moveDown(){
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    positionJust = justin.VALUE;
}
void moveUp(){
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    positionJust = justin.VALUE;
}
void stop(){
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
    positionJust = justin.VALUE;
}
void move(int rot){
    while(positionJustin<rot){
        moveDown();
    }
    stop();
    delay(1000);
}
String collectData(){
    String thing;
    if(eCom.available()){
        thing = eCom.readStringUntil('\n');
        Serial.println(thing);
        delay(800);
    }
    else{
        Serial.println('\n');
        delay(800);
    }
    return thing;
}
void setup(){
    Serial.begin(9600);
    eCom.begin(9600);
    ss.begin(GPSBaud);
    radioCom.begin(9600);
}

void loop(){
    move(20);
    for(int i = 0; i<10; i++){
        collectData();
    }
    delay(1000);
    move(40);
    for(int i = 0; i<10; i++){
        radioCom.print("S1");
        if (gps.date.isValid())
        {
            radioCom.print("@timestamp#");
            radioCom.print(gps.date.month());
            radioCom.print("-");
            radioCom.print(gps.date.day());
            radioCom.print("-");
            radioCom.print(gps.date.year());
        }
        if(gps.location.isValid()){
            radioCom.print("$@lat#");
            radioCom.print(gps.location.lat(),6);
            radioCom.print("$@lon#");
            radioCom.print(gps.location.lng(),6);
        }
        radioCom.print(collectData());
        radioCom.print('%');
    }
    delay(1000);
    move(60);
    for(int i = 0; i<10; i++){
        collectData();
    }
    delay(1000);
    while(justinPosition>0){
    moveUp();
    }
    delay(10000); //move to next waypoint
}