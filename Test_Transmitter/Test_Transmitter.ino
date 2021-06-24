/*
  MX1508 DC MOTOR DRIVER MODULE
  modified on 25 Sep 2020
  by Saeed Olfat @ Electropeak
  https://electropeak.com/learn/
*/
int a=4;
int b=12;
void setup() {
  pinMode(a, OUTPUT); //IN2
  pinMode(b, OUTPUT); //IN1
}
void loop() {

// Full speed forward
  digitalWrite(a, HIGH);
  digitalWrite(b, LOW);


  delay(3000);
//
//// Full speed backward
//  digitalWrite(a, LOW);
//  digitalWrite(b, HIGH);
//
//  delay(3000);
//
//// 0 to 100% speed in forward mode
//  for (int i=0;i<256;i++)
//  {   digitalWrite(a, HIGH);
//      analogWrite(b, i);
//      delay(20);      
//      }
//
//  delay(50);
//
//  // 0 to 100% speed in backward mode
//        for (int i=0;i<256;i++)
//  {   digitalWrite(b, HIGH);
//      analogWrite(a, i);
//      delay(20);      
//      }
//
//        delay(50);
}
