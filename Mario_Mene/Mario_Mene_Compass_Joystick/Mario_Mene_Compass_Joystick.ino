//**************************Mario Mene*************************************

/*
    LED:ON/OFF
    MOVE:(-50~0)return/(0~50)forward      //distance unit:cm
    TURN:(-180~0)left/(0~180)right        //angular unit:degree
    SPEED:(0-255)
*/
//===============Including and Initializing the LCD library=========================================
#include <LiquidCrystal.h>
const int rs = 37, en = 36, d4 = 35, d5 = 34, d6 = 33, d7 = 32;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
// add I2C library: I2C Inter-Integrated Circuit is a serial communications bus.
// It is often used for communicaions between microcontrollers and peripheral ICs, and for ...
#include <Wire.h>

//===============Defined constants========================================
#define Motor_L_dir_pin 7
#define Motor_R_dir_pin 8
#define Motor_L_pwm_pin 9
#define Motor_R_pwm_pin 10

#define Motor_forward 0
#define Motor_return 1

//===============Setting variables==============================
//>>>>>>>>>>>>>>>>>> variables for compass >>>>>>>>>>>>>>>>
#define CMPS14_ADDRESS 0x60  // Address of CMPS14 shifted right one bit for arduino wire library
byte raw;
int bearing;
String dir;
//>>>>>>>>>>>>>>>>>> variables for joystick >>>>>>>>>>>>>>>>
const int x_pin = A8, y_pin = A9;//horizontal/vertical analog pin for the joystick
int y_speed = 0;
int x_speed = 0; //turning speed

//******************************************************************************************************************************************
//******************************** Setup Section
//******************************************************************************************************************************************
void setup() {
  // join I2C bus
  Wire.begin();

  lcd.begin(20, 4);
}

//******************************************************************************************************************************************
//******************************** Loop Section
//******************************************************************************************************************************************
void loop() {
  //>>>>>>>>>>>>>>>>>>>>>>>>>>>> Compass >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
  Wire.beginTransmission(CMPS14_ADDRESS);  //starts communication with CMPS14
  Wire.write(1);                     //Sends the register 1 from where we wish to start reading heading value
  Wire.endTransmission(false);       // keep connection active

  // Request one byte from the CMPS14
  // this will give us the 8 bit bearing
  delay(1);
  Wire.requestFrom(CMPS14_ADDRESS, 1);

  // Wait for all bytes to come back
  if (Wire.available() >= 1) {
    raw = Wire.read();              // 0~255
    Serial.println(raw);
  }
  bearing =  360.0 * raw / 255.0;

  if (bearing == 360) {
    bearing =  360;
  }
  else if (bearing >= 180) {
    bearing =  360.0 * raw / 255.0 - 180;
  }
  else if (bearing < 180) {
    bearing = 360.0 * raw / 255.0 + 180;
  }


  lcd.setCursor(0, 0);
  lcd.print(bearing);
  delay(5);

  if (bearing  == 0 || bearing  == 360)
    dir = "North !!!";
  else if (bearing  > 0 && bearing  < 90)
    dir = "NE";
  else if (bearing  == 90)
    dir = "East -->";
  else if (bearing  > 90 && bearing  < 180)
    dir = "SE";
  else if (bearing  == 180)
    dir = "South !!!";
  else if (bearing  > 180 && bearing  < 270)
    dir = "SW";
  else if (bearing  == 270)
    dir = "West <--";
  else if (bearing  > 270 && bearing  < 360)
    dir = "NW";

  lcd.setCursor(0, 1);
  lcd.print(dir);
  delay(5);
  lcd.clear();
  //>>>>>>>>>>>>>>>>>>>>>>>>>>>> Joystick control >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
  if (analogRead(y_pin) > 533)
  {
    digitalWrite(Motor_R_dir_pin, Motor_forward);
    digitalWrite(Motor_L_dir_pin, Motor_forward);
    y_speed = (analogRead(y_pin) / 512.0) * 255 + 255;
  }
  else if (analogRead(y_pin) < 490) {
    digitalWrite(Motor_R_dir_pin, Motor_return);
    digitalWrite(Motor_L_dir_pin, Motor_return);
    y_speed = -(analogRead(y_pin) / 512.0) * 255 - 255;
  }
  else {
    y_speed = 0;
  }
  analogWrite(Motor_L_pwm_pin, y_speed);
  analogWrite(Motor_R_pwm_pin, y_speed);

  if (analogRead(x_pin) < 490) {
    digitalWrite(Motor_L_dir_pin, Motor_forward);
    x_speed = -(analogRead(x_pin) / 512.0) * 255 + 255;
    analogWrite(Motor_L_pwm_pin, x_speed);
  }
  else if (analogRead(x_pin) > 533)
  {
    digitalWrite(Motor_R_dir_pin, Motor_forward);
    x_speed = (analogRead(x_pin) / 512.0) * 255 - 255;
    analogWrite(Motor_R_pwm_pin, x_speed);
  }
  else {
    x_speed = 0;
  }
}
