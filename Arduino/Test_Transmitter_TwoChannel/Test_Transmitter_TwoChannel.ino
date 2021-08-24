/*
  MX1508 DC MOTOR DRIVER MODULE
  modified on 25 Sep 2020
  by Saeed Olfat @ Electropeak
  https://electropeak.com/learn/
*/
String incomingByte = ""; // for incoming serial data

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);
  Serial2.begin(9600);
}
void loop() {
  //Transmitting
  Serial1.println("Hello\n");
  delay(100);


  //Receiving
  if (Serial2.available() > 0) {
    // read the incoming byte:
    incomingByte = Serial2.read();

    // say what you got:
    Serial.print("I received: ");
    Serial.println(incomingByte);
  }
  
}
