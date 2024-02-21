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
#define led_pin 4

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

//===============Setting variables==============================
//>>>>>>>>>>>>>>>>>> variables for joystick >>>>>>>>>>>>>>>>
const int x_pin = A8, y_pin = A9;//horizontal/vertical analog pin for the joystick
int run_speed = 100;

int counter_R = 0;  // amount of right encoder pulses
int counter_L = 0;  // amount of left encoder pulses
int currentStateA_R, currentStateA_L, lastStateA_R, lastStateA_L;

//>>>>>>>>>>>>>>>>>> variables for compass >>>>>>>>>>>>>>>>
#define CMPS14_ADDRESS 0x60  // Address of CMPS14 shifted right one bit for arduino wire library
byte raw;

bool isSignalActive = false;
bool isRobotMoving = false;
bool isRobotTurning = false;
bool isRobotTurningLeft = false;
bool isRobotTurningRight = false;
int distanceToMove = 0;
int initialAngle = 0;
int currentAngle = 0;
int targetAngle = 0;


void setup() {
  // Set encoder pins as inputs
  pinMode(outputA_R, INPUT);
  pinMode(outputB_R, INPUT);
  pinMode(outputA_L, INPUT);
  pinMode(outputB_L, INPUT);
  pinMode(led_pin, OUTPUT);

  // Read the initial state of outputA
  lastStateA_R = digitalRead(outputA_R);
  lastStateA_L = digitalRead(outputA_L);

  lcd.begin(20, 4);
  Wire.begin();
  // Open serial communications
  Serial.begin(9600);
  while (!Serial) {
    ;  // wait for serial port to connect. Needed for native USB port only
  }
  Serial.println("Write *someCommand:someValue* to the serial monitor.");

}

void TURN(int angular)
{
  isRobotTurning = true;
  targetAngle = initialAngle + angular;

  // Start turning the robot - no need to check the condition here,
  // since this method only runs once when you move the joystick
  if (angular > 0) {  // turn right
    isRobotTurningRight = true;  
    digitalWrite(Motor_L_dir_pin, Motor_forward);
    analogWrite(Motor_L_pwm_pin, run_speed);
  } else if (angular < 0) { // turn left
    isRobotTurningLeft = true;  
    digitalWrite(Motor_R_dir_pin, Motor_forward);
    analogWrite(Motor_R_pwm_pin, run_speed);
  }
}

void MOVE(int distance)
{
  isRobotMoving = true;
  distanceToMove = distance;

  int forward_or_return = distance > 0 ? Motor_forward : Motor_return; // decide the direction to go

  // Start moving the robot - no need to check the condition here,
  // since this method only runs once when you move the joystick
  digitalWrite(Motor_R_dir_pin, forward_or_return);
  digitalWrite(Motor_L_dir_pin, forward_or_return);
  analogWrite(Motor_L_pwm_pin, run_speed);
  analogWrite(Motor_R_pwm_pin, run_speed);
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

  // Check if signal is not active, only then call the MOVE or TURN method
  // This ensures that the method is only called once
  if (!isSignalActive && analogRead(y_pin) > 600) {
    isSignalActive = true;
    Serial.println("FORWARD");
    MOVE(3);
  }
  else if (!isSignalActive && analogRead(y_pin) < 400)  {
    isSignalActive = true;
    Serial.println("BACKWARDS");
    MOVE(-3);
  }
  else if (!isSignalActive && analogRead(x_pin) > 600) {
    isSignalActive = true;
    initialAngle =  currentAngle;
    Serial.println("RIGHT");
    TURN(22.5);
  }
  else if (!isSignalActive && analogRead(x_pin) < 400) {
    isSignalActive = true;
    initialAngle =  currentAngle;
    Serial.println("LEFT");
    TURN(-22.5);
  }

  // set the signal back to false only when the joystick returns to the home position
  if (isSignalActive && 
      analogRead(x_pin) < 600 &&
      analogRead(x_pin) > 400 &&
      analogRead(y_pin) < 600 &&
      analogRead(y_pin) > 400) {

    isSignalActive = false;
  }

  // if the robot is currently moving, continuously check if it already reached the goal
  if (isRobotMoving) {
    checkMoveCondition();
  }

  // if the robot is currently turning, continuously check if it already reached the goal  
  if (isRobotTurning) {
    checkTurnCondition();
  }

  //===============Updating the encoder steps==========================================================
  counter_update();
}

void checkMoveCondition() {
  if(counter_R >= abs(steps_per_cm * distanceToMove)) {

    isRobotMoving = false;
    
    analogWrite(Motor_R_pwm_pin, 0);
    analogWrite(Motor_L_pwm_pin, 0);

    counter_R = counter_L = 0;
  }
}

void checkTurnCondition() {
  if((isRobotTurningRight && currentAngle >= targetAngle) ||
      isRobotTurningLeft && currentAngle <= targetAngle) {

    isRobotTurning = false;
    isRobotTurningRight = false;
    isRobotTurningLeft = false;

    analogWrite(Motor_R_pwm_pin, 0);
    analogWrite(Motor_L_pwm_pin, 0);
    
    counter_R = counter_L = 0;
  }
}

void counter_update() {
  currentStateA_L = digitalRead(outputA_L);
  if (currentStateA_L != lastStateA_L && currentStateA_L == 1) {
    counter_L++;
  }
  lastStateA_L = currentStateA_L;
  delay(1);

  currentStateA_R = digitalRead(outputA_R);
  if (currentStateA_R != lastStateA_R && currentStateA_R == 1) {
    counter_R++;
  }
  lastStateA_R = currentStateA_R;
  delay(1);
}
