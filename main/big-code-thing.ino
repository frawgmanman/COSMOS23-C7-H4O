//gps
#include <TinyGPSPlus.h>
#include <SoftwareSerial.h>
//temperature
#include <OneWire.h>
#include <DallasTemperature.h>
//TDS
#define TdsSensorPin 23 

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
#define TdsSensorPin A0
//pls change sensor pin to somethign else
#define VREF 5.0      // analog reference voltage(Volt) of the ADC
#define SCOUNT  30           // sum of sample point
int analogBuffer[SCOUNT];    // store the analog value in the array, read from ADC
int analogBufferTemp[SCOUNT];
int analogBufferIndex = 0,copyIndex = 0;
float averageVoltage = 0,tdsValue = 0,temperature = 25;


//gps

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
  return locAndTime;
}

String gpsLoop(){
  while (ss.available() > 0){
    if (gps.encode(ss.read())){
      return gpsDisplayInfo();
    } 
  }
      

  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    return gpsDisplayInfo();
    while(true);
  }
}

void gpsSetup(){
    Serial.begin(57600);
    ss.begin(GPSBaud);
}


//radio

 void radioSetup(){
  radioCom.begin(57600);
}

void radioLoop(){
    // if (tx){
    for(int i = 0; i < sendTimes; i++){
    radioCom.print("S1");
    radioCom.print(gpsLoop());
    radioCom.print(tempLoop());
    radioCom.print(ph1Loop());
    radioCom.print(tdsLoop());
    radioCom.print("%\n"); 
    delay(100);
    }
    delay(1000);

    // tx = false;
  // }
  // else{
  //   if(Serial.available()){
  //     char data =(char)Serial.read();
  //     Serial.println(data);
  //   }
  //   if(radioCom.available()){
  //     char data =(char)radioCom.read();
  //     Serial.println(data);
  //     tx = true;
  //   }
  // }
}

//temperature 

String tempLoop(){
  char tempString[] = "@temp#nnn.nn$";
  float tempK = sensors.getTempCByIndex(0) +273.15;
  char tValue[7];
  dtostrf(tempK, 6, 2, tValue);
  if(tempK < 100){
    tempString[6] = '0';
    for(int i = 0; i<5; i++){
      tempString[i+7] = tValue[i];
    }
  }
  else{
    for(int i = 0; i<6; i++){
      tempString[i+6] = tValue[i];
    }
  }
  return tempString;
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

String ph1Loop() {
  // PH1 NEEDS TO BE RECALIBRATED T-T
  float phValue = getPh1();
  char phAsString[6];
  dtostrf(phValue,5,2,phAsString);
  char ph1[] = "@pH#nn.nn$";
  if ( phValue <= 9.99){
    ph1[4] = 0;
  } else{
    ph1[4] = phAsString[0];
  }
  for (int i = 4; i < 9; i++){
    ph1[i] = phAsString[i-4];
  } // Print the pH value to the serial monitor (optional)
  return ph1;
  // delay(1000); // Delay for 1 second before taking another reading
}


void debug(){
  for(int i = 0; i< sendTimes; i++){
  Serial.print("S1");
  Serial.print(gpsLoop());
  Serial.print(tempLoop());
  Serial.print(ph1Loop());
  Serial.print(tdsLoop());
  Serial.print("%\n");
  Serial.println();
  delay(100);
  }
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

String tdsLoop()
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
      char tdsChars[5];
      char tds[] = "@tds#nnnn$";
      itoa(tdsValue, tdsChars, 10);
      if (tdsValue < 10){
        tds[8] = tdsChars[0];
        tds[7] = 0;
        tds[6] = 0;
        tds[5] = 0;
      } else if (tdsValue<100){
        tds[5] = tdsChars[0];
        tds[6] = tdsChars[1];
        tds[7] = 0;
        tds[8] = 0;
      }
      else if(tdsValue<1000){
        tds[5] = tdsChars[0];
        tds[6] = tdsChars[1];
        tds[7] = tdsChars[2];
        tds[8] = 0;
      }
      else{
        tds[5] = tdsChars[0];
        tds[6] = tdsChars[1];
        tds[7] = tdsChars[2];
        tds[8] = tdsChars[3];
      }
      return tds; 
   }
   
}


//main 

void setup() {
  Serial.begin(57600);
  gpsSetup();
  radioSetup();
  tempSetup();
}
//note: add sensor functions to radioLoop()! It will not work if you add it here! 
void loop() {
  radioLoop();
  debug();
  delay(1000);
} 
