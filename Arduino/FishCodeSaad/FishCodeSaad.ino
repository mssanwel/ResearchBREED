//Libraries
#include <Servo.h>
#include <SPI.h>
#include <Wire.h>

//Servo
Servo servo1;
Servo servo2;
int servoPin1 = 5;
int servoPin2 = 3;
int initial1 = 90;
int initial2 = 90;
int min_angle = 0;
int max_angle = 180;
int mid_angle = 90;
int min_range = 0;
int max_range = 9;
int mid_range = 4;

//Communication
int num = 0;
char cmd[11];    //to store the signal from transmitter
int s1, s2;
bool check;
char incomingByte;
String w;
String letter;

//Motor
unsigned int pwm_Pin = 9;
unsigned int motor_Pwm = 0;
bool red_Flag = LOW;
bool on = LOW;

//encoder
unsigned long int timer1 = 0;
unsigned long int timer2 = 0;
long int counter = 0;
long int runtime = 0;
long int timer = 0;
float timeStep = 0;
long int encoderCounter = 0;
double angularVelocity[3] = {0, 0, 0};

//Turn Differential
int turnVal = 5;        //Stores commmand value for control left and right. values 1-4 are left, 5 is straight, 6-9 are right
int oldTurnVal = 5;
int encoderVal = 0;         //stores value of encoder at any given time. Updates from interrupt.
int power = 9;          //Stores power value from controller
int oldPower = 5;
long int tailDelay1 = 2000;
int encoderPin0   =  29;
double diff = 1.0;              //to store the differential value corresponding to the signal
double stepDiff = 0.2;          //sets the differential value
int highcutoff, lowcutoff, offset;

//Kill Switch
long int killTimer; //timer for kill switch



void setup() {
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW); // Off to indicate still in setup
  
  //Initialize Serial
  Serial.begin(9600, SERIAL_8O1);
  Serial1.begin(9600);
  while (!Serial.read()); //Wait for Serial to initialize

  //Initialize Servo
  pinMode(servoPin1, OUTPUT);
  pinMode(servoPin2, OUTPUT);
  servo1.attach(servoPin1); // Attach servo pins
  servo2.attach(servoPin2);
  servo1.write(initial1); // Set Servo to defaults
  servo2.write(initial2);

  //Initialize PWM_pin for motor control 
  pinMode(pwm_Pin, OUTPUT); // Set pwm pin
 
  //Initialize timer
  killTimer = millis();

  //Setup complete prompt
  digitalWrite(13, HIGH); //To Check if setup complete
  Serial.println("Setup is complete! Click to begin the program....");
  Serial.println("I'm ready for the code!");
}



void loop() {
  
  
  if (Serial1.available() > 0) {  //if bytes available to read in the buffer
    Serial.print("I'm receiving: ");
    
    // read the incoming byte:
    incomingByte = Serial1.read();
    Serial.print(incomingByte);
    int siglen = 0;  //to store the length of the incoming signal
    while (incomingByte != '?' and siglen<11) {  //read char by char until we know the end of signal is reached indicated by the identifier '?'
      cmd[siglen] = incomingByte;
      siglen++;
      incomingByte = Serial1.read();
      Serial.print(incomingByte);
    }
    Serial.println();
    check = checkSum(incomingByte, siglen, cmd);
    Serial.print("Checksum value is: ");
    Serial.println(check);
    
    if (check == true) {
      //Timer for kill switch. Note: Keep it in checksum condition as we want it to resume operation once proper signlas are recived and not due to interfering signals.
      killTimer = millis();


      //In incoming communication message: array[0] is 'R' or 'U' or 'T' or 'P' - convert to string, array[1:] are the numbers - convert to integer
      w = String(cmd);
      letter = String(w[0]);
      num = String(w.substring(1, 2)).toInt();
      
      if (letter == "T") {
        turnVal = num; // tail turning signal
      }
      else if (letter == "P") {
        power = num ; // power signal
      }
      else if (letter == "R") { //for turning the fins left and right (servo motors turn in opposite directions)
        if (num >= min_range && num < mid_range) {
          s1 = map(num, min_range, mid_range - 1, min_angle, mid_angle); //left
          s2 = map(num, min_range, mid_range - 1, max_angle, mid_angle);
        }
        else if (num > mid_range + 1 && num <= max_range) { //right
          s1 = map(num, mid_range + 2, max_range, mid_angle, max_angle);
          s2 = map(num, mid_range + 2, max_range, mid_angle, min_angle);
        }
        else if (num >= 4 and num <= 5) { //no movement
          s1 = 90;
          s2 = 90;
        }
      }
      else if (letter == "U") { //for moving the fins up and down (servo motors turn in same directions)
        if (num >= 0 && num < 4) {
          s1 = map(num, min_range, mid_range - 2, min_angle, mid_angle); //left
          s2 = map(num, min_range, mid_range - 1, min_angle, mid_angle);
        }
        else if (num > 5 && num <= 9) { //right
          s1 = map(num, mid_range + 2, max_range, mid_angle, max_angle);
          s2 = map(num, mid_range + 2, max_range, mid_angle, max_angle);
        }
        else if (num >= 4 and num <= 5) { //no movement
          s1 = 90;
          s2 = 90;
        }
      }
      
      
      
      //Send PWM signal to motor
      analogWrite (pwm_Pin, motor_Pwm);
      
     
      //Servo motor angle is set
      servo1.write(s1);
      servo2.write(s2);

      
    } else {
      //Signal recieved but checksum not passed -> probably got interfered
      Serial.print("Rejected!");
    }
  }
  
      // CAUTION: Fish will turn on after kill switch activated once if signal is restored and checksum is passed
      killswitch();
      Serial.println();
      Serial.print("The character is: ");
      Serial.println(letter);
      Serial.print("The number for movement is: ");
      Serial.println(num);
      Serial.print("motor pwm: ");
      Serial.println(motor_Pwm);
      Serial.print("output to servo1: ");
      Serial.println(s1);
      Serial.print("output to servo2: ");
      Serial.println(s2);
}


void killswitch(){
   if (millis() - killTimer > 3000) {
    analogWrite (pwm_Pin, 0); 
  }
}

//function for check sum
bool checkSum(char incomingByte, int siglen, char cmd[]) {
    String numcheck = String(cmd[2]) + String(cmd[3]) + String(cmd[4]);
    int number = numcheck.toInt();
    if (cmd[0] + cmd[1] == number) {
      Serial.print("Inside the array there is: ");
      for (int i = 0; i < 2; i++) {
        Serial.print(cmd[i]);
      }
      check = true;
    } else {
      check = false;
    }
    return check;
}
