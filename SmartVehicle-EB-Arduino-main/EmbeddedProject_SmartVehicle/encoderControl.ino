#define Motor_forward         0      // CW
#define Motor_return          1      // ACW
#define Motor_L_dir_pin       7
#define Motor_R_dir_pin       8
#define Motor_L_pwm_pin       9
#define Motor_R_pwm_pin       10
#define rstbtn                19

// Mega2560 ISR pin: D2,D3,D18,D19,D20,D21
#define Encoder_R_CLK_pin     24
#define Encoder_R_DT_pin      2
#define Encoder_L_CLK_pin     23
#define Encoder_L_DT_pin      3

const float pi = 3.14; // Pi value
const int R = 3;       //Radius of the wheel from center to edge

// under measure!!
// test pulses when car go one meter, test pulses when car turn full
const int steps_per_meter = 1000; //number of steps for one rotation
const int steps_per_degree = 100;

void setup()
{
  Serial.begin(9600);
  pinMode(buttonPin, INPUT);
  pinMode(Motor_L_dir_pin, OUTPUT);
  pinMode(Motor_R_dir_pin, OUTPUT);
  pinMode(Motor_L_pwm_pin, OUTPUT);
  pinMode(Motor_R_pwm_pin, OUTPUT);
  // Initialize encoder pins
  pinMode(Encoder_R_CLK_pin, INPUT);
  pinMode(Encoder_R_DT_pin, INPUT);
  pinMode(Encoder_L_CLK_pin, INPUT);
  pinMode(Encoder_L_DT_pin, INPUT);
  pinMode(rstbtn, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(Encoder_R_CLK_pin), readEncoder, FALLING);

  forward(30, 150);
  turnRight(180, 100);
  return (30, 150);
  turnLeft(180, 100);

  //Create program that drives you vehicle:
  //- 3 cm forward or return, each time when turning joystick full  to front or back ( next 3cm drive possible only after joystick returned back to middle)
  //- by giving left or right pulse by joystick, car turns 22.5° left or right( next turn possible only after joystick returned back to middle)
}

void loop()
{
  Serial.println(getCounter());
}

//set int distance, unit in cm
//set int degree, 0-180
//set int speed, 0-255
//get stepsToGo, amount of pulses for a specific distance or degree
void forward(distance, run_speed) {
  int stepsToGo = distance * steps_per_meter;
  while (getCounter() < stepsToGo) {
    digitalWrite(Motor_R_dir_pin, Motor_forward);
    digitalWrite(Motor_L_dir_pin, Motor_forward);
    analogWrite(Motor_L_pwm_pin, run_speed);
    analogWrite(Motor_R_pwm_pin, run_speed);
  }
}
void return (distance, run_speed) {
  int stepsToGo = distance * steps_per_meter;
  while (getCounter() < stepsToGo) {
    digitalWrite(Motor_R_dir_pin, Motor_return);
    digitalWrite(Motor_L_dir_pin, Motor_return);
    analogWrite(Motor_L_pwm_pin, run_speed);
    analogWrite(Motor_R_pwm_pin, run_speed);
  }
}
void turnRight(degree, turn_speed) {
  int stepsToGo = distance * steps_per_degree;
  while (getCounter() < stepsToGo) {
    digitalWrite(Motor_R_dir_pin, Motor_forward);
    analogWrite(Motor_R_pwm_pin, turn_speed);
    analogWrite(Motor_L_pwm_pin, 0);
  }
}
void turnLeft(degree, turn_speed) {
  int stepsToGo = distance * steps_per_degree;
  while (getCounter() < stepsToGo) {
    digitalWrite(Motor_L_dir_pin, Motor_forward);
    analogWrite(Motor_L_pwm_pin, turn_speed);
    analogWrite(Motor_R_pwm_pin, 0);
  }
}
void readEncoder() {
  //read state of outputB
  //if state B is still 1 when state A falls to 0
  // the encoder is rotating CW so increment
  if (digitalRead(outputB) == HIGH) {
    counter++; // Clockwise
  }
  //if state B is already 0 when state A falls to 0
  // the encoder is rotating CCW so decrement
  if (digitalRead(outputB) == LOW) {
    counter--; // Counterclockwise
  }
}

// Get the counter value, disabling interrupts.
// This make sure readEncoder() doesn't change the value
// while we're reading it.
int getCounter() {
  noInterrupts();
  int result = counter;
  interrupts();
  return result;
}
