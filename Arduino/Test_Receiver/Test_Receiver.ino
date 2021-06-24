
#include <SD.h>
#include <Servo.h>
#include <SPI.h>
#include <Wire.h>



void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);
  pinMode(12, OUTPUT);
  pinMode(8, OUTPUT);
}




void loop() {
 
  //Serial.println("Running");
  if (Serial.available() > 0) {
      String Data=Serial.readString();
      Serial.println(Data);
      Serial.println("recieved");
    
      }


  digitalWrite(12, HIGH);
  digitalWrite(8, LOW);

}
