#include <SoftwareSerial.h>
#define tx 2
#define rx 3


SoftwareSerial configBt(rx, tx); // RX, TX

void setup() 
{
  Serial.begin(38400);
  configBt.begin(38400);
  pinMode(tx, OUTPUT);
  pinMode(rx, INPUT);
}

void loop() 
{
  if(configBt.available()) // if the HC05 is sending something… 
  {
    Serial.print(configBt.readString()); // print in serial monitor
  }
  if(Serial.available()) // if serial monitor is outputting something… 
  {
    configBt.write(Serial.read()); // write to Arduino’s Tx pin
  }
}
