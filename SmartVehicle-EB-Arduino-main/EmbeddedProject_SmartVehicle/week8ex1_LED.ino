/*
  programme intro:
  more code idea: command:value,pwm value-->lightness, speed; LED: right/left turning light, night light, reverse light.
  web design: vehicle image, programme intro, message accepted, prewriten message with description.
*/
//===============Including and Initializing the LCD library=========================================
#include <LiquidCrystal.h>
const int rs = 37, en = 36, d4 = 35, d5 = 34, d6 = 33, d7 = 32;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
​
​
//===============Defined constants========================================
#define Motor_L_dir_pin 7
#define Motor_R_dir_pin 8
#define Motor_L_pwm_pin 9
#define Motor_R_pwm_pin 10
#define led_pin 4
​
// Rotary Encoder Inputs
//------Right motor
#define outputA_R 24  //CLK to D24
#define outputB_R 2   //DT to D2
//------Left motor
#define outputA_L 23  //CLK to D23
#define outputB_L 3   //DT to D3
​
#define Motor_forward 0
#define Motor_return 1
​
//===============Setting constants==============================
const float steps_per_cm = 403 / 20.0;         //distance unit:cm
const float steps_per_degree_R = 153 / 180.0;  //angular unit:degree
const float steps_per_degree_L = 273 / 180.0;  //angular unit:degree
const int y_speed = 100;                       //forward/return speed
const int x_speed = 100;                       //turning speed
​
//===============Setting variables==============================
String message = "";
String command = "";
String value = "";
​
int forward_or_return = Motor_forward;
int on_or_off = LOW;  //LED OFF by default
String wheel_to_turn = "";
float target_steps = 0;
​
int counter_R = 0;  // amount of right encoder pulses
int counter_L = 0;  // amount of left encoder pulses
int currentStateA_R, currentStateA_L;
int lastStateA_R, lastStateA_L;
​
void setup() {
  // Set encoder pins as inputs
  pinMode(outputA_R, INPUT);
  pinMode(outputB_R, INPUT);
  pinMode(outputA_L, INPUT);
  pinMode(outputB_L, INPUT);
  pinMode(led_pin, OUTPUT);
​
  // Read the initial state of outputA
  lastStateA_R = digitalRead(outputA_R);
  lastStateA_L = digitalRead(outputA_L);
​
  // Open serial communications
  Serial.begin(9600);
  while (!Serial) {
    ;  // wait for serial port to connect. Needed for native USB port only
  }
  // send an intro:
  /*
    LED:ON/OFF
    MOVE:(-50~0)return/(0~50)forward      //distance unit:cm
    TURN:(-180~0)left/(0~180)right        //angular unit:degree
  */
  Serial.println("Write *someCommand:*someValue* to the serial monitor.");
​
  lcd.begin(20, 4);
}
​
void loop() {
  //===============Read and analyse the input message=================================
  while (Serial.available() > 0) {
    message = Serial.readStringUntil('\n');
    //===============1) get command and value===========================
    command = message.substring(0, message.indexOf(":"));
    value = message.substring(message.indexOf(":") + 1);
​
    Serial.print("\nCommand: ");
    Serial.print(command);
    Serial.print("\tValue: ");
    Serial.println(value);
​
​
    // clear the string for new input:
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Command: ");
    lcd.print(command);
    lcd.setCursor(0, 1);
    lcd.print("Value: ");
    lcd.println(value);
​
    //===============2) computing distance/degree from given value======
    if (command == "LED") {
      if (value == "ON")
        on_or_off = HIGH;
      else if (value == "OFF")
        on_or_off = LOW;
    }
    else if (command == "MOVE") {
      counter_R = 0;
      float distance = value.toFloat();
      target_steps = abs(steps_per_cm * distance);
​
      if (distance > 0) {
        forward_or_return = Motor_forward;
        Serial.print("Motor_forward");
      } else if (distance < 0) {
        forward_or_return = Motor_return;
        Serial.print("Motor_return");
      }
    }
    else if (command == "TURN") {
      counter_R = 0;
      counter_L = 0;
      float angular = value.toFloat();
​
      if (angular > 0) {
        target_steps = abs(steps_per_degree_L * angular);
        wheel_to_turn = "left";
        Serial.print("Turn_right");
      } else if (angular < 0) {
        target_steps = abs(steps_per_degree_R * angular);
        wheel_to_turn = "right";
        Serial.print("Turn_left");
      }
    }
    Serial.print("\ttarget_steps: ");
    Serial.println(target_steps);
  }
  //===============Control the vehicle according to the given command==================
  //===============case 1: LED:ON/OFF===========================
  digitalWrite(led_pin, on_or_off);
  //===============case 2: MOVE:forward/return==================
  if (command == "MOVE") {
    if (counter_R < target_steps) {
      digitalWrite(Motor_R_dir_pin, forward_or_return);
      digitalWrite(Motor_L_dir_pin, forward_or_return);
      analogWrite(Motor_L_pwm_pin, y_speed);
      analogWrite(Motor_R_pwm_pin, y_speed);
    } else {
      analogWrite(Motor_L_pwm_pin, 0);
      analogWrite(Motor_R_pwm_pin, 0);
    }
  }
  //===============case 3: TURN:LEFT/RIGHT======================
  else if (command == "TURN" && wheel_to_turn == "left") {
    if (counter_L < target_steps) {
      digitalWrite(Motor_L_dir_pin, Motor_forward);
      analogWrite(Motor_L_pwm_pin, x_speed);
    } else {
      analogWrite(Motor_L_pwm_pin, 0);
    }
  } else if (command == "TURN" && wheel_to_turn == "right") {
    if (counter_R < target_steps) {
      digitalWrite(Motor_R_dir_pin, Motor_forward);
      analogWrite(Motor_R_pwm_pin, x_speed);
    } else {
      analogWrite(Motor_R_pwm_pin, 0);
    }
  }
​
  //===============Updating the encoder steps==================================
  currentStateA_L = digitalRead(outputA_L);
  if (currentStateA_L != lastStateA_L && currentStateA_L == 1) {
    counter_L++;
    Serial.print("Left counter: ");
    Serial.print(counter_L);
  }
  lastStateA_L = currentStateA_L;
  delay(1);
​
  currentStateA_R = digitalRead(outputA_R);
  if (currentStateA_R != lastStateA_R && currentStateA_R == 1) {
    counter_R++;
    Serial.print("                        || ");
    Serial.print("Right counter: ");
    Serial.println(counter_R);
  }
  lastStateA_R = currentStateA_R;
  delay(1);
}
