//**********************Mario Mene*********************************

// get 120 steps for left turning 180
// get 400 steps for right turning 180
// get 40 steps(right wheel) for forward 20cm
// so stepsPerCm=2, stepsPerDegree=0.6
#define Motor_forward         0
#define Motor_return          1
#define Motor_L_dir_pin       7
#define Motor_R_dir_pin       8
#define Motor_L_pwm_pin       9
#define Motor_R_pwm_pin       10
#define reset_button          19

// Rotary Encoder Inputs
#define outputA_left  23     //CLK to D23
#define outputB_left  3     //DT to D3
#define outputA_right 24     //CLK to D24
#define outputB_right 2     //DT to D2

const int x_pin = A8, y_pin = A9;//horizontal/vertical analog pin for the joystick
int y_speed = 0;
int x_speed = 0; //turning speed

int counter_right = 0, counter_left = 0; // amount of steps
int currentStateA_right, currentStateA_left;
int lastStateA_right, lastStateA_left;
String currentDir_right = "", currentDir_left = "";
unsigned long lastButtonPress = 0;

#include <LiquidCrystal.h>
const int rs = 37, en = 36, d4 = 35, d5 = 34, d6 = 33, d7 = 32;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup()
{
  // Setup Serial Monitor
  Serial.begin(9600);

  pinMode(reset_button, INPUT);

  // Set encoder pins as inputs
  pinMode(outputA_right, INPUT);
  pinMode(outputB_right, INPUT);
  pinMode(outputA_left, INPUT);
  pinMode(outputB_left, INPUT);
  // Read the initial state of outputA
  lastStateA_right = digitalRead(outputA_right);
  lastStateA_left = digitalRead(outputA_left);

  lcd.begin(20, 4);
  lcd.setCursor(0, 0);
  lcd.print("Left: ");
  lcd.setCursor(0, 1);
  lcd.print("Right: ");
}

void loop()
{
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


  // Read the current state of outputA
  currentStateA_right = digitalRead(outputA_right);
  // If last and current state of outputA are different, then pulse occurred
  // React to only 1 state change to avoid double count
  if (currentStateA_right != lastStateA_right  && currentStateA_right == 1) {
    // If the outputB state is different than the outputA state then
    // the encoder is rotating CCW so decrement
    if (digitalRead(outputB_right) != currentStateA_right) {
      counter_right --;
      currentDir_right = "ACW";// anti-clockwise
    }
    else {
      // Encoder is rotating CW so increment
      counter_right ++;
      currentDir_right = " CW";//clockwise
    }
    Serial.print("                             Right encoder: ");
    Serial.print(currentDir_right);
    Serial.print("   ");
    Serial.println(counter_right);


    lcd.setCursor(7, 1);
    lcd.print(currentDir_right);
    lcd.setCursor(15, 1);
    lcd.print(counter_right);
    lcd.print("     ");
  }


  currentStateA_left = digitalRead(outputA_left);
  if (currentStateA_left != lastStateA_left  && currentStateA_left == 1) {
    if (digitalRead(outputB_left) != currentStateA_left) {
      counter_left --;
      currentDir_left = "ACW";
    }
    else {
      counter_left ++;
      currentDir_left = " CW";
    }
    Serial.print("Left encoder: ");
    Serial.print(currentDir_left);
    Serial.print("   ");
    Serial.println(counter_left);// counter for one round is the steps number

    lcd.setCursor(7, 0);
    lcd.print(currentDir_left);
    lcd.setCursor(15, 0);
    lcd.print(counter_left);
    lcd.print("     ");
  }

  // Remember last outputA state
  lastStateA_right = currentStateA_right;
  lastStateA_left = currentStateA_left;

  if (digitalRead(reset_button) == LOW) {
    //if 50ms have passed since last LOW pulse, it means that the
    //button has been pressed, released and pressed again
    if (millis() - lastButtonPress > 50) {
      Serial.println("Button pressed!");
      counter_right = 0;
      counter_left = 0;
    }
    // Remember last button press event
    lastButtonPress = millis();
  }
}
