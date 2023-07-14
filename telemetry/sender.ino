#include <SoftwareSerial.h>
const byte rxPin = 4;
const byte txPin = 5;
static bool tx = true;
SoftwareSerial radioCom (rxPin, txPin);

void setup() {
  Serial.begin(57600);
  radioCom.begin(57600); 

}

void loop() {
  
  if (tx){
    delay(1000);
    tx = false;
  }
  else{
    if(Serial.available()){
      char data =(char)Serial.read();
      Serial.println(data);
    }
    if(radioCom.available()){
      char data =(char)radioCom.read();
      Serial.println(data);
      tx = true;
    }
  }
}