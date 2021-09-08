[14:48, 08/09/2021] Pallavi Goel BREED HKU: //this is slave

#include <SoftwareSerial.h>
#include<String.h>
#include "Wire.h" // This library allows you to communicate with I2C devices.

const int MPU_ADDR = 0x68; // I2C address of the MPU-6050. If AD0 pin is set to HIGH, the I2C address will be 0x69.
int16_t accelerometer_x, accelerometer_y, accelerometer_z; // variables for accelerometer raw data
int16_t gyro_x, gyro_y, gyro_z; // variables for gyro raw data
int16_t temperature; // variables for temperature data
char tmp_str[7]; // temporary variable used in convert function
char* convert_int16_to_str(int16_t i)
{ // converts int16 to string. Moreover, resulting strings will have the same length in the debug monitor.
sprintf(tmp_str, "%6d", i);
return tmp_str;
}

SoftwareSerial BTSerial(10, 11);

String state = "";
const int led = 8;
int buttonstate = 1;

char message[100];

void setup() 
{
  Serial.begin(9600);
  BTSerial.begin(38400);
  pinMode(led, OUTPUT);
  digitalWrite(led, LOW);
//  pinMode(button, INPUT);
//  digitalWrite(button, HIGH);

  Wire.begin();
  Wire.beginTransmission(MPU_ADDR); // Begins a transmission to the I2C slave (GY-521 board)
  Wire.write(0x6B); // PWR_MGMT_1 register
  Wire.write(0); // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);
}
void loop() 
{
 if(BTSerial.available() > 0)
 { 
    // Checks whether data is comming from the serial port
    state = BTSerial.read(); // Reads the data from the serial port
 }
 
  if (state == '1') 
 {
  digitalWrite(led, HIGH); // LED ON
  Serial.println("Receiving signal from master");
  
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x3B); // starting with register 0x3B (ACCEL_XOUT_H) [MPU-6000 and MPU-6050 Register Map and Descriptions Revision 4.2, p.40]
  Wire.endTransmission(false); // the parameter indicates that the Arduino will send a restart. As a result, the connection is kept active.
  Wire.requestFrom(MPU_ADDR, 7*2, true); // request a total of 7*2=14 registers
  //"Wire.read()<<8 | Wire.read();" means two registers are read and stored in the same variable
  accelerometer_x = Wire.read()<<8 | Wire.read(); // reading registers: 0x3B (ACCEL_XOUT_H) and 0x3C (ACCEL_XOUT_L)
  accelerometer_y = Wire.read()<<8 | Wire.read(); // reading registers: 0x3D (ACCEL_YOUT_H) and 0x3E (ACCEL_YOUT_L)
  accelerometer_z = Wire.read()<<8 | Wire.read(); // reading registers: 0x3F (ACCEL_ZOUT_H) and 0x40 (ACCEL_ZOUT_L)
  temperature = Wire.read()<<8 | Wire.read(); // reading registers: 0x41 (TEMP_OUT_H) and 0x42 (TEMP_OUT_L)
  gyro_x = Wire.read()<<8 | Wire.read(); // reading registers: 0x43 (GYRO_XOUT_H) and 0x44 (GYRO_XOUT_L)
  gyro_y = Wire.read()<<8 | Wire.read(); // reading registers: 0x45 (GYRO_YOUT_H) and 0x46 (GYRO_YOUT_L)
  gyro_z = Wire.read()<<8 | Wire.read(); // reading registers: 0x47 (GYRO_ZOUT_H) and 0x48 (GYRO_ZOUT_L)
  String str[7];
  str[0]=convert_int16_to_str(accelerometer_x);
  str[1]=convert_int16_to_str(accelerometer_y);
  str[2]=convert_int16_to_str(accelerometer_z);
  str[3]=convert_int16_to_str(temperature);
  str[4]=convert_int16_to_str(gyro_x);
  str[5]=convert_int16_to_str(gyro_y);
  str[6]=convert_int16_to_str(gyro_z);

  String newstr = str[0]+','+str[1]+','+str[2]+','+str[3]+','+str[4]+','+str[5]+','+str[6]+'?';
  Serial.println(newstr);
  
  //Prepare string and send to receiver
  strcpy(message,newstr.c_str());
  BTSerial.write(message);        

//  int i=0;
//  while(newstr[i]!='?'){
//    char x = newstr[i];
//    BTSerial.write(x);//Sending back signal to master
//    Serial.println(x);
//    i++;
//  }
 }
 state = "";
 
// else if (state == "") 
// {
//  digitalWrite(led, LOW); // LED ON
//  BTSerial.write("NoValues");
//  state = 0;
// }
}
