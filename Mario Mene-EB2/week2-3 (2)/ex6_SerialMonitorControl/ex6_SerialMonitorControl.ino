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
const float steps_per_degree = 193 / 90.0;  //angular unit:degree

//===============Setting variables==============================
//>>>>>>>>>>>>>>>>>> variables for compass >>>>>>>>>>>>>>>>
#define CMPS14_ADDRESS 0x60  // Address of CMPS14 shifted right one bit for arduino wire library
byte raw;
int initial_dir, current_dir;


String message = "";
String command = "";
int value = 0;

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

  // join I2C bus
  Wire.begin();

}

void Turn(int angular)
{
  if (angular > 0 && current_dir < initial_dir + angular) {  // turn right
    digitalWrite(Motor_L_dir_pin, Motor_forward);
    analogWrite(Motor_L_pwm_pin, run_speed);
  }
  else if (angular < 0 && current_dir > initial_dir + angular) { // turn left
    digitalWrite(Motor_R_dir_pin, Motor_forward);
    analogWrite(Motor_R_pwm_pin, run_speed);
  }
  else {                                                 // stop
    analogWrite(Motor_R_pwm_pin, 0);
    analogWrite(Motor_L_pwm_pin, 0);
  }
}
void Move(int distance)
{

  int forward_or_return = distance > 0 ? Motor_forward : Motor_return; // decide the direction to go

  if (counter_R < abs(steps_per_cm * distance)) {                     // run the car to correct direction with correct distance
    Serial.println(counter_R);
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
  current_dir =  360.0 * raw / 255.0;

  //===============Read and analyse the input message=================================
  while (Serial.available() > 0) {
    initial_dir = 360.0 * raw / 255.0;
    message = Serial.readStringUntil('\n');
    //===============1) get command and value===========================
    command = message.substring(0, message.indexOf(":"));
    value = message.substring(message.indexOf(":") + 1).toInt();

    if (command == "MOVE")
      counter_R = counter_L = 0;

    lcd.clear();           // clear the LCD screen for new input
    lcd.setCursor(0, 0);       // print the command on first row
    lcd.print("Command: ");
    lcd.print(command);
    lcd.setCursor(0, 1);       // print the value on second row
    lcd.print("Value: ");
    lcd.println(value);
  }

  //=============== Control the vehicle according to the given command and value ==================
  if (command == "SPEED")        //===============case 1: SPEED:0-255=========
    run_speed = value;
  else if (command == "TURN")    //===============case 2: TURN:LEFT/RIGHT=====
    Turn(value);
  else if (command == "MOVE")    //===============case 3: MOVE:forward/return==
    Move(value);
  else if (command == "LED")     //===============case 4: LED:0/1===============
    digitalWrite(led_pin, value);// 0->LOW,LED OFF    1->HIGH,LED ON

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
