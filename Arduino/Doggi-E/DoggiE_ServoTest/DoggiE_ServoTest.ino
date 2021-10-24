#include <Servo.h> 

Servo myservo;
 int numPins=12;
 int firstPin=28;
  
void setup() 
{ 
  myservo.attach(36);
// set servo to mid-point
} 

void loop() {
  myservo.write(120);
  delay(1000);

  
  //for (int i=0;i<numPins;i++) {
     //myservo.attach(firstPin + i); 
     //myservo.write(90);  // set servo to mid-point

     //delay(100);
  //}
} 
