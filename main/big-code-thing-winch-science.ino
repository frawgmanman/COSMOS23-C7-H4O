//ethernet
#include <SoftwareSerial.h>
//temperature
#include <OneWire.h>
#include <DallasTemperature.h>


//temperature variables
const int oneWireBus = 17;
OneWire oneWire(oneWireBus);
DallasTemperature sensors(&oneWire);

//ethernet variables
const byte rxPin = 4;
const byte txPin = 5;
static bool dataSent = false;
SoftwareSerial ethernetCom (rxPin, txPin);
const static int sendTimes = 10;
static bool readyToSend = false;


//ph variables
const int SensorPin = 15;
float buf[10];
int temp;


//ethernet

 void ethernetSetup(){
  ethernetCom.begin(57600);
}
String eRead(){
    if(ethernetCom.available()){
        char data = (char)eCom.read();
        return data;
    }
    // else{
    // return "i love sophia"; 
    // }
}
void ethernetLoop(){
    if (!dataSent){
    //for(int i = 0; i < sendTimes; i++){
    while(!readyToSend){
      char data = eRead();
      if(data = '?'){
        readyToSend = true; 
        ethernetCom.print('^');
      }
    }
    delay(500);
    ethernetCom.print("@temp#");
    ethernetCom.print(getTemp());
    ethernetCom.print("$@ph#");
    ethernetCom.print(getPh1());
    ethernetCom.print("$%");
  
    dataSent = true;
    readyToSend = false; 
   // }
    
   }
  else{
    if(Serial.available()){
      char data =(char)Serial.read();
      Serial.println(data);
    }
    if(ethernetCom.available()){
      char data =(char)ethernetCom.read();
      Serial.println(data);
      if(data = '!'){
      ethernetCom.print("*");
      dataSent = false;
      }
    }
  }
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
} 