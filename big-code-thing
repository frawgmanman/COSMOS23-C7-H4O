//gps
#include <TinyGPSPlus.h>
#include <SoftwareSerial.h>
#include <string>
using namespace std;

//gps variables
//switch rxp,txp??
static const int RXPin = 4, TXPin = 5;
static const uint32_t GPSBaud = 9600;
// The TinyGPSPlus object
TinyGPSPlus gps;
// The serial connection to the GPS device
SoftwareSerial ss(RXPin, TXPin);

String gpsDisplayInfo(){
  char locAndTime[] = "@lat#+nnn.nn$@lon#-nnn.nn$@mon#nn$@day#nn$@year#nnnn$@hour#nn$@min#nn$@sec#nn$@csec#nn$";
  if (gps.location.isValid()){
  //latitude
    char lat[8];
    dtostrf(abs(gps.location.lat()),6,2,lat);
    if (gps.location.lat() <= 99.99 && gps.location.lat() >= 0){
      locAndTime[6] = '0';
      locAndTime[5] = '+';
    }else{
      if (gps.location.lat() < 0){
        locAndTime[5] = '-';
        if (gps.location.lat() >= -99.99){
          locAndTime[6] = '0';
        }
      }
      
    }
    if (locAndTime[6] != '0'){
        locAndTime[6] = lat[0];
      }
    for (int i = 7; i < 12; i++){
      locAndTime[i] = lat[i-6];  
    } 
// longitude
    char lng[8];
    dtostrf(abs(gps.location.lng()),6,2,lng);
    if (gps.location.lng() <= 99.99 && gps.location.lng() >= 0){
      locAndTime[19] = '0';
      locAndTime[18] = '+';
    }else{
      if (gps.location.lng() < 0){
        locAndTime[18] = '-';
        if (gps.location.lng() >= -99.99){
          locAndTime[19] = '0';
        }
      }
    }
    if (locAndTime[19] != '0'){
        locAndTime[19] = lng[0];
      }
    for (int i = 20; i < 25; i++){
      locAndTime[i] = lng[i-19];  
    } 
  } 

  if (gps.date.isValid()){
    char mon[3];
    itoa(gps.date.month(), mon, 10);
    //31 32
    if (gps.date.month() < 10){
      locAndTime[31] = '0';
      locAndTime[32] = mon[0];
    } else{
      locAndTime[31] = mon[0];
      locAndTime[32] = mon[1];
    }

    char day[3];
    itoa(gps.date.day(), day, 10);
    //31 32
    if (gps.date.day() < 10){
      locAndTime[39] = '0';
      locAndTime[40] = day[0];
    } else{
      locAndTime[39] = day[0];
      locAndTime[40] = day[1];
    }
    
    char year[5];
    itoa(gps.date.year(), year, 10);
    for (int i = 48; i < 52; i++){
      locAndTime[i] = year[i-48];
    }
  }

  if (gps.time.isValid()){
    char hour[3];
    itoa(gps.time.hour(), hour, 10);
    if (gps.time.hour() < 10){
      locAndTime[59] = '0';
      locAndTime[60] = hour[0];
    } else{
      locAndTime[59] = hour[0];
      locAndTime[60] = hour[1];
    }
    //67 68, 75 76, 83 84
    char min[3];
    itoa(gps.time.minute(), min, 10);
    if (gps.time.minute() < 10){
      locAndTime[67] = '0';
      locAndTime[68] = min[0];
    } else{
      locAndTime[67] = min[0];
      locAndTime[68] = min[1];
    }
    char sec[3];
    itoa(gps.time.second(), sec, 10);
    if (gps.time.second() < 10){
      locAndTime[75] = '0';
      locAndTime[76] = sec[0];
    } else{
      locAndTime[75] = sec[0];
      locAndTime[76] = sec[1];
    }
    char csec[3];
    itoa(gps.time.centisecond(), csec, 10);
    if (gps.time.centisecond() < 10){
      locAndTime[84] = '0';
      locAndTime[85] = csec[0];
    } else{
      locAndTime[84] = csec[0];
      locAndTime[85] = csec[1];
    }
  }
  Serial.println(locAndTime);
  return locAndTime;
}

String gpsLoop(){
  while (ss.available() > 0){
    if (gps.encode(ss.read())){
      gpsDisplayInfo();
    } 
  }
      

  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    return ("No GPS detected: check wiring.");
    while(true);
  }
}

void gpsSetup(){
    Serial.begin(115200);
    ss.begin(GPSBaud);
}


void setup() {
  gpsSetup();
}

void loop() {
  gpsLoop();

}
