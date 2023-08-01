#include <SoftwareSerial.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#include <SimpleEncoder.h>
#define in1 14 //HIGH CLOCKWISE
#define in2 15 //HIGH COUNTERCLOCKWISE
SimpleEncoder justin(0,34,35);


long positionJustin = -999;
// GPIO where the DS18B20 is connected to
const int oneWireBus = 17;     

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(oneWireBus);

// Pass our oneWire reference to Dallas Temperature sensor 
DallasTemperature sensors(&oneWire);

SoftwareSerial eCom(4,5);

#define USE_PULSE_OUT

#ifdef USE_PULSE_OUT
  #include "do_iso_grav.h"       
  Gravity_DO_Isolated DO = Gravity_DO_Isolated(26);         
#else
  #include "do_grav.h"
  Gravity_DO DO = Gravity_DO(26);
#endif

float getDO(){
    return DO.read_do_percentage();
}

uint8_t user_bytes_received = 0;
const uint8_t bufferlen = 32;
char user_data[bufferlen];


void parse_cmd(char* string) {
  strupr(string);
  String cmd = String(string);
  if(cmd.startsWith("CAL")){
    int index = cmd.indexOf(',');
    if(index != -1){
      String param = cmd.substring(index+1, cmd.length());
      if(param.equals("CLEAR")){
        DO.cal_clear();
        Serial.println("CALIBRATION CLEARED");
      }
    }
    else{
      DO.cal();
      Serial.println("DO CALIBRATED");
    }
  }
}

void sendData(){
    if (Serial.available() > 0) {
    user_bytes_received = Serial.readBytesUntil(13, user_data, sizeof(user_data));
  }

  if (user_bytes_received) {
    parse_cmd(user_data);
    user_bytes_received = 0;
    memset(user_data, 0, sizeof(user_data));
  }
  sensors.requestTemperatures();
  float temperatureK =sensors.getTempCByIndex(0) + 273.15;
  Serial.println(temperatureK);
  Serial.println(getDO());
  eCom.print("@temp#");
  eCom.print(temperatureK);
  eCom.print("$@do#");
  eCom.print(getDO());
  eCom.print('$');
  eCom.print('!');
  delay(100);
  //eCom.print('\n');
}



void setup(){
  Serial.begin(9600);
  eCom.begin(9600);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  sensors.begin();
}

void loop(){
    sendData();
}