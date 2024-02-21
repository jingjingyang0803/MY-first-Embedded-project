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
#define Motor_return          1
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
const int steps_per_cm = 20;
String message = "";
int distance = 0;
int target_steps = 0;
int y_move = 0;
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
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  // send an intro:
  Serial.println("Write dist:*somenumberhere(0-20)* to the serial monitor.");
}

void loop()
{
  // Read serial input:
  while (Serial.available() > 0) {
    counter = 0;
    message = Serial.readStringUntil('\n');
    int distance = message.substring(message.indexOf(":") + 1).toInt();

    Serial.print("distance: ");
    Serial.print(distance);

    target_steps = abs(steps_per_cm * distance);
    Serial.print("\ttarget_steps: ");
    Serial.print(target_steps);

    if ( distance > 0) {
      y_move = Motor_forward;
      Serial.println("\t\tMotor_forward");
    }
    else if (distance < 0) {
      y_move = Motor_return;
      Serial.println("\t\tMotor_return");
    }
  }


  if (counter < target_steps) {
    digitalWrite(Motor_R_dir_pin, y_move);
    digitalWrite(Motor_L_dir_pin, y_move);
    analogWrite(Motor_L_pwm_pin, y_speed);
    analogWrite(Motor_R_pwm_pin, y_speed);
  }
  else {
    analogWrite(Motor_L_pwm_pin, 0);
    analogWrite(Motor_R_pwm_pin, 0);
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
