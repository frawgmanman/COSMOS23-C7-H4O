#include <SimpleEncoder.h>
#include <SoftwareSerial.h>
#include <TinyGPSPlus.h>

//motor
#define in1 14
#define in2 15
SimpleEncoder justin(0,34,35);
long positionJustin = -999;
static bool justinUp = false;
static bool justinDown = true;


//ethernet
const static byte eRx = 4;
const static byte eTx = 5;
SoftwareSerial eCom(eRx, eTx);
static bool startPacket = false; 
static bool endPacket = false; 


//gps
static const int RXPin = 12, TXPin = 13;
static const uint32_t GPSBaud = 9600;
TinyGPSPlus gps;
// The serial connection to the GPS device
SoftwareSerial ss(RXPin, TXPin);
static const int sendTimes = 10;
static bool timeSent = false;


//radio
const byte rxPin = 25;
const byte txPin = 33;
static bool tx = true;
SoftwareSerial radioCom (rxPin, txPin);


//winch commands
void moveUp(){
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
}
void moveDown(){
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
}
void stop(){
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
}

//dateAndTime 
void sendDateAndTime(){
  radioCom.print("S1@#");
  if(gps.date.isValid()){
    radioCom.print(gps.date.year());
    radioCom.print("-");
    radioCom.print(gps.date.month());
    radioCom.print("-");
    radioCom.print(gps.date.day());
    radioCom.print("-");

  }
  else{
    radioCom.print("0000-00-00-");
  }
  if(gps.time.isValid()){
    radioCom.print(gps.time.hour());
    radioCom.print("-");
    radioCom.print(gps.time.minute());
    radioCom.print("-");
    radioCom.print(gps.time.second());

  }
  else{
    radioCom.print("00-00-00");
  }
  radioCom.print("$");
  timeSent = true; 
}

//gps
double gpsLat(){
  if (gps.location.isValid()){
    return gps.location.lat(); 
  }
  else{
    return 999.99999999;
  }
}

double gpsLon(){
    if (gps.location.isValid()&&ss.available()>0){
        return gps.location.lng();
    }
    else if (!gps.location.isValid()){
        //Serial.println("gps L");
        return 999.99999999;
    }
    else if (ss.available()<=0){
        //Serial.println("serial L");
        return 888.88888888;
    }
    else{
        //Serial.println("you problem lmao");
        return 777.77777777;
    }
}

void gpsSetup(){
    Serial.begin(57600);
    ss.begin(GPSBaud);
}

//ethernet
String eRead(){
    if(eCom.available()){
        char data = (char)eCom.read();
        return data;
    }
    else{
    return "i love sophia"; 
    }
}

//radio
void radioSetup(){
    radioCom.begin(9600);
}
void sendRadio(){
    if(radioCom.available()){
    for(int i = 0; i< sendTime; i++){
        sendDateAndTime();
        delay(500);
        if(timeSent){
            radioCom.print("@lat#");
            radioCom.print(gpsLat(), 8);
            radioCom.print("$@lon#");
            radioCom.print(gpsLon(), 8);
            eCom.print('?');
            while(!startPacket){
                char handshake = eRead();
                if(handshake = '^'){
                    startPacket = true; 
                }
            }
            while(!endPacket){
                char data = eRead();
                radioCom.print(data);
                Serial.print(data); //debugging
                if(data = "%"){
                    endPacket = true;
                    eCom.print('!');
                }
            }
        radioCom.print("\n");
        Serial.print("\n");
         timeSent = false;
         startPacket = false; 
         endPacket = false; 
        }
    }
    }
    
}
void debug(){
    
}



void setup(){
Serial.begin(57600);
gpsSetup();
radioSetup();

}

void loop(){
sendRadio();
}