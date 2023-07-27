#include <SimpleEncoder.h>
#define in1 14 //HIGH CLOCKWISE
#define in2 15 //HIGH COUNTERCLOCKWISE
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

void loop(){
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    //if(justin.CHANGING){
        positionJustin = justin.VALUE;
        Serial.println(positionJustin);
    //}
}