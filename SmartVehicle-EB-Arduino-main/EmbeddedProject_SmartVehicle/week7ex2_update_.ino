/*
  String to Integer conversion

  Reads a serial input string until it sees a newline, then converts the string
  to a number if the characters are digits.

  The circuit:
  - No external components needed.

  created 29 Nov 2010
  by Tom Igoe

  This example code is in the public domain.

  https://www.arduino.cc/en/Tutorial/BuiltInExamples/StringToInt
*/
#define Motor_forward         0
#define Motor_L_dir_pin       7
#define Motor_R_dir_pin       8
#define Motor_L_pwm_pin       9
#define Motor_R_pwm_pin       10

// Rotary Encoder Inputs
//////Right motor
#define outputA 24     //CLK to D24
#define outputB 2     //DT to D2

int counter = 0;// amount of pulses
int currentStateA;
int lastStateA;
const int steps_per_cm = 10;
const int steps_per_degree = 2;
int y_speed = 100;

void setup()
{
  // Set encoder pins as inputs
  pinMode(outputA, INPUT);
  pinMode(outputB, INPUT);
  // Read the initial state of outputA
  lastStateA = digitalRead(outputA);
  // Open serial communications
  Serial.begin(9600);
}

void loop()
{
  if (counter < 30 * steps_per_cm) {
    digitalWrite(Motor_R_dir_pin, Motor_forward );
    digitalWrite(Motor_L_dir_pin, Motor_forward );
    analogWrite(Motor_R_pwm_pin, y_speed);
    analogWrite(Motor_L_pwm_pin, y_speed);
  }
  else if (counter < 30 * steps_per_cm + 180 * steps_per_degree) {
    digitalWrite(Motor_R_dir_pin, Motor_forward );
    digitalWrite(Motor_L_dir_pin, Motor_forward );
    analogWrite(Motor_R_pwm_pin, y_speed);
    analogWrite(Motor_L_pwm_pin, 0);
  }
  else if (counter < 60 * steps_per_cm + 180 * steps_per_degree) {
    digitalWrite(Motor_R_dir_pin, Motor_forward );
    digitalWrite(Motor_L_dir_pin, Motor_forward );
    analogWrite(Motor_R_pwm_pin, y_speed);
    analogWrite(Motor_L_pwm_pin, y_speed);
  }
  else {
    analogWrite(Motor_R_pwm_pin, 0);
    analogWrite(Motor_L_pwm_pin, 0);
  }


  currentStateA = digitalRead(outputA);
  if (currentStateA != lastStateA && currentStateA == 1) {
    counter ++;
    Serial.print("counter: ");
    Serial.println(counter);
  }
  lastStateA = currentStateA;
  delay(1);
}
