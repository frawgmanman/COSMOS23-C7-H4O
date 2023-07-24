#include <Servo.h> 

 Servo winch;
 int servoPin = 0; // check later




 void setup(){
    winch.attach(servoPin);
    servo.write(90);
 }

 void loop(){
    winch.write(180);
    delay(2000);
    winch.write(90);
    delay(500);
    winch.write(0)
    delay(2000);
 }