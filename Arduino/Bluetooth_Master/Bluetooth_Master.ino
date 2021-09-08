[14:48, 08/09/2021] Pallavi Goel BREED HKU: //this is master

#include <SoftwareSerial.h>

SoftwareSerial BTSerial(10, 11);

String state="";
const int ledPin = 8;
const int buttonPin = 4;
int buttonState = 1;

void setup() 
{
  Serial.begin(9600);
  BTSerial.begin(38400);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  pinMode(buttonPin, INPUT);
  digitalWrite(buttonPin, HIGH);
}

void loop()
{
 // Controlling the LED
 buttonState = digitalRead(buttonPin);
 if (buttonState == LOW)
 {
  Serial.println("Sending signal to slave to get MPU value");
  BTSerial.write('1');
  
  if(BTSerial.available() > 0)
  {
    Serial.println("Getting value");// Checks whether data is comming from the serial port
    while(BTSerial.available()!='?')
    {
      Serial.println("hey");
      state = state + BTSerial.read(); // Reads the data from the serial port 
      }
    Serial.println(state);
    }
 }

// //Receiving values
// if (state != "") 
// {
//  digitalWrite(ledPin, HIGH); // LED ON
//  Serial.println("Receiving signal from slave");
//  Serial.println(state);
//  String results = state.Split(',');
//  // print out data
//  Serial.print("aX = "); Serial.print(results[0]);
//  Serial.print(" | aY = "); Serial.print(results[1]);
//  Serial.print(" | aZ = "); Serial.print(results(rstr[2]));
//  // the following equation was taken from the documentation [MPU-6000/MPU-6050 Register Map and Description, p.30]
//  Serial.print(" | tmp = "); Serial.print(convert_str_to_int16(results[3]/340.00+36.53));
//  Serial.print(" | gX = "); Serial.print(results[4]);
//  Serial.print(" | gY = "); Serial.print(results[5]);
//  Serial.print(" | gZ = "); Serial.print(results[6]);
//  Serial.println();
//  } 
  state = "";
}
