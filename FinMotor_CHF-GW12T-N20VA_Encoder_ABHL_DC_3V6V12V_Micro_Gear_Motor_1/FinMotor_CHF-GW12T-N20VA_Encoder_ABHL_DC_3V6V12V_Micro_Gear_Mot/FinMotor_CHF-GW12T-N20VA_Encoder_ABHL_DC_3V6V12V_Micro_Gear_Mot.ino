/*     Arduino Rotary Encoder Tutorial
 *      
 *  by Dejan Nedelkovski, www.HowToMechatronics.com
 *  
 */
 
 #define outputA 2
 #define outputB 7

 #define m1 5
 #define m2 6
 volatile int counter = 0; 
 volatile int aState;
 volatile int aLastState;  
 volatile int dest=50;
 String readString;

 void setup() { 
   pinMode(outputA, INPUT); //set as input
   digitalWrite(outputA, HIGH);//enable internal pullup resistor
   attachInterrupt(digitalPinToInterrupt(outputA), interruptName, RISING);//Interrupt initialization
   pinMode (outputB,INPUT);
   pinMode (m1,OUTPUT);
   pinMode (m2,OUTPUT);
   
   Serial.begin (9600);
   // Reads the initial state of the outputA
   aLastState = digitalRead(outputA);   
 } 



void interruptName(){
  Serial.println("Int");
  aState = digitalRead(outputA); // Reads the "current" state of the outputA
   // If the previous and the current state of the outputA are different, that means a Pulse has occured

   Serial.println(aState);
   Serial.println(aLastState);
   //if (aState == aLastState)
   {     
     // If the outputB state is different to the outputA state, that means the encoder is rotating clockwise
     if (digitalRead(outputB) != aState) { 
       counter ++;
     } else {
       counter --;
     }
     Serial.print("Position: ");
     Serial.println(counter);
   } 
   aLastState = aState; // Updates the previous state of the outputA with the current state

   if (counter>dest+1){
     analogWrite(m1, 100);
     analogWrite(m2, 0);
     Serial.println("Below");
   }
   else if ((counter<dest-1)){
     analogWrite(m1, 0);
     analogWrite(m2, 100);
     Serial.println("Above");
   }
   else
   {
     analogWrite(m1, 0);
     analogWrite(m2, 0);
     Serial.println("Reached");
   }
}



 void loop() { 
  while (Serial.available()) {
    char c = Serial.read();  //gets one byte from serial buffer
    readString += c; //makes the string readString
    delay(2);  //slow looping to allow buffer to fill with next character
  }
  if (readString.length() >0) {
    int dest = readString.toInt(); 
    Serial.print("======================================New Dest Value:"); 
    Serial.println(dest);
    readString="";
  }
 }
