#define Motor_forward         0
#define Motor_return          1
#define Motor_L_dir_pin       7
#define Motor_R_dir_pin       8
#define Motor_L_pwm_pin       9
#define Motor_R_pwm_pin       10
#define rstbtn  19  //reset button pin//SW


volatile int buttonPin = 19;     // the number of the pushbutton pin
const int x_pin = A8, y_pin = A9;//horizontal/vertical analog pin for the joystick
int y_speed = 0;
int x_speed = 0; //turning speed

// Rotary Encoder Inputs
///////Left motor
//#define outputA 23     //CLK to D23
//#define outputB 3     //DT to D3

//////Right motor
#define outputA 24     //CLK to D24
#define outputB 2     //DT to D2

// include the library code:
#include <LiquidCrystal.h>

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 37, en = 36, d4 = 35, d5 = 34, d6 = 33, d7 = 32;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

int counter = 0;// amount of pulses
int currentStateA;
int lastStateA;
String currentDir = "";
const double distance = 50;
const float pi = 3.14; // Pi value
const int R = 3;  //Radius of the wheel from center to edge
int N = 0; //number of steps for one rotation
int n = 160;// this value is known from N
unsigned long lastButtonPress = 0;

void setup()
{
  pinMode(buttonPin, INPUT);
  pinMode(Motor_L_dir_pin, OUTPUT);
  pinMode(Motor_R_dir_pin, OUTPUT);
  pinMode(Motor_L_pwm_pin, OUTPUT);
  pinMode(Motor_R_pwm_pin, OUTPUT);

  // set up the LCD's number of columns and rows:
  lcd.begin(20, 4);
  // Setup Serial Monitor
  Serial.begin(9600);

  // Set encoder pins as inputs
  pinMode(outputA, INPUT);
  pinMode(outputB, INPUT);

  // Read the initial state of outputA
  lastStateA = digitalRead(outputA);


  Serial.print("Radius of the wheel from center to edge ?    ");
  Serial.print(R);
  Serial.println("cm");

  Serial.print("How many pulses one round contain ?          ");
  Serial.println(n);

  Serial.print("What is wheel distance in one round ?        ");
  Serial.print(2 * pi * R);
  Serial.println("cm");

  Serial.print("How many pulses driving 50cm gives ?         ");
  Serial.println(50 / ((2 * pi * R) / n));

  Serial.print("How many pulses 1 cm driving gives ?         ");
  Serial.println(1 / ((2 * pi * R) / n));
}

void loop()
{
  if (analogRead(y_pin) > 533)
  {
    digitalWrite(Motor_R_dir_pin, Motor_forward);
    digitalWrite(Motor_L_dir_pin, Motor_forward);
    y_speed = (analogRead(y_pin) / 512.0) * 255 - 255;
  }
  else if (analogRead(y_pin) < 490) {
    digitalWrite(Motor_R_dir_pin, Motor_return);
    digitalWrite(Motor_L_dir_pin, Motor_return);
    y_speed = -(analogRead(y_pin) / 512.0) * 255 + 255;
  }
  else {
    y_speed = 0;
  }
  analogWrite(Motor_L_pwm_pin, y_speed);
  analogWrite(Motor_R_pwm_pin, y_speed);

  if (analogRead(x_pin) < 490) {
    digitalWrite(Motor_R_dir_pin, HIGH);
    digitalWrite(Motor_L_dir_pin, LOW);
    x_speed = -(analogRead(x_pin) / 512.0) * 255 + 255;
    analogWrite(Motor_R_pwm_pin, x_speed);
  }
  else if (analogRead(x_pin) > 533)
  {
    digitalWrite(Motor_R_dir_pin, LOW);
    digitalWrite(Motor_L_dir_pin, HIGH);
    x_speed = (analogRead(x_pin) / 512.0) * 255 - 255;
    analogWrite(Motor_L_pwm_pin, x_speed);
  }
  else {
    x_speed = 0;
  }


  // Read the current state of outputA
  currentStateA = digitalRead(outputA);

  // If last and current state of outputA are different, then pulse occurred
  // React to only 1 state change to avoid double count
  if (currentStateA != lastStateA  && currentStateA == 1) {
    // If the outputB state is different than the outputA state then
    // the encoder is rotating CCW so decrement
    if (digitalRead(outputB) != currentStateA) {
      counter --;
      currentDir = "ACW";// anti-clockwise
    }
    else {
      // Encoder is rotating CW so increment
      counter ++;
      currentDir = "CW ";//clockwise
    }

    Serial.print("Direction: ");
    Serial.print(currentDir);
    Serial.print(" | Total counter: ");
    Serial.print(counter);// counter for one round is the steps number
    Serial.print("| Average");
    Serial.println(((2 * pi * R) / distance) * counter);

if (counter < 30)
      Serial.println("30");
    else if (counter < 50)
      Serial.println("50");
  }
  // Remember last outputA state
  lastStateA = currentStateA;
  // Put in a slight delay to help debounce the reading
  delay(1);

  if (digitalRead(rstbtn) == LOW) {
    counter = 0;
    //if 50ms have passed since last LOW pulse, it means that the
    //button has been pressed, released and pressed again
    if (millis() - lastButtonPress > 50) {
      Serial.println("Button pressed!");
    }
    // Remember last button press event
    lastButtonPress = millis();
  }
}
