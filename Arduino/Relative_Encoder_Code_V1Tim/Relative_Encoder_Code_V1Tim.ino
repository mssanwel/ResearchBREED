/* This code sets the motor to spin at a fix speed and measures 


1) Rotation Count
2) Displays the current position 
3) Displays the speed of the motor
*/

#include <Servo.h>

long int counter = 0;
const long int ticRatioMainMotor = 6000;
float rel_angle = 0;
volatile long int pos_Main;
Servo motor;

#define ENCA 3 // YELLOW
#define ENCB 2 // WHITE

int motor_Pwm = 1550;

///////////////////////////////////////////////////////////////////////////////////////////////////////
#define THROTTLE_MIN        1500
#define THROTTLE_MAX        2000
#define THROTTLE_BAKE       1000

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  // Attach the relative encoder to the ISR
  pinMode(ENCA,INPUT_PULLUP);
  pinMode(ENCB,INPUT_PULLUP);
  motor.attach(5);
  counter = 0;
  motor.writeMicroseconds(1500);
  delay(1000);
  attachInterrupt( digitalPinToInterrupt(ENCA), readEncoder_Main, RISING);
  

 
}

void loop() {
  // put your main code here, to run repeatedly:
  rel_Encoder();
//  if (rel_angle< 180){
//    motor_Pwm = ((75));
//  }
//  else{
//    motor_Pwm = (155);
//  } 
//  int throttle=map(motor_Pwm, 0, 255, THROTTLE_MIN, THROTTLE_MAX);
//  motor.writeMicroseconds(throttle);
  motor.writeMicroseconds(motor_Pwm);
}

void rel_Encoder(){
    long int x = pos_Main;
    Serial.print(pos_Main);

    // removes the cumulative ticks
    counter = 0;
     while (((long int)x/ticRatioMainMotor)>0){
      x=x-ticRatioMainMotor;
      counter++;
  }

  Serial.print(" This is the final value of x");
  Serial.print("\t");
  Serial.print(x);
  Serial.print("\t");
  Serial.print(counter);

  rel_angle = (x/(ticRatioMainMotor*1.0))*360; //this is angle in degrees
  Serial.print("\t");
  Serial.println(rel_angle);
  
  
}


void readEncoder_Main(){
    int b_A = digitalRead(ENCB);
  if(b_A > 0){
    pos_Main++;
  }
  else{
    pos_Main--;
  }

//  //Serial.print("Relative Encoder: ---------------------------->");
//  //Serial.println(pos_Main);
  //yield();
}
