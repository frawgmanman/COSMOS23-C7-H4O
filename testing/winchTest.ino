#include <SimpleEncoder.h>
#define in1 14
#define in2 15
SimpleEncoder justin(0,34,35);


long positionJustin = -999;

static bool justinUp = false;
static bool justinDown = true;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  //Serial.println("test");
  if(justinUp){
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
  }
  else if(justinDown){
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
  }
  else{
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
  }
//   if(justin.VALUE>20){
//     justinUp = false;
//     justinDown = true;
//     delay(1000);
//   }
//   if(justin.Value<20){
//     justinUp = true;
//     justinDown = false; 
//     delay(1000);
//   }
  if(justin.CHANGING) {
        positionJustin = justin.VALUE;
        Serial.println(positionJustin);
    }
  
}