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
//char encoderValue[6];
String encoderValue="";

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
 // delay(3);
  while (!Serial)
    delay(10); // will pause Zero, Leonardo, etc until Serial console opens
  delay(1000);
  Serial.begin(9600);
  
  Serial.println("Adafruit LSM6DS+LIS3MDL test!");

  bool lsm6ds_success, lis3mdl_success;

  // hardware I2C mode, can pass in address & alt Wire

//  lsm6ds_success = lsm6ds.begin_I2C(0x6B);
//  lis3mdl_success = lis3mdl.begin_I2C(0x1E);
  lsm6ds_success = lsm6ds.begin_I2C(0x6A);
  lis3mdl_success = lis3mdl.begin_I2C(0x1C);

  if (!lsm6ds_success){
    Serial.println("Failed to find LSM6DS chip");
  }
  if (!lis3mdl_success){
    Serial.println("Failed to find LIS3MDL chip");
  }
  if (!(lsm6ds_success && lis3mdl_success)) {
    while (1) {
      delay(10);
    }
  }

  Serial.println("LSM6DS and LIS3MDL Found!");

  // lsm6ds.setAccelRange(LSM6DS_ACCEL_RANGE_2_G);
  Serial.print("Accelerometer range set to: ");
  switch (lsm6ds.getAccelRange()) {
  case LSM6DS_ACCEL_RANGE_2_G:
    Serial.println("+-2G");
    break;
  case LSM6DS_ACCEL_RANGE_4_G:
    Serial.println("+-4G");
    break;
  case LSM6DS_ACCEL_RANGE_8_G:
    Serial.println("+-8G");
    break;
  case LSM6DS_ACCEL_RANGE_16_G:
    Serial.println("+-16G");
    break;
  }

  // lsm6ds.setAccelDataRate(LSM6DS_RATE_12_5_HZ);
  Serial.print("Accelerometer data rate set to: ");
  switch (lsm6ds.getAccelDataRate()) {
  case LSM6DS_RATE_SHUTDOWN:
    Serial.println("0 Hz");
    break;
  case LSM6DS_RATE_12_5_HZ:
    Serial.println("12.5 Hz");
    break;
  case LSM6DS_RATE_26_HZ:
    Serial.println("26 Hz");
    break;
  case LSM6DS_RATE_52_HZ:
    Serial.println("52 Hz");
    break;
  case LSM6DS_RATE_104_HZ:
    Serial.println("104 Hz");
    break;
  case LSM6DS_RATE_208_HZ:
    Serial.println("208 Hz");
    break;
  case LSM6DS_RATE_416_HZ:
    Serial.println("416 Hz");
    break;
  case LSM6DS_RATE_833_HZ:
    Serial.println("833 Hz");
    break;
  case LSM6DS_RATE_1_66K_HZ:
    Serial.println("1.66 KHz");
    break;
  case LSM6DS_RATE_3_33K_HZ:
    Serial.println("3.33 KHz");
    break;
  case LSM6DS_RATE_6_66K_HZ:
    Serial.println("6.66 KHz");
    break;
  }

  // lsm6ds.setGyroRange(LSM6DS_GYRO_RANGE_250_DPS );
  Serial.print("Gyro range set to: ");
  switch (lsm6ds.getGyroRange()) {
  case LSM6DS_GYRO_RANGE_125_DPS:
    Serial.println("125 degrees/s");
    break;
  case LSM6DS_GYRO_RANGE_250_DPS:
    Serial.println("250 degrees/s");
    break;
  case LSM6DS_GYRO_RANGE_500_DPS:
    Serial.println("500 degrees/s");
    break;
  case LSM6DS_GYRO_RANGE_1000_DPS:
    Serial.println("1000 degrees/s");
    break;
  case LSM6DS_GYRO_RANGE_2000_DPS:
    Serial.println("2000 degrees/s");
    break;
  case ISM330DHCX_GYRO_RANGE_4000_DPS:
    Serial.println("4000 degrees/s");
    break;
  }
  // lsm6ds.setGyroDataRate(LSM6DS_RATE_12_5_HZ);
  Serial.print("Gyro data rate set to: ");
  switch (lsm6ds.getGyroDataRate()) {
  case LSM6DS_RATE_SHUTDOWN:
    Serial.println("0 Hz");
    break;
  case LSM6DS_RATE_12_5_HZ:
    Serial.println("12.5 Hz");
    break;
  case LSM6DS_RATE_26_HZ:
    Serial.println("26 Hz");
    break;
  case LSM6DS_RATE_52_HZ:
    Serial.println("52 Hz");
    break;
  case LSM6DS_RATE_104_HZ:
    Serial.println("104 Hz");
    break;
  case LSM6DS_RATE_208_HZ:
    Serial.println("208 Hz");
    break;
  case LSM6DS_RATE_416_HZ:
    Serial.println("416 Hz");
    break;
  case LSM6DS_RATE_833_HZ:
    Serial.println("833 Hz");
    break;
  case LSM6DS_RATE_1_66K_HZ:
    Serial.println("1.66 KHz");
    break;
  case LSM6DS_RATE_3_33K_HZ:
    Serial.println("3.33 KHz");
    break;
  case LSM6DS_RATE_6_66K_HZ:
    Serial.println("6.66 KHz");
    break;
  }

  lis3mdl.setDataRate(LIS3MDL_DATARATE_155_HZ);
  // You can check the datarate by looking at the frequency of the DRDY pin
  Serial.print("Magnetometer data rate set to: ");
  switch (lis3mdl.getDataRate()) {
    case LIS3MDL_DATARATE_0_625_HZ: Serial.println("0.625 Hz"); break;
    case LIS3MDL_DATARATE_1_25_HZ: Serial.println("1.25 Hz"); break;
    case LIS3MDL_DATARATE_2_5_HZ: Serial.println("2.5 Hz"); break;
    case LIS3MDL_DATARATE_5_HZ: Serial.println("5 Hz"); break;
    case LIS3MDL_DATARATE_10_HZ: Serial.println("10 Hz"); break;
    case LIS3MDL_DATARATE_20_HZ: Serial.println("20 Hz"); break;
    case LIS3MDL_DATARATE_40_HZ: Serial.println("40 Hz"); break;
    case LIS3MDL_DATARATE_80_HZ: Serial.println("80 Hz"); break;
    case LIS3MDL_DATARATE_155_HZ: Serial.println("155 Hz"); break;
    case LIS3MDL_DATARATE_300_HZ: Serial.println("300 Hz"); break;
    case LIS3MDL_DATARATE_560_HZ: Serial.println("560 Hz"); break;
    case LIS3MDL_DATARATE_1000_HZ: Serial.println("1000 Hz"); break;
  }

  lis3mdl.setRange(LIS3MDL_RANGE_4_GAUSS);
  Serial.print("Range set to: ");
  switch (lis3mdl.getRange()) {
    case LIS3MDL_RANGE_4_GAUSS: Serial.println("+-4 gauss"); break;
    case LIS3MDL_RANGE_8_GAUSS: Serial.println("+-8 gauss"); break;
    case LIS3MDL_RANGE_12_GAUSS: Serial.println("+-12 gauss"); break;
    case LIS3MDL_RANGE_16_GAUSS: Serial.println("+-16 gauss"); break;
  }

  lis3mdl.setPerformanceMode(LIS3MDL_MEDIUMMODE);
  Serial.print("Magnetometer performance mode set to: ");
  switch (lis3mdl.getPerformanceMode()) {
    case LIS3MDL_LOWPOWERMODE: Serial.println("Low"); break;
    case LIS3MDL_MEDIUMMODE: Serial.println("Medium"); break;
    case LIS3MDL_HIGHMODE: Serial.println("High"); break;
    case LIS3MDL_ULTRAHIGHMODE: Serial.println("Ultra-High"); break;
  }

  lis3mdl.setOperationMode(LIS3MDL_CONTINUOUSMODE);
  Serial.print("Magnetometer operation mode set to: ");
  // Single shot mode will complete conversion and go into power down
  switch (lis3mdl.getOperationMode()) {
    case LIS3MDL_CONTINUOUSMODE: Serial.println("Continuous"); break;
    case LIS3MDL_SINGLEMODE: Serial.println("Single mode"); break;
    case LIS3MDL_POWERDOWNMODE: Serial.println("Power-down"); break;
  }

  lis3mdl.setIntThreshold(500);
  lis3mdl.configInterrupt(false, false, true, // enable z axis
                          true, // polarity
                          false, // don't latch
                          true); // enabled!


  myservo1.attach(10);  // attaches the servo on pin 9 to the servo object
  myservo2.attach(6);
  
  sensors_event_t accel, gyro, mag, temp;
  lsm6ds.getEvent(&accel, &gyro, &temp);
  defaultX=accel.acceleration.x;
  defaultY=accel.acceleration.y;

  pinMode(a, OUTPUT); //IN2
  pinMode(b, OUTPUT); //IN1


  // Open Serial communications and wait for port to open:
  //Serial.begin(9600);
  

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

  char filename[15];
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

  Wire.begin();                // join i2c bus as master
}

