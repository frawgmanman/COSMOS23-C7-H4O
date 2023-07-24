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
    ethernetCom.print("S1");
    ethernetCom.print(tempLoop());
    ethernetCom.print(ph1Loop());
    ethernetCom.print(tdsLoop());
    ethernetCom.print("%\n"); 
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
  ethernetSetup();
  tempSetup();
}
//note: add sensor functions to radioLoop()! It will not work if you add it here! 
void loop() {
  ethernetLoop();
  debug();
  delay(1000);
} 