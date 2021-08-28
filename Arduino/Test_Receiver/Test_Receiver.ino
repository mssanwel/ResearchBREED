String incomingByte = ""; // for incoming serial data

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);
  Serial.print("Starting: ");
}

void loop() {

  //Receiving
  // send data only when you receive data:
  if (Serial1.available() > 0) {
    // read the incoming byte:
    incomingByte = Serial1.read();

    // say what you got:
    Serial.print("I received: ");
    Serial.println(incomingByte);
  }

}
