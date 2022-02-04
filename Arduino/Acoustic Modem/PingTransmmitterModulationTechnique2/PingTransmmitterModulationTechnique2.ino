// ---------------------------------------------------------------- //
// Arduino Ultrasoninc Sensor HC-SR04
// Re-writed by
// Using Arduino IDE 1.8.7
// Using HC-SR04 Module
// Tested on 17 September 2019
// ---------------------------------------------------------------- //

#define echoPin 2 // attach pin D2 Arduino to pin Echo of HC-SR04
#define trigPin 3 //attach pin D3 Arduino to pin Trig of HC-SR04

// defines variables
long duration; // variable for the duration of sound wave travel
int distance; // variable for the distance measurement

char incomingByte;
int multi=10;
#define t0 20
#define t1 40
#define tGuard 80

void setup() {
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an OUTPUT
  pinMode(echoPin, INPUT); // Sets the echoPin as an INPUT
  Serial.begin(9600); // // Serial Communication is starting with 9600 of baudrate speed
  Serial.println("Ultrasonic Sensor HC-SR04 Test"); // print some text in Serial Monitor
  Serial.println("with Arduino UNO R3");
}
void loop() {
  //clear the buffer now
  while (Serial.available()) {
    Serial.print("Clearing the buffer: ");
    Serial.println(Serial.read());
  }
  Serial.println("Waiting for serial input");
  while (Serial.available() == 0) {} //Loop until serial command receieved
  // read the incoming byte:
  incomingByte = Serial.read();
  Serial.print("I received: ");
  Serial.println(incomingByte, BIN);

<<<<<<< HEAD
//  digitalWrite(trigPin, LOW);
//  delayMicroseconds(t);
  digitalWrite(trigPin, HIGH);// 1
  delay(t1);
  digitalWrite(trigPin, LOW);
  delay(tGuard);

//  digitalWrite(trigPin, LOW);
//  delayMicroseconds(t);
  digitalWrite(trigPin, HIGH);// 1
  delay(t1);
  digitalWrite(trigPin, LOW);
  delay(tGuard);

  //  // Clears the trigPin condition
  //  digitalWrite(trigPin, LOW);
  //  delay(20);
  //  // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
  //  digitalWrite(trigPin, HIGH);
  //  delay(20);
  //  digitalWrite(trigPin, LOW);
  //
  //  // Clears the trigPin condition
  //  digitalWrite(trigPin, LOW);
  //  delay(60);
  //  // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
  //  digitalWrite(trigPin, HIGH);
  //  delay(60);
  //  digitalWrite(trigPin, LOW);
  //
  //  // Clears the trigPin condition
  //  digitalWrite(trigPin, LOW);
  //  delay(180);
  //  // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
  //  digitalWrite(trigPin, HIGH);
  //  delay(180);
  //  digitalWrite(trigPin, LOW);
  int t = 20000;
  //Clears the trigPin condition
  //  digitalWrite(trigPin, LOW);
  //  delayMicroseconds(tGuard);


  for (int i = 0; i < 5000; i++) {
    digitalWrite(trigPin, HIGH);// 1 Start Bit
    delay(t1);
    digitalWrite(trigPin, LOW);
    delay(tGuard);

  digitalWrite(trigPin, HIGH);// 0
  delay(t0);
  digitalWrite(trigPin, LOW);
  delay(tGuard);


    for (int j = 6, l = 0; j >= 0; j--, l++) {
      int sig = bitRead(incomingByte, j);  //the bit to send
      if (i == 0) {  //only print the first time
        Serial.print("Writing bit ");
        Serial.print(l);
        Serial.print(" -> ");
        Serial.println(sig);
      }
      digitalWrite(trigPin, sig);
      if (sig == 0) {
        delay(t0);
      }
      else {
        delay(t1);
      }
      digitalWrite(trigPin, !sig);
      delay(tGuard);
    }


    digitalWrite(trigPin, LOW);// 0 Stop bit
    delay(tGuard);
    digitalWrite(trigPin, LOW);
    delay(tGuard);

    //  digitalWrite(trigPin, LOW);
    //  delayMicroseconds(t);
    digitalWrite(trigPin, LOW);// 0 Stop bit
    delay(tGuard);
    digitalWrite(trigPin, LOW);
    delay(tGuard);
  }
}
