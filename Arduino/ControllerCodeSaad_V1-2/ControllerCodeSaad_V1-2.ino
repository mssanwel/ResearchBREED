/*
   Fish encoder + servo fins (transmitter code)

   Transmits 4 signals:
        servo: up-down, left-right
        tail: turnValue, power

   Edited by: Tasnia, Brianna, Heidi, Allison, Saad, Taha

*/
//Screen
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


// Timers
int timer1 = 0;       //store the time when transmiting
int timer2 = 0;       //store the time when sending T signals
int timer3 = 0;       //store the time when pressing left-right buttons
int timer4 = 0;       //store the time when pressing up-down buttons
int timer5 = 0;       //store the time when sending P signals
int bounceDelay = 350; // delay to avoid bounce on switch

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
int pMap = 0;         //default value of power

//declare Funduino joystick
int sensorValueX = analogRead(A0);
int sensorValueY = analogRead(A1);

//Mapping
int initial_angle=0;
int final_angle=90;


void setup() {
  // Setup Serial
  Serial.begin(9600);
  Serial1.begin(9600, SERIAL_8O1);
  while (!Serial.read());
  while (!Serial1.read()); // Wait for serial to setup




  // Setup Screen
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { //Ive changed the address //already chill
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Don't proceed, loop forever
  }
  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  //  display.display();
  //  delay(2000); // Pause for 2 seconds
  //  // Clear the buffer
  display.clearDisplay();

  // Draw BREED welcome message
  display.clearDisplay();
  display.setTextSize(2); // Draw 2X-scale text
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(SCREEN_WIDTH, 0);
  display.println("Welcome to");
  display.setTextSize(3); // Draw 2X-scale text
  display.println("BREED");
  display.display();

  // Show the display buffer on the screen. You MUST call display() after
  // drawing commands to make them visible on screen!
  display.display();
  delay(3000);
  // display.display() is NOT necessary after every single drawing command,
  // unless that's what you want...rather, you can batch up a bunch of
  // drawing operations and then update the screen all at once by calling
  // display.display(). These examples demonstrate both approaches...





  //Setup Funduino pins
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(leftbutton, INPUT_PULLUP);
  pinMode(rightbutton, INPUT_PULLUP);
  pinMode(upbutton, INPUT_PULLUP);
  pinMode(downbutton, INPUT_PULLUP);
}



void loop() {


  //Update control variables from buttons
  //Reading Joystick Value
  sensorValueX = analogRead(A0);
  sensorValueY = analogRead(A1);
  xMap = map(sensorValueX, 0, 1023, initial_angle, final_angle);
  yMap = map(sensorValueY, 0, 1023, initial_angle, final_angle);


  //Reading the left, right, up and down buttons
  if ((digitalRead(leftbutton) == LOW) && tMap > 1) {
    if (millis() - timer3 > bounceDelay) {
      tMap--;
      Serial.println("button left");
      timer3 = millis();
    }
  }
  else if ((digitalRead(rightbutton) == LOW) && tMap < 9) {
    if (millis() - timer3 > bounceDelay) {
      tMap++;
      Serial.println("button right");
      timer3 = millis();
    }
  }
  else if ((digitalRead(downbutton) == LOW) && pMap > 0) {
    if (millis() - timer4 > bounceDelay) {
      pMap--;
      Serial.println("button down");
      timer4 = millis();
    }
  }
  else if ((digitalRead(upbutton) == LOW) && pMap < 9) {
    if (millis() - timer4 > bounceDelay) {
      pMap++;
      Serial.println("button up");
      timer4 = millis();
    }
  }


  //Update Screen

  display.clearDisplay();
  display.setTextSize(1); // Draw 2X-scale text
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print("PWM: ");
  display.println(pMap);
  display.print("Turning: ");
  display.println(tMap);
  display.println("Servo:  ");
  display.print("X= ");
  display.print(xMap);
  display.print(", Y= ");
  display.print(yMap);
  display.println();
  display.display();


  // Send signal after every 200 ms
  if (millis() - timer1 > 200) {
    timer1 = millis(); //update timer1

    // Prepare string and send to receiver
    ascii_sum = 'R' + (xMap + '0') + 'U' + (yMap + '0') + 'T' + (tMap + '0') + 'P' + (pMap + '0'); // Checksum
    sum_str = "R" + String(xMap) + "U" + String(yMap) + "T" + String(tMap) + "P" + String(pMap) + String(ascii_sum) + "?"; //payload + checksum
    strcpy(message, sum_str.c_str());
    Serial1.write(message);

    Serial.print("Signal: ");
    Serial.println(message);
    //blocking function, no need to wait for delay if use flush
    Serial1.flush();
  }
}
