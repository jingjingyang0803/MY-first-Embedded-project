// Create program that drives your vehicle:- 30 cm forward,- turns 180 degree- returns back to original place

#define Motor_forward         0
#define Motor_return          1
#define Motor_L_dir_pin       7
#define Motor_R_dir_pin       8
#define Motor_L_pwm_pin       9
#define Motor_R_pwm_pin       10

int speed = 100;

void setup()
{
  digitalWrite(Motor_R_dir_pin, Motor_forward);
  digitalWrite(Motor_L_dir_pin, Motor_forward);
  analogWrite(Motor_L_pwm_pin, speed);
  analogWrite(Motor_R_pwm_pin, speed);
  delay(4300);
  analogWrite(Motor_L_pwm_pin, 0);
  delay(6000);
  analogWrite(Motor_L_pwm_pin, speed);
  delay(4300);
  analogWrite(Motor_L_pwm_pin, 0);
  analogWrite(Motor_R_pwm_pin, 0); 
}

void loop()
{

}
