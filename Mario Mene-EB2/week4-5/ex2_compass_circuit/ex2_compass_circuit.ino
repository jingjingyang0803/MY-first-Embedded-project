// https://github.com/garmin/LIDARLite_Arduino_Library/blob/master/examples/v3/GetDistanceI2c/GetDistanceI2c.ino
// https://github.com/garmin/LIDARLite_Arduino_Library/issues/14


// include the library code:
#include <LiquidCrystal.h>

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 37, en = 36, d4 = 35, d5 = 34, d6 = 33, d7 = 32;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

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

int run_speed = 100;
int counter_R = 0;  // amount of right encoder pulses
int counter_L = 0;  // amount of left encoder pulses
int currentStateA_R, currentStateA_L, lastStateA_R, lastStateA_L;

int p_pin = A2; // potentional meter pin

int target_distance = 30; // default as 30cm, but can be adjusted from potentional meter(range 20cm to 100cm)
int meausred_distance;
String motor_status = ""; // OFF,RUN FW,RUN RET

#include <Wire.h>
#include <LIDARLite.h>

LIDARLite myLidarLite;

//>>>>>>>>>>>>>>>>>> variables for compass >>>>>>>>>>>>>>>>
#define CMPS14_ADDRESS 0x60  // Address of CMPS14 shifted right one bit for arduino wire library
byte raw;
int bearing;
String dir;

void setup()
{
  // Set encoder pins as inputs
  pinMode(outputA_R, INPUT);
  pinMode(outputB_R, INPUT);
  pinMode(outputA_L, INPUT);
  pinMode(outputB_L, INPUT);
  pinMode(led_pin, OUTPUT);

  // Read the initial state of outputA
  lastStateA_R = digitalRead(outputA_R);
  lastStateA_L = digitalRead(outputA_L);

  Wire.begin();
  lcd.begin(20, 4);

  Serial.begin(9600); // Initialize serial connection to display distance readings

  myLidarLite.begin(0, true); // Set configuration to default and I2C to 400 kHz

  myLidarLite.configure(0); // Change this number to try out alternate configurations
}

void loop()
{
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


  target_distance = analogRead(p_pin) * 80 / 1023.0 + 20; //range 20cm to 1m
  meausred_distance = myLidarLite.distance(false);

  if (meausred_distance > target_distance) {
    motor_status = "RUN FW";
    Move(meausred_distance - target_distance);
  }
  else {
    Turn(-30);                // turn left 30 degree when reaching the adjusted distance
  }

  counter_update();

  lcd.setCursor(5, 0);
  lcd.print("Adjusted distance: ");
  lcd.print(target_distance);
  lcd.print(" cm");

  lcd.setCursor(5, 1);
  lcd.print("Measured distance: ");
  lcd.print(meausred_distance);
  lcd.print(" cm");

  lcd.setCursor(5, 2);
  lcd.print("Motor status: ");
  lcd.print(motor_status);

  lcd.setCursor(5, 3);
  lcd.print("Direction: ");
  lcd.print(dir);
}

void Move(int distance)
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

void Turn(int angular)
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
