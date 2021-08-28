/*
  MX1508 DC MOTOR DRIVER MODULE
  modified on 25 Sep 2020
  by Saeed Olfat @ Electropeak
  https://electropeak.com/learn/
*/
void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);
}
void loop() {
  Serial.println("Hello\n");
  Serial1.println("Hello\n");
  delay(100);
}
