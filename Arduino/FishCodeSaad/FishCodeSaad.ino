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

int s1, s2;
bool check;
char incomingByte ='0';
String w;
String char1, char2, char3, char4;
int val1 = 0, val2 = 0, val3 = 0, val4 = 0;
float power = 0;          //Stores power value from controller

//Motor
unsigned int pwm_Pin1 = 8;
unsigned int pwm_Pin2 = 12;
float motor_Pwm = 0;
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
  while (!Serial.read());
  while (!Serial1.read()); //Wait for Serial to initialize

  //Initialize Servo
  pinMode(servoPin1, OUTPUT);
  pinMode(servoPin2, OUTPUT);
  servo1.attach(servoPin1); // Attach servo pins
  servo2.attach(servoPin2);
  servo1.write(initial1); // Set Servo to defaults
  servo2.write(initial2);

  //Initialize PWM_pin for motor control 
  pinMode(pwm_Pin1, OUTPUT); // Set pwm pin
  pinMode(pwm_Pin2, OUTPUT); // Set pwm pin
  
  //Initialize timer
  killTimer = millis();

  //Setup complete prompt
  digitalWrite(13, HIGH); //To Check if setup complete
  Serial.println("Setup is complete! Click to begin the program....");
  Serial.println("I'm ready for the code!");
}



void loop() {
  
  
  if (Serial1.available() > 0) {  //if bytes available to read in the buffer
    Serial.println("I'm receiving: ");
    
    // waiting for the start bit 'P':
    while (incomingByte != 'R'){
      incomingByte = Serial1.read();
    }
    Serial.print(incomingByte);
    char cmd[12];    //to store the signal from transmitter
    int siglen = 0;  //to store the length of the incoming signal
    while (incomingByte != '?' and siglen<11) {  //read char by char until we know the end of signal is reached indicated by the identifier '?'
      cmd[siglen] = incomingByte;
      siglen++;
      incomingByte = Serial1.read();
      Serial.print(incomingByte);
    }
    //Empty out buffer
    while (Serial1.available()>0){
      Serial1.read();
    }
    //As we convert char array to string last element should exist and ideally must be the limiting character
    cmd[11]='\0';
    check = checkSum(incomingByte, siglen, cmd);
    //check= true;
    Serial.print("Checksum value is: ");
    Serial.println(check);
    
    if (check == true) {
      //Timer for kill switch. Note: Keep it in checksum condition as we want it to resume operation once proper signlas are recived and not due to interfering signals.
      killTimer = millis();


      //In incoming communication message: array[0] is 'R' or 'U' or 'T' or 'P' - convert to string, array[1:] are the numbers - convert to integer
      w = String(cmd);
      Serial.print("w= ");
      Serial.println(w);
      char1 = String(w[0]);
      val1 = String(w.substring(1, 2)).toInt();
      char2 = String(w[2]);
      val2 = String(w.substring(3, 4)).toInt();
      char3 = String(w[4]);
      val3 = String(w.substring(5, 6)).toInt();
      char4 = String(w[6]);
      val4 = String(w.substring(7, 8)).toInt();


      if (char1 == "R") { //for turning the fins left and right (servo motors turn in opposite directions)
        if (val1 >= min_range && val1 < mid_range) {
          s1 = map(val1, min_range, mid_range - 1, min_angle, mid_angle); //left
          s2 = map(val1, min_range, mid_range - 1, max_angle, mid_angle);
        }
        else if (val1 > mid_range + 1 && val1 <= max_range) { //right
          s1 = map(val1, mid_range + 2, max_range, mid_angle, max_angle);
          s2 = map(val1, mid_range + 2, max_range, mid_angle, min_angle);
        }
        else if (val1 >= 4 and val1 <= 5) { //no movement
          s1 = 90;
          s2 = 90;
        }
      }

      if (char2 == "U") { //for moving the fins up and down (servo motors turn in same directions)
        if (val2 >= 0 && val2 < 4) {
          s1 = map(val2, min_range, mid_range - 2, min_angle, mid_angle); //left
          s2 = map(val2, min_range, mid_range - 1, min_angle, mid_angle);
        }
        else if (val2 > 5 && val2 <= 9) { //right
          s1 = map(val2, mid_range + 2, max_range, mid_angle, max_angle);
          s2 = map(val2, mid_range + 2, max_range, mid_angle, max_angle);
        }
        else if (val2 >= 4 and val2 <= 5) { //no movement
          s1 = 90;
          s2 = 90;
        }
      }
      
      if (char3 == "T") {
        turnVal = val3; // tail turning signal
      }
      if (char4 == "P") {
        power = val4 ; // power signal
      }
      
      
      
      
      
      //Send PWM signal to motor
      motor_Pwm = (power*255)/9.0;
      analogWrite (pwm_Pin1, motor_Pwm);
      analogWrite (pwm_Pin2, LOW);
     
      //Servo motor angle is set
      servo1.write(s1);
      servo2.write(s2);

      
    } else {
      //Signal recieved but checksum not passed -> probably got interfered
      Serial.print("Rejected! ----------------------------------------------------------------------------------------------");
    }
  }
  
      // CAUTION: Fish will turn on after kill switch activated once if signal is restored and checksum is passed
      killswitch();
      Serial.println();
      Serial.print("The message is: ");
      Serial.println(w);
      
      Serial.print("The character 1 is: ");
      Serial.println(char1);
      Serial.print("Value 1 is: ");
      Serial.println(val1);
      Serial.print("The character 2 is: ");
      Serial.println(char2);
      Serial.print("Value 2 is: ");
      Serial.println(val2);
      Serial.print("The character 3 is: ");
      Serial.println(char3);
      Serial.print("Value 3 is: ");
      Serial.println(val3);
      Serial.print("The character 4 is: ");
      Serial.println(char4);
      Serial.print("Value 4 is: ");
      Serial.println(val4);
      
      Serial.print("motor pwm: ");
      Serial.println(motor_Pwm);
      Serial.print("turn Val: ");
      Serial.println(turnVal);
      Serial.print("output to servo1: ");
      Serial.println(s1);
      Serial.print("output to servo2: ");
      Serial.println(s2);
      Serial.println();
}


void killswitch(){
   if (millis() - killTimer > 3000) {
    analogWrite (pwm_Pin1, 0);
    Serial.println("Kill switch activated"); 
  }
}

//function for check sum
bool checkSum(char incomingByte, int siglen, char cmd[]) {
  check = false;
  int cSum=0;
  for (int c=0; c<8; c++){
    cSum+=cmd[c];
  }
  String numcheck = String(cmd[8]) + String(cmd[9]) + String(cmd[10]);
  int number = numcheck.toInt();
  Serial.println("Csum & number: ");
  Serial.println(cSum);
  Serial.println(number);
  if (cSum == number) {
    check = true;
  }
  return check;
}
