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
#include <Wire.h>

//===============Defined constants========================================
#define Motor_L_dir_pin 7
#define Motor_R_dir_pin 8
#define Motor_L_pwm_pin 9
#define Motor_R_pwm_pin 10

// Rotary Encoder Inputs
//------Right motor
#define outputA_R 24  //CLK to D24
#define outputB_R 2   //DT to D2
//------Left motor
#define outputA_L 23  //CLK to D23
#define outputB_L 3   //DT to D3

#define Motor_forward 0
#define Motor_return 1

//===============Setting constants==============================
const float steps_per_cm = 293 / 30.0;         //distance unit:cm
const float steps_per_degree = 193 / 90.0;  //angular unit:degree

const int pot_max_value = 982;
const int pot_min_value = 19;
const float degrees_per_pot_step = 360.0 / (pot_max_value - pot_min_value);

//===============Setting variables==============================
//>>>>>>>>>>>>>>>>>> variables for joystick >>>>>>>>>>>>>>>>
const int x_pin = A8, y_pin = A9;//horizontal/vertical analog pin for the joystick
const int turn_pin = A1; // analog pin for the potentiometer, which defines the turning heading 0-360
int run_speed = 100;
int y_speed = 0;
int x_speed = 0; //turning speed

int counter_R = 0;  // amount of right encoder pulses
int counter_L = 0;  // amount of left encoder pulses
int currentStateA_R, currentStateA_L, lastStateA_R, lastStateA_L;

//>>>>>>>>>>>>>>>>>> variables for compass >>>>>>>>>>>>>>>>
#define CMPS14_ADDRESS 0x60  // Address of CMPS14 shifted right one bit for arduino wire library
byte raw;

int potValue = 0;
int potAngle = 0;
int initialAngle = 999;
int currentAngle = 0;

bool isRobotTurning = false;

void setup() {
  // Set encoder pins as inputs
  pinMode(outputA_R, INPUT);
  pinMode(outputB_R, INPUT);
  pinMode(outputA_L, INPUT);
  pinMode(outputB_L, INPUT);

  // Read the initial state of outputA
  lastStateA_R = digitalRead(outputA_R);
  lastStateA_L = digitalRead(outputA_L);

  lcd.begin(20, 4);
  Wire.begin();
  // Open serial communications
  Serial.begin(9600);
}

void TURN(int angular)
{
  if(currentAngle < potAngle) {    
    Serial.println("turning right");
    isRobotTurning = true;
    digitalWrite(Motor_L_dir_pin, Motor_forward);
    analogWrite(Motor_L_pwm_pin, 100);
  }

  if(currentAngle > potAngle) {
    Serial.println("turning left");
    isRobotTurning = true;
    digitalWrite(Motor_R_dir_pin, Motor_forward);
    analogWrite(Motor_R_pwm_pin, 100);
  }

  if(isRobotTurning && abs(currentAngle - potAngle) < 5) {
    Serial.println("stopping");
    analogWrite(Motor_R_pwm_pin, 0);
    analogWrite(Motor_L_pwm_pin, 0);      
    isRobotTurning = false;
  }
}

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
  }
  currentAngle =  360.0 * raw / 255.0;
  if (initialAngle == 999) {
    initialAngle = currentAngle;
  }

  //>>>>>>>>>>>>>>>>>>>>>>>>>>>> Potentiometer >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
  potValue = analogRead(turn_pin);
  potAngle = (potValue - pot_min_value) * degrees_per_pot_step;

  TURN(potAngle);
  
  //>>>>>>>>>>>>>>>>>>>>>>>>>>>> Joystick >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

  bool isSignalActive = false;
  if (analogRead(y_pin) > 533)
  {
    digitalWrite(Motor_R_dir_pin, Motor_forward);
    digitalWrite(Motor_L_dir_pin, Motor_forward);
    y_speed = (analogRead(y_pin) / 512.0) * 255 + 255;
    isSignalActive = true;
  }
  else if (analogRead(y_pin) < 490) {
    digitalWrite(Motor_R_dir_pin, Motor_return);
    digitalWrite(Motor_L_dir_pin, Motor_return);
    y_speed = -(analogRead(y_pin) / 512.0) * 255 - 255;
    isSignalActive = true;
  }
  else if (analogRead(y_pin) < 533 && analogRead(y_pin) > 490) {
    y_speed = 0;
  }

  if (isSignalActive) {
    analogWrite(Motor_L_pwm_pin, y_speed);
    analogWrite(Motor_R_pwm_pin, y_speed);

    if (y_speed == 0) {
      isSignalActive = false;
    }    
  }

}
