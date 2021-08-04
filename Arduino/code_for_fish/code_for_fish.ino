
#define ENCA_A 18 // YELLOW
#define ENCB_A 19 // WHITE
#define PWM_A 5
#define IN2_A 6
#define IN1_A 7

#define ENCA 20 // YELLOW
#define ENCB 21 // WHITE
#define PWM 8
#define IN2 9
#define IN1 10

#define  motor1pin1 23
#define  motor1pin2 22

int pos_A = 0;
long prevT_A = 0;
float eprev_A = 0;
float eintegral_A = 0;

int pos = 0;
long prevT = 0;
float eprev = 0;
float eintegral = 0;

unsigned long startMillis;
unsigned long currentMillis;
int target_A;
int target;
void setup() {
  Serial.begin(9600);
  pinMode(ENCA_A,INPUT);
  pinMode(ENCB_A,INPUT);
  attachInterrupt(digitalPinToInterrupt(ENCA_A),readEncoder_A,RISING);

  pinMode(ENCA,INPUT);
  pinMode(ENCB,INPUT);
  attachInterrupt(digitalPinToInterrupt(ENCA),readEncoder,RISING);
  
  pinMode(PWM_A,OUTPUT);
  pinMode(IN1_A,OUTPUT);
  pinMode(IN2_A,OUTPUT);

  pinMode(PWM,OUTPUT);
  pinMode(IN1,OUTPUT);
  pinMode(IN2,OUTPUT);


  pinMode(motor1pin1, OUTPUT);
  pinMode(motor1pin2, OUTPUT);


  Serial.println("target pos_A");
  Serial.println("target pos");
  startMillis = millis();
}

void loop() {

  digitalWrite(motor1pin1, HIGH);
  digitalWrite(motor1pin2, LOW);
  currentMillis = millis();

  // set target position
  //int target = 20 degree;
  //for 360 needs 4221 ticks

  if (currentMillis - startMillis >= 10*1000){
    target =  -1000;
    target_A =  -1000;
    }
    else{
      target =  1000;
      target_A =  1000;
      }

  // PID constants
  float kp_A = 2;
  float kd_A = 0.025;
  float ki_A = 0.0;

  float kp = 5;
  float kd = 0.025;
  float ki = 0.0;

  // time difference
  long currT_A = micros();
  float deltaT_A = ((float) (currT_A - prevT_A))/( 1.0e6 );
  prevT_A = currT_A;

  long currT = micros();
  float deltaT = ((float) (currT - prevT))/( 1.0e6 );
  prevT = currT;
  
  // error
  int e_A = pos_A - target_A;
  
  int e = pos - target;

  // derivative
  float dedt_A = (e_A-eprev_A)/(deltaT_A);

  float dedt = (e-eprev)/(deltaT);

  // integral
  eintegral_A = eintegral_A + e_A*deltaT_A;

  eintegral = eintegral + e*deltaT;

  // control signal
  float u_A = kp_A*e_A + kd_A*dedt_A + ki_A*eintegral_A;

  float u = kp*e + kd*dedt + ki*eintegral;

  // motor power
  float pwr_A = fabs(u_A);
  if( pwr_A > 255 ){
    pwr_A = 255;
  }
  if( pwr_A < 50 ){
    pwr_A = 0;
  }


  float pwr = fabs(u);
  if( pwr > 255 ){
    pwr = 255;
  }
  if( pwr < 50 ){
    pwr = 0;
  }


  // motor direction
  int dir_A = 1;
  if(u_A<0){
    dir_A = -1;
  }
  
  int dir = 1;
  if(u<0){
    dir = -1;
  }

  // signal the motor
  setMotor(dir_A,pwr_A,PWM_A,IN1_A,IN2_A);
  
  setMotor(dir,pwr,PWM,IN1,IN2);


  // store previous error
  eprev_A = e_A;
  eprev = e;

  Serial.print(target_A);
  Serial.print(" ");
  Serial.print(pos_A);
  Serial.print(" ");
  Serial.print(target);
  Serial.print(" ");
  Serial.print(pos);
  
  Serial.println();
}

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

void readEncoder(){
  int b = digitalRead(ENCB);
  if(b > 0){
    pos++;
  }
  else{
    pos--;
  }
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
