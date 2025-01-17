//gps lat 18 numbers add 180 15 decimal
//gps lon 18 numbers add 180 15 decimal
//temperature 5 2 decimal
//ph 4 2 decimal
//orp 3 digits add 500 
//tds 3 digits 
//dissolved oxygen 3 digits 1 decimal 
//photoresistor three digits
//dash n after everything
//gps
#include <TinyGPSPlus.h>
#include <SoftwareSerial.h>
//temperature
#include <OneWire.h>
#include <DallasTemperature.h>
//ORP
#include <EEPROM.h>
//TDS
#define TdsSensorPin 23
//ORP 
#define USE_PULSE_OUT

#ifdef USE_PULSE_OUT
  #include "orp_iso_grav.h"       
  Gravity_ORP_Isolated ORP = Gravity_ORP_Isolated(27);         
#else
  #include "orp_grav.h"
  Gravity_ORP ORP = Gravity_ORP(27);
#endif

//DO variables
#ifdef USE_PULSE_OUT
  #include "do_iso_grav.h"       
  Gravity_DO_Isolated DO = Gravity_DO_Isolated(26);         
#else
  #include "do_grav.h"
  Gravity_DO DO = Gravity_DO(26);
#endif

//gps variables
//switch rxp,txp??
static const int RXPin = 4, TXPin = 5;
static const uint32_t GPSBaud = 9600;

// The TinyGPSPlus object
TinyGPSPlus gps;
// The serial connection to the GPS device
SoftwareSerial ss(RXPin, TXPin);
static const int sendTimes = 10;

//temperature variables
const int oneWireBus = 17;
OneWire oneWire(oneWireBus);
DallasTemperature sensors(&oneWire);

//radio variables
const byte rxPin = 25;
const byte txPin = 33;
static bool tx = true;
SoftwareSerial radioCom (rxPin, txPin);


//ph variables
const int SensorPin = 15;
float buf[10];
int temp;

//tds variables
#define TdsSensorPin 34
//pls change sensor pin to somethign else
#define VREF 5.0      // analog reference voltage(Volt) of the ADC
#define SCOUNT  30           // sum of sample point
int analogBuffer[SCOUNT];    // store the analog value in the array, read from ADC
int analogBufferTemp[SCOUNT];
int analogBufferIndex = 0,copyIndex = 0;
float averageVoltage = 0,tdsValue = 0,temperature = 25;

static bool timeSent = false;
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

//time --> not using
void sendDateAndTime(){
  String dateAndTime = "S1@#";
  if(gps.date.isValid()){
    dateAndTime = dateAndTime + gps.date.year() + "-";
    dateAndTime = dateAndTime + gps.date.month() + "-";
    dateAndTime = dateAndTime + gps.date.day() + "-";
  }
  else{
    dateAndTime = dateAndTime + "0000-00-00-";
  }
  if(gps.time.isValid()){
    dateAndTime = dateAndTime + gps.time.hour() + "-";
    dateAndTime = dateAndTime + gps.time.minute() + "-";
    dateAndTime = dateAndTime + gps.time.second() + "-";
  }
  else{
    dateAndTime = dateAndTime + "00-00-00";
  }
  dateAndTime = dateAndTime + "$";
  timeSent = true; 
}


//temperature 

float getTemp(){
  sensors.requestTemperatures();
  float tempK = sensors.getTempCByIndex(0) +273.15;
  return tempK;
}

void tempSetup(){
  sensors.begin();
}

//ph

float getPh1() {
  for (int i = 0; i < 10; i++) { // Get 10 sample values from the sensor to smooth the value
    float analogVal = analogRead(SensorPin);
    buf[i] = -0.02165*(analogVal) + 16.91709;
    // delay(10);
  }
  for (int i = 0; i < 9; i++) { // Sort the analog values from small to large
    for (int j = i + 1; j < 10; j++) {
      if (buf[i] > buf[j]) {
        temp = buf[i];
        buf[i] = buf[j];
        buf[j] = temp;
      }
    }
  }
  float medianPH = buf[4];
  // delay(500);
  return medianPH;
}


//tds
void tdsSetup()
{
    //Serial.begin(115200);
    pinMode(TdsSensorPin,INPUT);
}

int getMedianNum(int bArray[], int iFilterLen) 
{
      int bTab[iFilterLen];
      for (byte i = 0; i<iFilterLen; i++)
      bTab[i] = bArray[i];
      int i, j, bTemp;
      for (j = 0; j < iFilterLen - 1; j++) 
      {
      for (i = 0; i < iFilterLen - j - 1; i++) 
          {
        if (bTab[i] > bTab[i + 1]) 
            {
        bTemp = bTab[i];
            bTab[i] = bTab[i + 1];
        bTab[i + 1] = bTemp;
         }
      }
      }
      if ((iFilterLen & 1) > 0)
    bTemp = bTab[(iFilterLen - 1) / 2];
      else
    bTemp = (bTab[iFilterLen / 2] + bTab[iFilterLen / 2 - 1]) / 2;
      return bTemp;
}

