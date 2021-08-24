String incomingByte = ""; // for incoming serial data

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);
  Serial2.begin(9600); // opens serial port, sets data rate to 9600 bps
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

  //Transmitting
  Serial2.println("Hello\n");
  delay(100);
}
