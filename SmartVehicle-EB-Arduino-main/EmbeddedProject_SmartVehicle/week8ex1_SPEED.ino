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
const float steps_per_cm = 64 / 30.0;         //distance unit:cm
const float steps_per_degree_R = 148 / 180.0;  //angular unit:degree
const float steps_per_degree_L = 499 / 180.0;  //angular unit:degree

//===============Setting variables==============================
String message = "";
String command = "";
int value = 0;

int run_speed = 100;
int forward_or_return = Motor_forward;
float target_steps = 0;

int counter_R = 0;  // amount of right encoder pulses
int counter_L = 0;  // amount of left encoder pulses
int currentStateA_R, currentStateA_L;
int lastStateA_R, lastStateA_L;

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
  Serial.println("Write *someCommand:*someValue* to the serial monitor.");
}

void loop() {
  //===============Read and analyse the input message=================================
  while (Serial.available() > 0) {
    message = Serial.readStringUntil('\n');
    //===============1) get command and value===========================
    command = message.substring(0, message.indexOf(":"));
    value = message.substring(message.indexOf(":") + 1).toInt();

    lcd.clear();           // clear the LCD screen for new input
    lcd.setCursor(0, 0);       // print the command on first row
    lcd.print("Command: ");
    lcd.print(command);
    lcd.setCursor(0, 1);       // print the value on second row
    lcd.print("Value: ");
    lcd.println(value);

    if (command == "MOVE" || command == "TURN") {
      counter_R = 0;
      counter_L = 0;
    }
  }

  //=============== Control the vehicle according to the given command and value ==================
  if (command == "SPEED") {
    //===============case 1: SPEED:0-255===========================
    run_speed = value;
  }
  else if (command == "TURN") {
    //===============case 2: TURN:LEFT/RIGHT======================
    if (value > 0) {// turn right
      target_steps = abs(steps_per_degree_L * value);
      if (counter_L < target_steps) {
        digitalWrite(Motor_L_dir_pin, Motor_forward);
        analogWrite(Motor_L_pwm_pin, run_speed);
        analogWrite(Motor_R_pwm_pin, 0);
      } else {
        analogWrite(Motor_L_pwm_pin, 0);
      }
    } else if (value < 0) {// turn left
      target_steps = abs(steps_per_degree_R * value);
      if (counter_R < target_steps) {
        digitalWrite(Motor_R_dir_pin, Motor_forward);
        analogWrite(Motor_R_pwm_pin, run_speed);
        analogWrite(Motor_L_pwm_pin, 0);
      } else {
        analogWrite(Motor_R_pwm_pin, 0);
      }
    }
  }
  else if (command == "MOVE") {
    //===============case 3: MOVE:forward/return==================
    target_steps = abs(steps_per_cm * value);

    // decide which direction to run
    if (value > 0)
      forward_or_return = Motor_forward;
    else if (value < 0)
      forward_or_return = Motor_return;

    // drive the target distance
    if (counter_R < target_steps) {
      digitalWrite(Motor_R_dir_pin, forward_or_return);
      digitalWrite(Motor_L_dir_pin, forward_or_return);
      analogWrite(Motor_L_pwm_pin, run_speed);
      analogWrite(Motor_R_pwm_pin, run_speed);
    } else {
      analogWrite(Motor_L_pwm_pin, 0);
      analogWrite(Motor_R_pwm_pin, 0);
    }
  }
  else if (command == "LED") {
    //===============case 4: LED:0/1===========================
    // 0->LOW,LED OFF          1->HIGH,LED ON
    digitalWrite(led_pin, value);
  }

  //===============Updating the encoder steps==================================
  currentStateA_L = digitalRead(outputA_L);
  if (currentStateA_L != lastStateA_L && currentStateA_L == 1) {
    counter_L++;
    Serial.print("Left counter: ");
    Serial.print(counter_L);
  }
  lastStateA_L = currentStateA_L;
  delay(1);

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
