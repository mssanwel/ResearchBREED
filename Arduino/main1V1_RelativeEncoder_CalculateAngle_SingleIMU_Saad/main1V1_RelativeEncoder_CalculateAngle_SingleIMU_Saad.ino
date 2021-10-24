//#include <SPI.h>
//#include "SdFat.h"
#include <Servo.h>
//SdFat sd;
int a=0;
int b=16;
//
char message[100];

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
String data="";


//For calculating Euler Angles
#include <Adafruit_Sensor_Calibration.h>
#include <Adafruit_AHRS.h>
// pick your filter! slower == better quality output
//Adafruit_NXPSensorFusion filter; // slowest
//Adafruit_Madgwick filter;  // faster than NXP
Adafruit_Mahony filter;  // fastest/smalleset
#if defined(ADAFRUIT_SENSOR_CALIBRATION_USE_EEPROM)
  Adafruit_Sensor_Calibration_EEPROM cal;
#else
  Adafruit_Sensor_Calibration_SDFat cal;
#endif
boolean initialization=true;
float defaultRoll, defaultPitch;
#define FILTER_UPDATE_RATE_HZ 150
#define PRINT_EVERY_N_UPDATES 10
//#define AHRS_DEBUG_OUTPUT
uint32_t timestamp;
float gx, gy, gz;


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
Adafruit_ISM330DHCX lsm6ds2;

#include <Adafruit_LIS3MDL.h>
Adafruit_LIS3MDL lis3mdl;
Adafruit_LIS3MDL lis3mdl2;


//Bus
char incomingByte ='0';
String w="";
String char1="";
int val1=0;
String char2="";
int val2=0;
String char3="";
int val3=0;



//Relative Encoder Main Motor
//
#define ENCA 10 // YELLOW
#define ENCB 9 // WHITE
volatile int pos_Main = 0;
int ticRatioMainMotor=500; //Number of tic per revolution of the main motor. Implemnted to use the relative encoder as an absolute encoder temporarily. 


//Neopixel indicator
#include <Adafruit_NeoPixel.h>    //  Library that provides NeoPixel functions

// -- Create a NeoPixel object called onePixel that addresses 1 pixel in pin 8
Adafruit_NeoPixel onePixel = Adafruit_NeoPixel(1, 8, NEO_GRB + NEO_KHZ800);


