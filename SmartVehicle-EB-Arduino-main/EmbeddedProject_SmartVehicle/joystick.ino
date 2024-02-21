// https://www.bing.com/videos/search?q=JOYSTICK+ARDUINO+TURNING&docid=608013180286818736&mid=B029D5D80360AFE375C9B029D5D80360AFE375C9&view=detail
#define Motor_forward         0
#define Motor_return          1
#define Motor_L_dir_pin       7
#define Motor_R_dir_pin       8
#define Motor_L_pwm_pin       9
#define Motor_R_pwm_pin       10

volatile int buttonPin = 19;     // the number of the pushbutton pin
const int x_pin = A8, y_pin = A9;//horizontal/vertical analog pin for the joystick
int x_speed = 0; //turning speed
int y_speed = 0;


void setup()
{
  Serial.begin(9600);
  pinMode(buttonPin, INPUT);
  pinMode(Motor_L_dir_pin, OUTPUT);
  pinMode(Motor_R_dir_pin, OUTPUT);
  pinMode(Motor_L_pwm_pin, OUTPUT);
  pinMode(Motor_R_pwm_pin, OUTPUT);
}

void loop()
{
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


  if (analogRead(y_pin) < 490) {
    digitalWrite(Motor_R_dir_pin, Motor_return);
    digitalWrite(Motor_L_dir_pin, Motor_return);
    y_speed = -(analogRead(y_pin) / 512.0) * 255 + 255;
  }
  else if (analogRead(y_pin) > 533)
  {
    digitalWrite(Motor_R_dir_pin, Motor_forward);
    digitalWrite(Motor_L_dir_pin, Motor_forward);
    y_speed = (analogRead(y_pin) / 512.0) * 255 - 255;

  }
  analogWrite(Motor_L_pwm_pin, y_speed);
  analogWrite(Motor_R_pwm_pin, y_speed);
}
