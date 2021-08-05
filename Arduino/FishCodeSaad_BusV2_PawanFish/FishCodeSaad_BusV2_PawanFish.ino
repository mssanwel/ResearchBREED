//Libraries
#include <Servo.h>
#include <SPI.h>
#include <Wire.h>
byte x=0;
//Servo
//Servo servo1;
//Servo servo2;
//int servoPin1 = 5;
//int servoPin2 = 2;
int initial1 = 90;
int initial2 = 90;
int min_angle = 0;
int max_angle = 180;
int mid_angle = 90;
int min_range = 0;
int max_range = 9;
int mid_range = 4;
float s1, s2;



//Communication

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
long int encoderRawVal = 0; //Global Variable for encoder absolute value
double angularVelocity[3] = {0, 0, 0};

//Turn Differential
int turnVal = 5;        //Stores commmand value for control left and right. values 1-4 are left, 5 is straight, 6-9 are right
int oldTurnVal = 5;
int encoderVal = 0;         //stores value of encoder at any given time. Updates from interrupt.
float turn_Pwm = 0;         // Pwm value with differential applied
int oldPower = 5;
long int tailDelay1 = 2000;
int encoderPin0   =  29;
double diff = 0.5;              //to store the differential value corresponding to the signal
double stepDiff = 0.1;          //sets the differential value
int highcutoff, lowcutoff, offset;

//Kill Switch
long int killTimer; //timer for kill switch

//Bus
long int busTimer1; //timer for bus transmission to slave with servo fin control
String slaveMess="";
char message[20];


//Encoder Motor Servos
//
///Motor Set A
#define ENCA_A 3 // YELLOW
#define ENCB_A 2 // WHITE
#define PWM_A 5
#define IN2_A 6
#define IN1_A 7


//Motor Set b
#define ENCA_B 19// YELLOW
#define ENCB_B 18 // WHITE
#define PWM_B 10
#define IN2_B 9
#define IN1_B 8

int maxpower = 255;
int minpower = 150;

int pos_A = 0;
long prevT_A = 0;
float eprev_A = 0;
float eintegral_A = 0;
long pos_B = 0;
long prevT_B = 0;
float eprev_B = 0;
float eintegral_B = 0;

  // PID constants
float kp_A = 0.5;
float kd_A = 0.02;
float ki_A = 0;

float kp_B = 0.5;
float kd_B = 0.02;
float ki_B = 0; 




void setup() {
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW); // Off to indicate still in setup
  
  //Initialize Serial
  Serial.begin(9600, SERIAL_8O1);
  Serial1.begin(9600);
  //while (!Serial.read());
  //while (!Serial1.read()); //Wait for Serial to initialize

  //Initialize Servo
