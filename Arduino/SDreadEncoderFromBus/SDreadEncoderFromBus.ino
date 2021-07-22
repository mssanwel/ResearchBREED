#include <Servo.h>
int a=0;
int b=16;

Servo myservo1;  // create servo object to control a servo
Servo myservo2;
// twelve servo objects can be created on most boards

int pos = 0;    // variable to store the servo position

float defaultX=0;
float defaultY=0;
float offX=0;
float offY=0;
int motorA=0;
int motorB=0;
long int timer=0;

// Basic demo for accelerometer, gyro, and magnetometer readings
// from the following Adafruit ST Sensor combo boards:
// * LSM6DSOX + LIS3MDL FeatherWing : https://www.adafruit.com/product/4565
// * ISM330DHCX + LIS3MDL FeatherWing https://www.adafruit.com/product/4569
// * LSM6DSOX + LIS3MDL Breakout : https://www.adafruit.com/product/4517
// * LSM6DS33 + LIS3MDL Breakout Lhttps://www.adafruit.com/product/4485

//#include <Adafruit_LSM6DSOX.h>
//Adafruit_LSM6DSOX lsm6ds;

// To use with the LSM6DS33+LIS3MDL breakout, uncomment these two lines
// and comment out the lines referring to the LSM6DSOX above
//#include <Adafruit_LSM6DS33.h>
//Adafruit_LSM6DS33 lsm6ds;

// To use with the ISM330DHCX+LIS3MDL Feather Wing, uncomment these two lines
// and comment out the lines referring to the LSM6DSOX above
#include <Adafruit_ISM330DHCX.h>
Adafruit_ISM330DHCX lsm6ds;

#include <Adafruit_LIS3MDL.h>
Adafruit_LIS3MDL lis3mdl;

#include <STM32SD.h>

// If SD card slot has no detect pin then define it as SD_DETECT_NONE
// to ignore it. One other option is to call 'SD.begin()' without parameter.
#ifndef SD_DETECT_PIN
#define SD_DETECT_PIN SD_DETECT_NONE
#endif

//Bus
char incomingByte ='0';
String w="";
String char1="";
int val1=0;
String char2="";
int val2=0;
String char3="";
int val3=0;

File dataFile;



void setup(void) {
  
//  while (!Serial)
//    delay(10); // will pause Zero, Leonardo, etc until Serial console opens
//  delay(1000);
//  Serial.begin(9600);
  
  

  
  Serial.print("Initializing SD card...");
  // see if the card is present and can be initialized:
  while (!SD.begin(SD_DETECT_PIN))
  {
    delay(10);
  }
  delay(100);
  Serial.println("card initialized.");

  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.

  char filename[17];
  strcpy(filename, "/datalogFish00.txt");
  for (uint8_t i = 0; i < 100; i++) {
    filename[12] = '0' + i/10;
    filename[13] = '0' + i%10;
    // create if does not exist, do not open existing, write, sync after write
    if (! SD.exists(filename)) {
      break;
    }
  }
  
  dataFile = SD.open(filename, FILE_WRITE);
  // if the file is available, seek to last position
  if (dataFile) {
    dataFile.seek(dataFile.size());
  }
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening datalog.txt");
  }
  if (dataFile) {
    dataFile.println("Start of File");
//    dataFile.print(String(defaultX));
//    dataFile.print(",");
//    dataFile.println(String(defaultY));
    dataFile.flush(); // use flush to ensure the data written
    // print to the serial port too:
  }
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);

  Wire.begin(4);                // join i2c bus with address #4
  Wire.onReceive(receiveEvent); // register event
}

void loop() {

  
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  
  //  /* Get new normalized sensor events */
  

   // make a string for assembling the data to log:
  String dataString = "";
  
//  char1 = String(w[0]);
//  val1 = String(w.substring(1, 2)).toInt();
//  char2 = String(w[2]);
//  val2 = String(w.substring(3, 4)).toInt();
//  char3 = String(w[4]);
//  val3 = String(w.substring(5, w.length()-1)).toInt();
  
  
  // read sensors and append to the string:
  timer=millis();
  dataString += String(timer);
  dataString += ",";
  dataString += String(w);
  dataString += ",";

  
  digitalWrite(LED_BUILTIN, HIGH);    // turn the LED off by making the voltage LOW
  
  // if the file is available, write to it:  
  if (dataFile) {
    dataFile.println(dataString);
    dataFile.flush(); // use flush to ensure the data written
    // print to the serial port too:
    Serial.println(dataString);
  }
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error on datalog.txt file handle");
  }
}


void receiveEvent(int howMany)
{
  char cmd[12];    //to store the signal from transmitter
  int siglen = 0;  //to store the length of the incoming signal
  while(0 < Wire.available()) // loop through all but the last
  {
    char c = Wire.read(); // receive byte as a character
    Serial.print(c);         // print the character
    cmd[siglen] = c;
    siglen++;
  }
  cmd[siglen]='\0';
  w = String(cmd);
}