int getTDS()
{
   static unsigned long analogSampleTimepoint = millis();
   if(millis()-analogSampleTimepoint > 40U)     //every 40 milliseconds,read the analog value from the ADC
   {
     analogSampleTimepoint = millis();
     analogBuffer[analogBufferIndex] = analogRead(TdsSensorPin);    //read the analog value and store into the buffer
     analogBufferIndex++;
     if(analogBufferIndex == SCOUNT) 
         analogBufferIndex = 0;
   }   
   static unsigned long printTimepoint = millis();
   if(millis()-printTimepoint > 800U)
   {
      printTimepoint = millis();
      for(copyIndex=0;copyIndex<SCOUNT;copyIndex++)
        analogBufferTemp[copyIndex]= analogBuffer[copyIndex];
      averageVoltage = getMedianNum(analogBufferTemp,SCOUNT) * (float)VREF / 1024.0; // read the analog value more stable by the median filtering algorithm, and convert to voltage value
      float compensationCoefficient=1.0+0.02*(temperature-25.0);    //temperature compensation formula: fFinalResult(25^C) = fFinalResult(current)/(1.0+0.02*(fTP-25.0));
      float compensationVolatge=averageVoltage/compensationCoefficient;  //temperature compensation
      tdsValue=(133.42*compensationVolatge*compensationVolatge*compensationVolatge - 255.86*compensationVolatge*compensationVolatge + 857.39*compensationVolatge)*0.5; //convert voltage value to tds value
      //Serial.print("voltage:");
      //Serial.print(averageVoltage,2);
      //Serial.print("V   ");
      return tdsValue; 
   }
   
}

int getORP(){

  return ((int)ORP.read_orp());
 
}


float getDO(){
    return DO.read_do_percentage();
}

// void debug(){
//     for(int i = 0; i< sendTimes; i++){
//       String data = "S1@#";
//       if(gps.date.isValid()){
//         data = data+ gps.date.year() + "-";
//         data = data + gps.date.month() + "-";
//         data = data + gps.date.day() + "-";
//       }
//       else{
//         data = data + "0000-00-00-";
//       }
//       if(gps.time.isValid()){
//         data = data + gps.time.hour() + "-";
//         data = data + gps.time.minute() + "-";
//         data = data + gps.time.second() + "-";
//       }
//       else{
//         data = data + "00-00-00";
//       }
//       data = data + "$";

//       char lat[14];
//       dtostrf(gpsLat(),13,8,lat);
//       data = data + "@lat#" + lat;
//       char lon[14];
//       dtostrf(gpsLon(),13,8,lon);
//       data = data + "$@lon#" + lon;
//       data = data + "$@temp#" + getTemp();
//       data = data + "$@ph#" + getPh1();
//       data = data + "$@orp#" + getORP();
//       data = data + "$@do#" + getDO();
//       data = data + "$@tds#" + getTDS();
//       data = data + "$%";
//       data = data + "\n";
//       Serial.println(data);
//       delay(100); 
//     }
//     delay(1000);
// }


 void radioSetup(){
  radioCom.begin(9600);
}

String radioLoop(){
  if(radioCom.available()){
      String data = "S1@#";
      if(gps.date.isValid()){
        data = data + gps.date.year() + "-";
        data = data + gps.date.month() + "-";
        data = data + gps.date.day() + "-";
      }
      else{
      data = data + "0000-00-00-";
      }
      if(gps.time.isValid()){
        data = data + gps.time.hour() + "-";
        data = data + gps.time.minute() + "-";
        data = data + gps.time.second() + "-";
      }
      else{
        data = data + "00-00-00";
      }
      data = data + "$";

      char lat[14];
      dtostrf(gpsLat(),13,8,lat);
      data = data + "@lat#" + lat;
      char lon[14];
      dtostrf(gpsLon(),13,8,lon);
      data = data + "$@lon#" + lon;
      data = data + "$@temp#" + getTemp();
      data = data + "$@ph#" + getPh1();
      data = data + "$@orp#" + getORP();
      data = data + "$@do#" + getDO();
      data = data + "$@tds#" + getTDS();
      data = data + "$%";
      data = data + "\n";
      return data;
  } 
  return "";  
}

//main 

void setup() {
  Serial.begin(9600);
  gpsSetup();
  radioSetup();
  tempSetup();
}
//note: add sensor functions to radioLoop()! It will not work if you add it here! 
void loop() {
 
      String data = "S1@timestamp#";
      if(gps.date.isValid()){
        data = data + gps.date.year() + "-";
        data = data + gps.date.month() + "-";
        data = data + gps.date.day() + "-";
      }
      else{
      data = data + "0000-00-00-";
      }
      if(gps.time.isValid()){
        data = data + gps.time.hour() + "-";
        data = data + gps.time.minute() + "-";
        data = data + gps.time.second() + "-";
      }
      else{
        data = data + "00-00-00";
      }
      data = data + "$";

      char lat[14];
      dtostrf(gpsLat(),13,8,lat);
      data = data + "@lat#" + lat;
      char lon[14];
      dtostrf(gpsLon(),13,8,lon);
      data = data + "$@lon#" + lon;
      data = data + "$@temperature#" + getTemp();
      data = data + "$@ph#" + getPh1();
      data = data + "$@orp#" + getORP();
      data = data + "$@do#" + getDO();
      data = data + "$@tds#" + getTDS();
      data = data + "$%";
      data = data + "\n";
      radioCom.print(data);
      Serial.print("boop");
  //debug();
  delay(1000);
} 
