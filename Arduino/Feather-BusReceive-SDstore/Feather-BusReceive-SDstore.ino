#include <Wire.h>
int a=0;
int b=16;

// twelve servo objects can be created on most boards

int pos = 0;    // variable to store the servo position

float defaultX=0;
float defaultY=0;
float offX=0;
float offY=0;
int motorA=0;
int motorB=0;

char c[100];
  


long int timer=0;
//char encoderValue[6];
String encoderValue="";




// make a string for assembling the data to log:------------------------------------------------------------
  String dataString = "";


//Bus
char incomingByte ='0';
String w="";
String char1="";
int val1=0;
String char2="";
int val2=0;
String char3="";
int val3=0;





void setup(void) {
// delay(3);
  while (!Serial)
    delay(10); // will pause Zero, Leonardo, etc until Serial console opens
  delay(1000);
  Serial.begin(9600);



  // Open Serial communications and wait for port to open:
  //Serial.begin(9600);
  

  
  
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);

  

  Wire.begin(4);                // join i2c bus with address #4
  Wire.onReceive(receiveEvent); // register event
}

void loop() {

  //Save Data 
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW

  
  
}

void receiveEvent(int howMany)
{
  long int siglen=0;
  while(c[siglen]!='?') // loop through all but the last
  {
    if (Wire.available()){
      c[siglen] = Wire.read(); // receive byte as a character
      Serial.print(c[siglen]);
      siglen+=1;
    }
  }
  c[siglen]='\0';
  Serial.println(c);
  //dataString=String(c);
}
