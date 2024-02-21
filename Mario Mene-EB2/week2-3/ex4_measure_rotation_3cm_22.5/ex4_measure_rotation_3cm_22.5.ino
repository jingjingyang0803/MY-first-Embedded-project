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
const float steps_per_degree = 193 / 90.0;  //angular unit:degree

//===============Setting variables==============================
//>>>>>>>>>>>>>>>>>> variables for joystick >>>>>>>>>>>>>>>>
const int x_pin = A8, y_pin = A9;//horizontal/vertical analog pin for the joystick
int run_speed = 100;

int counter_R = 0;  // amount of right encoder pulses
int counter_L = 0;  // amount of left encoder pulses
int currentStateA_R, currentStateA_L, lastStateA_R, lastStateA_L;

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
  // Open serial communications
  Serial.begin(9600);
  while (!Serial) {
    ;  // wait for serial port to connect. Needed for native USB port only
  }
  Serial.println("Write *someCommand:someValue* to the serial monitor.");
}

void TURN(int angular)
{
  int target_counter = abs(steps_per_degree * angular);   // calculate how much steps to go

  if (angular > 0 && counter_L < target_counter) {        // turn right
    digitalWrite(Motor_L_dir_pin, Motor_forward);
    analogWrite(Motor_L_pwm_pin, run_speed);
  }
  else if (angular < 0 && counter_R < target_counter) {  // turn left
    digitalWrite(Motor_R_dir_pin, Motor_forward);
    analogWrite(Motor_R_pwm_pin, run_speed);
  }
  else {                                                 // stop
    analogWrite(Motor_R_pwm_pin, 0);
    analogWrite(Motor_L_pwm_pin, 0);
  }
}
void MOVE(int distance)
{
  int forward_or_return = distance > 0 ? Motor_forward : Motor_return; // decide the direction to go

  if (counter_R < abs(steps_per_cm * distance)) {                     // run the car to correct direction with correct distance
    digitalWrite(Motor_R_dir_pin, forward_or_return);
    digitalWrite(Motor_L_dir_pin, forward_or_return);
    analogWrite(Motor_L_pwm_pin, run_speed);
    analogWrite(Motor_R_pwm_pin, run_speed);
  }
  else {                                                             // stop
    analogWrite(Motor_R_pwm_pin, 0);
    analogWrite(Motor_L_pwm_pin, 0);
  }
}

void loop() {
  Serial.println(analogRead(y_pin));
  if (analogRead(y_pin) > 1000) {
    MOVE(3);
  }
  else if (analogRead(y_pin) < 20)  {
    MOVE(-3);
  }
  else if (analogRead(x_pin) > 1000) {
    TURN(22.5);
  }
  else if (analogRead(x_pin) < 20)
  {
    TURN(-22.5);
  }
  else {
     analogWrite(Motor_R_pwm_pin, 0);
    analogWrite(Motor_L_pwm_pin, 0);
  }
  //===============Updating the encoder steps==========================================================
  counter_update();
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
