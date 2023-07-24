#include <TinyGPSPlus.h>
#include <SoftwareSerial.h>


//gps variables and setup
static const int RXPin = 4, TXPin = 5;
static const uint32_t GPSBaud = 9600;
TinyGPSPlus gps;
SoftwareSerial ss(RXPin,TXPin);
static const int sendTimes = 10;

//radio variables and setup
const byte rxPin = 25;
const byte txPin = 33;
SoftwareSerial radioCom(rxPin, txPin);

String gpsData(){
    char location[] = "@-011.1111111111111111@-011.1111111111111111";
    char lat[20];
    char lon[20];
    if (gps.location.isValid()){
        //latitude 
        dtostrf(abs(gps.location.lat()), 6, 2, lat);
        if (gps.location.lat() <= 99.99 && gps.location.lat() >= 0){
      location[3] = '0';
      location[1] = '+';
    }else{
      if (gps.location.lat() < 0){
        location[1] = '-';
        if (gps.location.lat() >= -99.99){
          location[3] = '0';
        }
      }
      
    }
    if (location[3] != '0'){
        location[3] = lat[0];
      }
    for (int i = 4; i < 21; i++){
      location[i] = lat[i-2];  
    } 
//longitude 
        dtostrf(abs(gps.location.lng()), 6, 2, lon);
        if (gps.location.lng() <= 99.99 && gps.location.lng() >= 0){
      location[24] = '0';
      location[22] = '+';
    }else{
      if (gps.location.lng() < 0){
        location[22] = '-';
        if (gps.location.lat() >= -99.99){
          location[24] = '0';
        }
      }
      
    }
    if (location[24] != '0'){
        location[24] = lat[0];
      }
    for (int i = 25; i < 42; i++){
      location[i] = lat[i-23];  
    } 
    }
    return location; 
}

void send(){
    for(int i = 0; i < sendTimes; i++){
        radioCom.print("S1");
        radioCom.print(gpsData());
        radioCom.print("%\n");
        delay(100);
    }
    delay(1000);
}

void setup(){
     Serial.begin(57600);
     radioCom.begin(56700);
     ss.begin(GPSBaud);
}

void loop(){
  Serial.println(gpsData());
  send(); 
  Serial.println(gps.location.lat());
}