#include <Servo.h>
int idx = 1;
long int counter = 0;
const long int ticRatioMainMotor = 6000;
float rel_angle = 0;
volatile long int pos_Main=0;

#define ENCA 3 // YELLOW
#define ENCB 2 // WHITE

Servo   pusherESC;  

#define THROTTLE_MIN        1500
#define THROTTLE_MAX        2000
#define THROTTLE_BRAKE       1000

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  // Attach the relative encoder to the ISR
  pinMode(ENCA,INPUT_PULLUP);
  pinMode(ENCB,INPUT_PULLUP);
  attachInterrupt( digitalPinToInterrupt(ENCA), readEncoder_Main, RISING);
  pusherESC.attach(6);
  pusherESC.writeMicroseconds(THROTTLE_MIN);
  Serial.println("Arming........");   // just some display message 
  delay(2000);
   
}

void loop() {
  // put your main code here, to run repeatedly:
  int tem = map(tem,1,20,1500,2000);
  Serial.println(pos_Main);
  
  
  while(pos_Main<1100){
    analogWrite(2,50);
  }
  analogWrite(2,0);
  delay(2000);
  while(pos_Main<2200){
    analogWrite(2,25);
  }
  analogWrite(2,0);
  
  idx++;
    
  
}



void readEncoder_Main(){
 int b_A = digitalRead(ENCB);
  if(b_A > 0){
    pos_Main++;
  }
  else{
    pos_Main--;
  }
#include <Servo.h>
int idx = 1;
long int counter = 0;
const long int ticRatioMainMotor = 6000;
float rel_angle = 0;
volatile long int pos_Main=0;

#define ENCA 3 // YELLOW
#define ENCB 2 // WHITE

Servo   pusherESC;  

#define THROTTLE_MIN        1500
#define THROTTLE_MAX        2000
#define THROTTLE_BRAKE       1000

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  // Attach the relative encoder to the ISR
  pinMode(ENCA,INPUT_PULLUP);
  pinMode(ENCB,INPUT_PULLUP);
  attachInterrupt( digitalPinToInterrupt(ENCA), readEncoder_Main, RISING);
  pusherESC.attach(6);
  pusherESC.writeMicroseconds(THROTTLE_MIN);
  Serial.println("Arming........");   // just some display message 
  delay(2000);
   
}

void loop() {
  // put your main code here, to run repeatedly:
  int tem = map(tem,1,20,1500,2000);
  Serial.println(pos_Main);
  
  
  while(pos_Main<1100){
    analogWrite(5,50);
  }
  analogWrite(5,0);
  delay(2000);
  while(pos_Main<2200){
    analogWrite(5,25);
  }
  analogWrite(5,0);
  
  idx++;
    
  
}



void readEncoder_Main(){
 int b_A = digitalRead(ENCB);
  if(b_A > 0){
    pos_Main++;
  }
  else{
    pos_Main--;
  }
}