void setup(void) {
// // delay(3);
  while (!Serial)
    delay(10); // will pause Zero, Leonardo, etc until Serial console opens
  delay(1000);
  Serial.begin(9600);

  //Neopixel indicator
  onePixel.begin();             // Start the NeoPixel object
  onePixel.clear();             // Set NeoPixel color to black (0,0,0)
  onePixel.setBrightness(20);   // Affects all subsequent settings
  onePixel.show();              // Update the pixel state
  
  //Interrupt pin pullup relative encoder
  pinMode(ENCA,INPUT_PULLUP);
  //pinMode(ENCB,INPUT_PULLUP);
  
  Serial.println("Adafruit LSM6DS+LIS3MDL test!");

  bool lsm6ds_success, lis3mdl_success;
  bool lsm6ds_success2, lis3mdl_success2;
  
  
// hardware I2C mode, can pass in address & alt Wire
//
  lsm6ds_success = lsm6ds.begin_I2C(0x6B);
  lis3mdl_success = lis3mdl.begin_I2C(0x1E);
//  lsm6ds_success = lsm6ds.begin_I2C(0x6A);
//  lis3mdl_success = lis3mdl.begin_I2C(0x1C);

  if (!lsm6ds_success){
    Serial.println("Failed to find LSM6DS chip");
  //Neopixel indicator
  int r=0, g=0, b=200;    //  Red, green and blue intensity to display
  onePixel.setPixelColor(0, r, g, b);   //  Set pixel 0 to (r,g,b) color value
  onePixel.show();                      //  Update pixel state
  }
  if (!lis3mdl_success){
    Serial.println("Failed to find LIS3MDL chip");
    //Neopixel indicator
    int r=0, g=0, b=200;    //  Red, green and blue intensity to display
    onePixel.setPixelColor(0, r, g, b);   //  Set pixel 0 to (r,g,b) color value
    onePixel.show();                      //  Update pixel state
  }
  if (!(lsm6ds_success && lis3mdl_success)) {
    while (1) {
      delay(10);
    }
  }

  Serial.println("LSM6DS and LIS3MDL Found!");
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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

  // set lowest range
  lsm6ds.setAccelRange(LSM6DS_ACCEL_RANGE_2_G);
  lsm6ds.setGyroRange(LSM6DS_GYRO_RANGE_250_DPS);
  lis3mdl.setRange(LIS3MDL_RANGE_4_GAUSS);

  // set slightly above refresh rate
  lsm6ds.setAccelDataRate(LSM6DS_RATE_104_HZ);
  lsm6ds.setGyroDataRate(LSM6DS_RATE_104_HZ);
  lis3mdl.setDataRate(LIS3MDL_DATARATE_1000_HZ);
  lis3mdl.setPerformanceMode(LIS3MDL_MEDIUMMODE);
  lis3mdl.setOperationMode(LIS3MDL_CONTINUOUSMODE);

//  if (!cal.begin()) {
//    Serial.println("Failed to initialize calibration helper");
//  } else if (! cal.loadCalibration()) {
//    Serial.println("No calibration loaded/found");
//  }
//  filter.begin(FILTER_UPDATE_RATE_HZ);
//  timestamp = millis();
//
//  Wire.setClock(400000); // 400KHz


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  
  myservo1.attach(6);  // attaches the servo on pin 9 to the servo object
  myservo2.attach(9);
  
  sensors_event_t accel, gyro, mag, temp;
  lsm6ds.getEvent(&accel, &gyro, &temp);
  
  
  defaultX=accel.acceleration.x;
  defaultY=accel.acceleration.y;

  pinMode(a, OUTPUT); //IN2
  pinMode(b, OUTPUT); //IN1


  // Open Serial communications and wait for port to open:
  //Serial.begin(9600);
  

  


  //Main motor relative encoder setup
  attachInterrupt( digitalPinToInterrupt(ENCA), readEncoder_Main, RISING);

  
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);

  //EulerAngle Calculation
  filter.begin(FILTER_UPDATE_RATE_HZ);
  timestamp = millis();
  
  

  Wire.begin();                // join i2c bus as master
  //Wire.setClock(400000); // 400KHz
}

