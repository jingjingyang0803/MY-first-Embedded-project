//Create program that drives you vehicle:
//- 30 cm forward,
//- turns 180 degree
//- returns back to original place


//Create program that drives you vehicle:
//- 3 cm forward or return, each time when turning joystick full  to front or back ( next 3cm drive possible only after joystick returned back to middle)
//- by giving left or right pulse by joystick, car turns 22.5° left or right( next turn possible only after joystick returned back to middle)

#define Motor_forward         0
#define Motor_return          1
#define Motor_L_dir_pin       7
#define Motor_R_dir_pin       8
#define Motor_L_pwm_pin       9
#define Motor_R_pwm_pin       10

// Mega2560 ISR pin: D2,D3,D18,D19,D20,D21
#define outputA 2  //CLK pin
#define outputB 3  //DT pin

int counter = 0;// amount of pulses
const float pi = 3.14; // Pi value

const int R = 3;  //Radius of the wheel from center to edge // under measure!!
const int N = 1600; //no of steps for one rotation            // under measure!!
float distance = 0;

void setup()
{
  Serial.begin(9600);
  pinMode(buttonPin, INPUT);
  pinMode(Motor_L_dir_pin, OUTPUT);
  pinMode(Motor_R_dir_pin, OUTPUT);
  pinMode(Motor_L_pwm_pin, OUTPUT);
  pinMode(Motor_R_pwm_pin, OUTPUT);
  // Initialize encoder pins
  pinMode(outputA, INPUT);
  pinMode(outputB, INPUT);
  pinMode(rstbtn, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(outputA), readEncoder, FALLING);
}

void loop()
{
  Serial.println(distance);
  // The distance travelled for one step of Encoder = (2xπxR)/N
  distance = ((2 * pi * R) / N) * getCounter();

  analogWrite(Motor_L_pwm_pin, 90);
  analogWrite(Motor_R_pwm_pin, 90);
  if (distance < 3) {
    digitalWrite(Motor_R_dir_pin, Motor_forward);
    digitalWrite(Motor_L_dir_pin, Motor_forward);
  }
  if (distance < pi * R) {
    digitalWrite(Motor_R_dir_pin, HIGH);
    digitalWrite(Motor_L_dir_pin, LOW);
  }
  counter = 0;
  if (distance < 3) {
    digitalWrite(Motor_R_dir_pin, Motor_forward);
    digitalWrite(Motor_L_dir_pin, Motor_forward);
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
