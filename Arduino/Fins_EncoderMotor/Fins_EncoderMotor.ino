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
float kd_A = 0;
float ki_A = 0.1;

float kp_B = 0.5;
float kd_B = 0;
float ki_B = 0.1; 


  
void setup() {
  Serial.begin(9600);
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


}


void loop() {
  DriveStright(4000,4000); // stright for 3 meters
  delay(1000*1);
  Serial.print("the position is set");
  Serial.print(pos_A);
  Serial.println(pos_B);
  DriveStright(-4000,-4000);
  delay(1000*1);
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


void DriveStright (int target_a, int target_b){

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
     setMotor(dir_B,pwr_B,PWM_B,IN1_B,IN2_B);

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

     if (fabs(e_A) < 50 and fabs(e_B) < 50 ){  
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
