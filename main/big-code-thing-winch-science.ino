//ethernet
#include <SoftwareSerial.h>
//temperature
#include <OneWire.h>
#include <DallasTemperature.h>
//TDS
#define TdsSensorPin 23 
#include <string>
using namespace std;



//temperature variables
const int oneWireBus = 17;
OneWire oneWire(oneWireBus);
DallasTemperature sensors(&oneWire);

//ethernet variables
const byte rxPin = 4;
const byte txPin = 5;
static bool tx = true;
SoftwareSerial ethernetCom (rxPin, txPin);
const static int sendTimes = 10;


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





//ethernet

 void ethernetSetup(){
  ethernetCom.begin(57600);
}

void ethernetLoop(){
    // if (tx){
    for(int i = 0; i < sendTimes; i++){
    ethernetCom.print(getTemp());
    ethernetCom.print("\n");
    ethernetCom.print(getPh1());
    ethernetCom.print("\n");
    ethernetCom.print(tdsLoop());
    ethernetCom.print("\n"); 
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

float getTemp(){
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


void debug(){
  for(int i = 0; i< sendTimes; i++){
  Serial.print("S1");
  Serial.print(tempLoop());
  Serial.print(ph1Loop());
  Serial.print(tdsLoop());
  Serial.print("%\n");
  Serial.println();
  delay(100);
  }
}


//tds
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
 


//main 

void setup() {
  Serial.begin(57600);
  ethernetSetup();
  tempSetup();
}
//note: add sensor functions to radioLoop()! It will not work if you add it here! 
void loop() {
  ethernetLoop();
  debug();
  delay(1000);
} 