//  pinMode(servoPin1, OUTPUT);
//  pinMode(servoPin2, OUTPUT);
//  servo1.attach(servoPin1); // Attach servo pins
//  servo2.attach(servoPin2);
//  servo1.write(initial1); // Set Servo to defaults
//  servo2.write(initial2);

  //Initialize PWM_pin for motor control 
  pinMode(pwm_Pin1, OUTPUT); // Set pwm pin
  pinMode(pwm_Pin2, OUTPUT); // Set pwm pin
  
  //Initialize timer
  killTimer = millis();

  // join i2c bus (address optional for master)
  Wire.begin(8);                // join i2c bus with address #8
  Wire.onRequest(requestEvent);  // register event

  //Encoder Setup
  for(int encoderPin = 22; encoderPin <= 41; encoderPin = encoderPin + 2){   //absolute encoder pin setup
    pinMode(encoderPin,INPUT_PULLUP) ; 
  }
  Serial.println("Encoder Pins Initialized!");
  Serial.println("Setup is complete! Click to begin the program....");
  

  //Servo Encoder Motors
  //
  pinMode(ENCA_A,INPUT_PULLUP);
  pinMode(ENCB_A,INPUT_PULLUP);
  pinMode(PWM_A, OUTPUT);
  pinMode(IN1_A, OUTPUT);
  pinMode(IN2_A, OUTPUT);
  pinMode(PWM_B, OUTPUT);
  pinMode(IN1_B, OUTPUT);
  pinMode(IN2_B, OUTPUT);  
  attachInterrupt(digitalPinToInterrupt(ENCA_A),readEncoder_A,RISING);
  attachInterrupt(digitalPinToInterrupt(ENCB_B),readEncoder_B,RISING);

  
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

      float maxAttacAngle=10;
      //Servo control expression. Linear combination of X and Y component of JoyStick
      s1= ((4-val1)*maxAttacAngle/5 +(4-val2)*maxAttacAngle/5)+ 90.0;
      s2= ((4-val1)*maxAttacAngle/5 -(4-val2)*maxAttacAngle/5)+ 90.0;
      
      if (char3 == "T") {
        turnVal = val3; // tail turning signal
      }
      if (char4 == "P") {
        power = val4 ; // power signal
      }
      
      
      
      
      
      //Send PWM signal to motor
      motor_Pwm = (power*255)/9.0;
      turn_Pwm = ((power*255)/9.0)*diff;
//      analogWrite (pwm_Pin1, motor_Pwm);
//      analogWrite (pwm_Pin2, LOW);
     
      //Servo motor angle is set
//      servo1.write(s1);
//      servo2.write(s2);
      setPosition(s1, s2);
      
    } else {
      //Signal recieved but checksum not passed -> probably got interfered
      Serial.print("Rejected! ----------------------------------------------------------------------------------------------");
    }
  }
      


      

      
      // CAUTION: Fish will turn on after kill switch activated once if signal is restored and checksum is passed
      killswitch();
      encoderRead();
  
  
    // Turning control Left
    if ((turnVal>=1) and (turnVal<=4)){
      Serial.println("Turning Left");
      if ((abs(encoderRawVal-offset)<=359) or (abs(encoderRawVal-offset)>=871)){
            motor_Pwm = ((power*255)/9.0)*diff;
      }
      else{
        motor_Pwm = (power*255)/9.0;
      } 
    }

    
    // Going Straight
    else if (turnVal==5){
      Serial.println("Going straight");
      motor_Pwm = (power*255)/9.0; //maps the value received (0-9) to (0-255)
    }

    // Turning control Right
    else if ((turnVal>=6) and (turnVal<=9)){
      Serial.println("Turning Right");
      if ((abs(encoderRawVal-offset)>=364) and (abs(encoderRawVal-offset)<=870)){
        motor_Pwm = (power*255)/9.0*diff;
      }
      else{
        motor_Pwm = ((power*255)/9.0);
      }
    }
  
  
  
  analogWrite (pwm_Pin1, motor_Pwm);
  analogWrite (pwm_Pin2, LOW);
  Serial.print("motor pwm:------------------------->");
  Serial.println(motor_Pwm);
















      
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
      Serial.print("EncoderValue ---> : ");
      Serial.println(encoderRawVal);
      Serial.println();
}


//void requestEvent() {
//  Wire.write("hello "); // respond with message of 6 bytes
//  // as expected by master
//}

void requestEvent()
{
  //Bus operation
      //State transmitted from BUS to slave with servo fin connection
        slaveMess=String(encoderRawVal)+"?";
        strcpy(message,slaveMess.c_str());
        Wire.write(message);        //Transmit fish state
}

