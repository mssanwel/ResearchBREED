/*
 * Fish encoder + servo fins (transmitter code)
 *
 * Transmits 4 signals:
 *      servo: up-down, left-right
 *      tail: turnValue, power
 *
 * Edited by: Tasnia, Brianna, Heidi, Allison, Saad, Taha
 *
 */

// Timers
int timer1 = 0;       //store the time when transmiting
int timer2 = 0;       //store the time when sending T signals
int timer3 = 0;       //store the time when pressing left-right buttons
int timer4 = 0;       //store the time when pressing up-down buttons
int timer5 = 0;       //store the time when sending P signals
int bounceDelay=350;  // delay to avoid bounce on switch

//Variables for transmission
//signal1:R;     signal2:U;     signal3:T;      signal4:P
String sum_str;
char message[20];
int ascii_sum;     //checkSum value

//declare Funduino buttons
int leftbutton = 5;           
int rightbutton = 3;
int upbutton = 2;
int downbutton = 4;

//Button default values
int xMap, yMap;
int tMap = 5;         //default value of turnVal
int pMap = 5;         //default value of power

//declare Funduino joystick
int sensorValueX = analogRead(A0);    
int sensorValueY = analogRead(A1);

void setup() {
  // Setup Serial
  Serial.begin(9600);
  Serial1.begin(9600, SERIAL_8O1);
  while (!Serial.read());
  while (!Serial1.read()); // Wait for serial to setup

  //Setup Funduino pins
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(leftbutton,INPUT_PULLUP);
  pinMode(rightbutton,INPUT_PULLUP);
  pinMode(upbutton,INPUT_PULLUP);
  pinMode(downbutton,INPUT_PULLUP);
}



void loop() {


  //Update control variables from buttons
  //Reading Joystick Value
  sensorValueX = analogRead(A0);
  sensorValueY = analogRead(A1);
  xMap = map(sensorValueX, 0, 1023, 0, 9);
  yMap = map(sensorValueY, 0, 1023, 0, 9);


  //Reading the left, right, up and down buttons
  if((digitalRead(leftbutton) == LOW) && tMap > 1) {
    if (millis()-timer3 > bounceDelay){
      tMap--;
      Serial.println("button left");
      timer3 = millis();
    }
  }
  else if ((digitalRead(rightbutton)== LOW) && tMap < 9) {
    if (millis()-timer3 > bounceDelay){
      tMap++;
      Serial.println("button right");
      timer3 = millis();
    }
  }
  else if((digitalRead(downbutton) == LOW) && pMap > 0) {
    if (millis()-timer4 > bounceDelay){
      pMap--;
      Serial.println("button down");
      timer4 = millis();
    }
  }
  else if ((digitalRead(upbutton)== LOW) && pMap < 9) {
    if (millis()-timer4 > bounceDelay){
      pMap++;
      Serial.println("button up");
      timer4 = millis();
    }
  }
  
  // Send signal after every 200 ms
  if (millis() - timer1 > 200) {
    timer1=millis(); //update timer1

    // Prepare string and send to receiver
    ascii_sum = 'R' + (xMap + '0')+'U' + (yMap + '0')+'T' + (tMap + '0')+'P' + (pMap + '0'); // Checksum
    sum_str = "R" + String(xMap) +"U"+ String(yMap)+"T" + String(tMap) +"P"+ String(pMap) + String(ascii_sum) + "?"; //payload + checksum
    strcpy(message,sum_str.c_str());
    Serial1.write(message);
    
    Serial.print("Signal: ");
    Serial.println(message);
    //blocking function, no need to wait for delay if use flush
    Serial1.flush();
  }
}
