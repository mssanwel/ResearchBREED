/* This code sets the motor to spin at a fix speed and measures 


1) Rotation Count
2) Displays the current position 
3) Displays the speed of the motor
*/

long int counter = 0;
const long int ticRatioMainMotor = 6000;
float rel_angle = 0;
volatile long int pos_Main=0;

#define ENCA 3 // YELLOW
#define ENCB 2 // WHITE

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  // Attach the relative encoder to the ISR
  pinMode(ENCA,INPUT_PULLUP);
  pinMode(ENCB,INPUT_PULLUP);
  attachInterrupt( digitalPinToInterrupt(ENCA), readEncoder_Main, RISING);
   
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println(pos_Main);
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