void killswitch(){
   if (millis() - killTimer > 3000) {
    power=0;
    motor_Pwm = 0;
    analogWrite(pwm_Pin1, motor_Pwm);
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
  //Serial.println("Csum & number: ");
  //Serial.println(cSum);
  //Serial.println(number);
  if (cSum == number) {
    check = true;
  }
  return check;
}

//Encoder Function
void encoderRead(){
  int a[10];
  int b[10];
  for(int n=0; n<10; n++){
    a[n] = !digitalRead(22+2*n);
    Serial.println(n);
  }
  b[9] = a[9];
  for(int i = 1;i<10;i++){
    b[9-i] = b[9-i+1]^a[9-i];  // xor
  }
  encoderRawVal = 512.0*b[9]+256.0*b[8]+128.0*b[7]+64.0*b[6]+32.0*b[5]+16.0*b[4]+8.0*b[3]+4.0*b[2]+2.0*b[1]+b[0];
}

//Servo Encoder Motor
//

void setMotor(int dir, int pwmVal, int pwm, int in1, int in2){
  analogWrite(pwm,pwmVal);
  if(dir == 1){
    digitalWrite(in1,HIGH);
    digitalWrite(in2,LOW);
  }
  else if(dir == -1){
    digitalWrite(in1,LOW);
    digitalWrite(in2,HIGH);
  }
  else{
    digitalWrite(in1,LOW);
    digitalWrite(in2,LOW);
  }  
}


void setPosition(int target_a, int target_b){

  int e_A = pos_A-target_a;
  int e_B = pos_B-target_b;
  
  
  while (fabs(e_A) >= 0 or fabs(e_B) >= 0 ){
    
    long currT_A = micros();
    float deltaT_A = ((float) (currT_A - prevT_A))/( 1.0e6 );
    prevT_A = currT_A;
      
    long currT_B = micros();
    float deltaT_B = ((float) (currT_B - prevT_B))/( 1.0e6 );
    prevT_B = currT_B;
          
    // error
    e_A = pos_A-target_a;
    e_B = pos_B-target_b;
          
    // derivative
    float dedt_A = (e_A-eprev_A)/(deltaT_A);
    float dedt_B = (e_B-eprev_B)/(deltaT_B);
          
    // integral
    eintegral_A = eintegral_A + e_A*deltaT_A;
    eintegral_B = eintegral_B + e_B*deltaT_B;
          
    // control signal
    float u_A = kp_A*e_A + kd_A*dedt_A + ki_A*eintegral_A;
    float u_B = kp_B*e_B + kd_B*dedt_B + ki_B*eintegral_B;
  
    // motor power
    float pwr_A  = fabs(u_A );
    if( pwr_A  > maxpower ){
      pwr_A  = maxpower;
    }

          
    float pwr_B = fabs(u_B);
    if( pwr_B > maxpower ){
      pwr_B = maxpower;
    }

    //motor direction
    int dir_A = 1;
    if(u_A<0){
      dir_A = -1;
    }
          
     int dir_B = 1;
     if(u_B<0){
      dir_B = -1;
     }

     setMotor(dir_A,pwr_A,PWM_A,IN1_A,IN2_A);
     setMotor(dir_A,pwr_B,PWM_B,IN1_B,IN2_B);

     // store previous error
     eprev_A = e_A;
     eprev_B = e_B;

     Serial.print(target_a);
     Serial.print(" ");
     Serial.print(pos_A);
     Serial.print(" ");
     Serial.print(target_b);
     Serial.print(" ");
     Serial.println(pos_B);

     if (fabs(e_A) < 100 and fabs(e_B) < 100 ){  
       break; 
     }

     //if (fabs(e_A) < fabs(target_a * 0.1) and fabs(e_B) < fabs(target_b * 0.1) ){  
      // break; 
     //}
          
  }

  
  setMotor(0,0,PWM_A,IN1_A,IN2_A);
  setMotor(0,0,PWM_B,IN1_B,IN2_B);
 

}




void readEncoder_A(){
  int b_A = digitalRead(ENCB_A);
  if(b_A > 0){
    pos_A++;
  }
  else{
    pos_A--;
  }
}

void readEncoder_B(){
  int b_B = digitalRead(ENCA_B);
  if(b_B > 0){
    pos_B++;
  }
  else{
    pos_B--;
  }
}
