#include <Wire.h>
#include <LIDARLite.h>
#include <LiquidCrystal.h>

#define Motor_L_dir_pin 7
#define Motor_R_dir_pin 8
#define Motor_L_pwm_pin 9
#define Motor_R_pwm_pin 10

//------Right motor
#define outputA_R 24  //CLK to D24
#define outputB_R 2   //DT to D2
//------Left motor
#define outputA_L 23  //CLK to D23
#define outputB_L 3   //DT to D3

#define Motor_forward 0
#define Motor_return 1

const int rs = 37, en = 36, d4 = 35, d5 = 34, d6 = 33, d7 = 32;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

const int pot_max_value = 982;
const int pot_min_value = 19;
const float distance_per_pot_step = 80.0 / (pot_max_value - pot_min_value); // range from 20cm to 1m
const int pot_pin = A1; // analog pin for the potentiometer, which defines the turning heading 0-360

// Globals
LIDARLite lidarLite;
int cal_cnt = 0;

int run_speed = 100;
int y_speed = 100;

int potValue = 0;
int potDist = 0;

int measuredDist = 0;
int adjustedDist = 0;
int fixedDist = 30;
String motorStatus = "OFF";

void setup()
{
  Serial.begin(9600); // Initialize serial connection to display distance readings

  lcd.begin(20, 4);
  Wire.begin();
  lidarLite.begin(0, true); // Set configuration to default and I2C to 400 kHz
  lidarLite.configure(1); // Change this number to try out alternate configurations
}

void loop()
{

  //=============== Distance control ==================

  // At the beginning of every 100 readings,
  // take a measurement with receiver bias correction
  int newMeasuredDist = 0;
  if ( cal_cnt == 0 ) {
    newMeasuredDist = lidarLite.distance();      // With bias correction
  } else {
    newMeasuredDist = lidarLite.distance(false); // Without bias correction
  }

  if (abs(measuredDist - newMeasuredDist) > 5 || measuredDist == 0) {
    measuredDist = newMeasuredDist;
  }

  potValue = analogRead(pot_pin);
  Serial.println(potValue);
  adjustedDist = ((potValue - pot_min_value) * distance_per_pot_step) + 20; //


  //=============== Motor control ==================

  if (abs(adjustedDist - measuredDist) < 6) {
    y_speed = 0;
    motorStatus = "OFF";
  } else if ((adjustedDist - measuredDist) < -5) {
    digitalWrite(Motor_R_dir_pin, Motor_forward);
    digitalWrite(Motor_L_dir_pin, Motor_forward);
    y_speed = 100;
    motorStatus = "RUN FW";
    Serial.println("forward");
  } else if ((adjustedDist - measuredDist) > 5) {
    digitalWrite(Motor_R_dir_pin, Motor_return);
    digitalWrite(Motor_L_dir_pin, Motor_return);
    y_speed = 100;
    motorStatus = "RUN RET";
    Serial.println("back");
  }

  analogWrite(Motor_L_pwm_pin, y_speed);
  analogWrite(Motor_R_pwm_pin, y_speed);


  //=============== LCD control ==================

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Adjusted: " + String(adjustedDist) + " cm");
  lcd.setCursor(0, 1);
  lcd.print("Measured: " + String(measuredDist) + " cm");
  lcd.setCursor(0, 2);
  lcd.print("Motor:    " + String(motorStatus));

  // Increment reading counter
  cal_cnt++;
  cal_cnt = cal_cnt % 100;

  delay(100);
}
