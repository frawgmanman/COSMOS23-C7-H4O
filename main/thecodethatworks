
#include <TinyGPSPlus.h>
#include <SoftwareSerial.h>
#include <OneWire.h>
#include <DallasTemperature.h>
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

//ph variables
const int SensorPin = 15;
float buf[10];
int temp;

//temperature variables
const int oneWireBus = 17;
OneWire oneWire(oneWireBus);
DallasTemperature sensors(&oneWire);

/*
   This sample sketch demonstrates the normal use of a TinyGPSPlus (TinyGPSPlus) object.
   It requires the use of SoftwareSerial, and assumes that you have a
   4800-baud serial GPS device hooked up on pins 4(rx) and 3(tx).
*/
static const int RXPin = 4, TXPin = 5;
static const uint32_t GPSBaud = 9600;

// The TinyGPSPlus object
TinyGPSPlus gps;

// The serial connection to the GPS device
SoftwareSerial ss(RXPin, TXPin);

float getTemp(){
  sensors.requestTemperatures();
  float tempK = sensors.getTempCByIndex(0) +273.15;
  return tempK;
}
int getORP(){

  return ((int)ORP.read_orp());
 
}
float getDO(){
    return DO.read_do_percentage();
}

void tempSetup(){
  sensors.begin();
}

SoftwareSerial radioCom(25,33);
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

void setup()
{
  Serial.begin(115200);
  radioCom.begin(9600);
  ss.begin(GPSBaud);
  tempSetup();

  Serial.println(F("DeviceExample.ino"));
  Serial.println(F("A simple demonstration of TinyGPSPlus with an attached GPS module"));
  Serial.print(F("Testing TinyGPSPlus library v. ")); Serial.println(TinyGPSPlus::libraryVersion());
  Serial.println(F("by Mikal Hart"));
  Serial.println();
}

void loop()
{
  // This sketch displays information every time a new sentence is correctly encoded.
  while (ss.available() > 0)
    if (gps.encode(ss.read()))
      displayInfo();

  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    Serial.println(F("No GPS detected: check wiring."));
    while(true);
  }
}

void displayInfo()
{
   if (gps.date.isValid())
  {
    radioCom.print("S1@timestamp#");
    radioCom.print(gps.date.month());
    radioCom.print("-");
    Serial.print(F("/"));
    radioCom.print(gps.date.day());
    radioCom.print("-");
    Serial.print(F("/"));
    radioCom.print(gps.date.year());
  }
  else
  {
    Serial.print(F("INVALID"));
  }
  Serial.print(F("Location: ")); 
  if (gps.location.isValid())
  {
    radioCom.print("$@lat#");
    radioCom.print(gps.location.lat(), 6);
    Serial.print(F(","));
    radioCom.print("$@lon#");
    radioCom.print(gps.location.lng(), 6);
    radioCom.print("$@ph#");
    radioCom.print(getPh1());
    radioCom.print("$@temp#");
    radioCom.print(getTemp());
    radioCom.print("$@do#");
    radioCom.print(getDO());
    radioCom.print("$@orp#");
    radioCom.print(getORP());
    radioCom.print("$%\n");
  }
  else
  {
    Serial.print(F("INVALID"));
  }

  Serial.print(F("  Date/Time: "));
  if (gps.date.isValid())
  {
    Serial.print(gps.date.month());
    Serial.print(F("/"));
    Serial.print(gps.date.day());
    Serial.print(F("/"));
    Serial.print(gps.date.year());
  }
  else
  {
    Serial.print(F("INVALID"));
  }

  Serial.print(F(" "));
  if (gps.time.isValid())
  {
    if (gps.time.hour() < 10) Serial.print(F("0"));
    Serial.print(gps.time.hour());
    Serial.print(F(":"));
    if (gps.time.minute() < 10) Serial.print(F("0"));
    Serial.print(gps.time.minute());
    Serial.print(F(":"));
    if (gps.time.second() < 10) Serial.print(F("0"));
    Serial.print(gps.time.second());
    Serial.print(F("."));
    if (gps.time.centisecond() < 10) Serial.print(F("0"));
    Serial.print(gps.time.centisecond());
  }
  else
  {
    Serial.print(F("INVALID"));
  }

  Serial.println();
}