void loop() {

  
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  
  //  /* Get new normalized sensor events */
  
  sensors_event_t accel, gyro, mag, temp;
  lsm6ds.getEvent(&accel, &gyro, &temp);
  lis3mdl.getEvent(&mag);



  //Bus master receiver
  Wire.requestFrom(8, 10);    // request 6 bytes from slave device #8
  char cmd[10];    //to store the signal from transmitter
  int siglen = 0;  //to store the length of the incoming signal
  int endCnt=0;
  while (Wire.available()) { // slave may send less than requested
    char incomingByte = Wire.read();
    if (incomingByte=='?'){
      endCnt=siglen;
    }
    cmd[siglen] = incomingByte;
    siglen++;
  }
  cmd[9]='\0';
  //char cmd2[8];
  //memcpy(cmd, &cmd[0], endCnt*sizeof(*cmd));
  //strncpy ( encoderValue, cmd, endCnt );
  //encoderValue[endCnt]='\0';
  encoderValue=cmd;
  Serial.println(encoderValue);

// Wire.requestFrom(4, 6);    // request 6 bytes from slave device #8
//
//  while (Wire.available()) { // slave may send less than requested
//    char c = Wire.read(); // receive a byte as character
//    Serial.print(c);         // print the character
//  }
//  
//  Wire.requestFrom(4, 30);    // request 6 bytes from slave device #8
//  char cmd2[30];    //to store the signal from transmitter
//  int siglen2 = 0;  //to store the length of the incoming signal
//  int endCnt2=0;
//  while (Wire.available()) { // slave may send less than requested
//    char incomingByte2 = Wire.read();
//    if (incomingByte2=='?'){
//      endCnt2=siglen2;
//    }
//    cmd2[siglen2] = incomingByte2;
//    siglen2++;
//  }
//  cmd2[30]='\0';
//  //char cmd2[8];
//  //memcpy(cmd, &cmd[0], endCnt*sizeof(*cmd));
//  //strncpy ( encoderValue, cmd, endCnt );
//  //encoderValue[endCnt]='\0';
//  //encoderValue=cmd;
//  Serial.println(cmd2);

  

//  Wire.requestFrom(8, 6);    // request 6 bytes from slave device #8
//
//  while (Wire.available()) { // slave may send less than requested
//    incomingByte = Wire.read();
//    cmd[siglen] = incomingByte;
//    siglen++;
//    Serial.print(incomingByte);
//  }

  // make a string for assembling the data to log:
  String dataString = "";
  
  // read sensors and append to the string:
  timer=millis();
  dataString += String(timer);
  dataString += ",";
  dataString += String(accel.acceleration.x);
  dataString += ",";
  dataString += String(accel.acceleration.y);
  dataString += ",";
  dataString += String(accel.acceleration.z);
  dataString += ",";
  dataString += String(gyro.gyro.x);
  dataString += ",";
  dataString += String(gyro.gyro.y);
  dataString += ",";
  dataString += String(gyro.gyro.z);
  dataString += ",";
  dataString += String(mag.magnetic.x);
  dataString += ",";
  dataString += String(mag.magnetic.y);
  dataString += ",";
  dataString += String(mag.magnetic.z);
  dataString += ",";
  dataString += String(encoderValue);
  dataString += ",";
  
  
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW

  
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