void loop() {
//
//  if ((millis() - timestamp) < (1000 / FILTER_UPDATE_RATE_HZ)) {
//    return;
//  }
//  timestamp = millis();
  
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  //EulerAngleFilter
  float roll, pitch, heading;
  
  //IMU1------------------------------------------------------------------------------------------------------
  //  /* Get new normalized sensor events */
  
  sensors_event_t accel, gyro, mag, temp;
  lsm6ds.getEvent(&accel, &gyro, &temp);
  lis3mdl.getEvent(&mag);



  // Bus master receiver -------------------------------------------------------------------------------------
  int b=14;
//  Wire.requestFrom(8, b);    // request 6 bytes from slave device #8
//  char cmd[b];    //to store the signal from transmitter
//  int siglen = 0;  //to store the length of the incoming signal
//  int endCnt=0;
//  while (Wire.available()) { // slave may send less than requested
//    char incomingByte = Wire.read();
//    if (incomingByte=='?'){
//      endCnt=siglen;
//    }
//    cmd[siglen] = incomingByte;
//    siglen++;
//  }
//  cmd[b]='\0';
//  //char cmd2[8];
//  //memcpy(cmd, &cmd[0], endCnt*sizeof(*cmd));
//  //strncpy ( encoderValue, cmd, endCnt );
//  //encoderValue[endCnt]='\0';
//  encoderValue=cmd;
//  //Serial.println(encoderValue);

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

  
//
//  Wire.requestFrom(8, 6);    // request 6 bytes from slave device #8
//
//  while (Wire.available()) { // slave may send less than requested
//    incomingByte = Wire.read();
//    cmd[siglen] = incomingByte;
//    siglen++;
//    Serial.print(incomingByte);
//  }



//  //Calibration-----------------------------------------------------------------------------------------------
//  cal.calibrate(mag);
//  cal.calibrate(accel);
//  cal.calibrate(gyro);
  
  // Gyroscope needs to be converted from Rad/s to Degree/s
  // the rest are not unit-important
  gx = gyro.gyro.x * SENSORS_RADS_TO_DPS;
  gy = gyro.gyro.y * SENSORS_RADS_TO_DPS;
  gz = gyro.gyro.z * SENSORS_RADS_TO_DPS;

  // Update the SensorFusion filter
  filter.update(gx, gy, gz, 
                accel.acceleration.x, accel.acceleration.y, accel.acceleration.z, 
                mag.magnetic.x, mag.magnetic.y, mag.magnetic.z);




  int r1=0, g1=0, b1=0;    //  Red, green and blue intensity to display
  onePixel.setPixelColor(0, r1, g1, b1);   //  Set pixel 0 to (r,g,b) color value
  onePixel.show();  
  // print the heading, pitch and roll
  roll = filter.getRoll();
  pitch = filter.getPitch();
  heading = filter.getYaw();
  if (initialization and millis()>1500){
    initialization=false;
    defaultRoll = 0;//filter.getYaw();
    defaultPitch = 0;//filter.getPitch();
  }
  else{
    Serial.print("Default values are");
    Serial.print(defaultRoll);
    Serial.print(", ");
    Serial.println(defaultPitch);
  }
  Serial.print("Orientation: ");
  Serial.print(heading);
  Serial.print(", ");
  Serial.print(pitch);
  Serial.print(", ");
  Serial.println(roll);


 //For Control signals input 
//  int count=0;
//  char finCommand[3];
//  finCommand[3]='\0';
//  int finCommandCount=0;
//  Serial.println(cmd);
//  while (count<b){
//    //Serial.println(cmd[count]);
//    if (cmd[count]==','){
//      finCommand[finCommandCount]=cmd[count+1];
//      //Serial.println(finCommand[finCommandCount]);
//      finCommandCount=finCommandCount+1;
//    }
//    count+=1;
//  }


//  float rFin=String(finCommand[0]).toInt();
//  float uFin=String(finCommand[1]).toInt();

  //PID implementation
  float s1=0, s2=0;
  float maxAttacAngle=40;
  
//  s1= ((4-rFin)*maxAttacAngle/5 + (4-uFin)*maxAttacAngle/5)+ 90.0;
//  s2= ((4-rFin)*maxAttacAngle/5 - (4-uFin)*maxAttacAngle/5)+ 90.0;
  

  s1= ((defaultPitch-pitch)*maxAttacAngle/45 + (defaultRoll-roll)*maxAttacAngle/45) + 90.0;
  s2= ((defaultPitch-pitch)*maxAttacAngle/45 - (defaultRoll-roll)*maxAttacAngle/45) + 90.0;

  if (s1>180){
    s1=180;
  }
  else if(s1<0){
    s1=0;
  }
  if (s2>180){
    s2=180;
  }
  else if(s2<0){
    s2=0;
  }
  
//  s1= ((defaultRoll-roll)*maxAttacAngle/45);
//  s2= ((defaultRoll-roll)*maxAttacAngle/45); 




  Serial.println(s1);
  Serial.println(s2);
  myservo1.write(s1);
  myservo2.write(s2);
  
  
   
//  Wire.beginTransmission(4); // transmit to device #4
//  strcpy(message,data.c_str());
//  Wire.write(message);        //// sends data
//  Wire.endTransmission();    // stop transmitting

  int r2=0, g2=200, b2=0;    //  Red, green and blue intensity to display
  onePixel.setPixelColor(0, r2, g2, b2);   //  Set pixel 0 to (r,g,b) color value
  onePixel.show();                      //  Update pixel state
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  
}


void readEncoder_Main(){
  int b_A = digitalRead(ENCB);
  if(b_A > 0){
    pos_Main++;
  }
  else{
    pos_Main--;
  }
//  Serial.print("Relative Encoder: ---------------------------->");
//  Serial.println(pos_Main);
  //yield();
}
