#include <LiquidCrystal.h>
#include <Wire.h>
#include <LIDARLite.h>
#include <math.h>

LIDARLite myLidarLite;

const int rs = 37, en = 36, d4 = 35, d5 = 34, d6 = 33, d7 = 32;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

//===pins layout===
#define Motor_forward 0
#define Motor_return 1
#define Motor_L_dir_pin 7
#define Motor_R_dir_pin 8
#define Motor_L_pwm_pin 9
#define Motor_R_pwm_pin 10

//===interrupts===
#define interrupt_left 2  //Encoder B left
#define interrupt_right 3  //Encoder B right
#define rotationL 24        //Encoder A left
#define rotationR 23        //Encoder A right

//===compass===
#define CMPS14_address 0x60

#define CMPS_ADDR 0x60    // Address of CMPS14 is 0xC0, but it needs to be shifted right one bit for arduino wire library
#define CMPS_ANG 0x01     // Register containing 8bit angle data
#define CMPS_ANG2 0x02    // Register containing 16bit angle data
#define CMPS_PITCH 0x04   // Register containing 8bit pitch data
#define CMPS_ROLL 0x05    // Register containing 8bit roll data
#define CMPS_ROLL2 0x1C   // Register containing 16bit roll data
#define CMPS_CAL_ST 0x1E  // Register containing 8bit calibration state data

uint16_t a16;


#define button_input 2
#define input_voltage 18

//===variables===
int analogPin = A0;
int analogPin2 = A1;
int potentiometer = A2;
int power = 0, rotation = 0;
float value = 0;
float voltage = 0, index_L, index_R;
String vol_string;
double horiz_divider = 0, vert_divider;
double horiz_num, vert_num;
int pressed_counter = 0;
int previous_hor_div, previous_vert_div;
volatile long int pulses = 0, L_counter_prev = 0, R_counter_prev = 0;
float total_brightness = 0;
double index = 0, index_of_power = 0;
volatile bool turnoff = false;
volatile int L_counter, R_counter = 0;
int dist_counter_L, dist_counter_R;
volatile bool carIsMoving = false, forward_L = false, forward_R = false;
int degree;
volatile double dist = 0;  //LIDAR distance variable
volatile double room_width, room_length;
volatile bool measure_length = false;
volatile bool pushed = false;
int volt;
unsigned int adjusted_dist = 0;
int lidar_dist_int;
String send_to_esp32;


//variables for compass
byte high_byte = 0;
byte low_byte = 0;
byte angle8 = 0;
unsigned int angle16 = 0;
int heading = 0;
float voltage_pot;

volatile bool stop_var = true;  // to define if the robot needs to be stopped


//===========SETUP LOOP=================

void setup() {
  Serial.begin(9600);
  Serial3.begin(9600);
  Serial.println("Put a command to the serial monitor.");

  Wire.begin();
  Wire.beginTransmission(CMPS14_address);
  Wire.write(1);
  Wire.endTransmission(false);
  Wire.requestFrom(CMPS14_address, 1, true);

  //LIDAR initialization and configuration
  myLidarLite.begin(0, true);
  myLidarLite.configure(0);


  pinMode(button_input, INPUT_PULLUP);
  pinMode(analogPin, INPUT);
  pinMode(analogPin2, INPUT);
  pinMode(potentiometer, INPUT);
  lcd.begin(20, 4);
  lcd.print("Motivated");
  lcd.setCursor(0, 2);
  lcd.print("Compass = ");

  //===interrupts===
  //attachInterrupt(digitalPinToInterrupt(button_input), buttonpush, FALLING);             //interrupt for button
  attachInterrupt(digitalPinToInterrupt(interrupt_left), interruptLeftMotor, RISING);    //interrupt for LEFT motor
  attachInterrupt(digitalPinToInterrupt(interrupt_right), interruptRightMotor, RISING);  //interrupt for RIGHT motor
}



//===========END OF SETUP LOOP=================



//===========BEGIN OF MAIN LOOP=================

void loop() {

  //motors power
  int pwm_R = 0;
  int pwm_L = 0;

  //displaying positions of cursor on LCD
  lcd.setCursor(0, 0);
  horiz_num = analogRead(A0);  //x-axis
  vert_num = analogRead(A1);   //y-axis

  a16 = CMPS_ReadAngle2();



  dist = distance();

  lcd.setCursor(0, 1);
  lcd.print("                ");
  lcd.setCursor(0, 1);
  lcd.print("Lidar =             ");
  lcd.setCursor(9, 1);
  lcd.print(dist, 1);
  lcd.setCursor(14, 1);
  lcd.print("cm");

  lidar_dist_int = (int)dist;

  horiz_divider = dist / 60 * 19 - 6;
  if (dist < 20) {
    horiz_divider = 0;
  } else if (dist > 77) {
    horiz_divider = 19;
  }

  heading = CMPS_ReadAngle2() / 10;
  lcd.setCursor(10, 2);
  lcd.print("       ");
  lcd.setCursor(10, 2);
  lcd.print(heading);

  //Serial3.print("COM=");  transferring data to ESP
  Serial3.print(lidar_dist_int);
  Serial3.print("+");
  Serial3.println(heading);
  Serial.print(lidar_dist_int);
  Serial.print("+");
  Serial.println(heading);
  //send_to_esp32 = dist + "+" + lidar_dist_int;

  //lcd.print(heading);
  //Serial.print("Raw = ");

  //processing commands from SerialMonitor3 (ESP)
  SerialMonitorCommand();


  //correlation to "I" position on LCD
  horiz_divider = horiz_num / 53, 84;
  vert_divider = vert_num / 53, 84;


  previous_hor_div = horiz_divider;
  previous_vert_div = vert_divider;


  index_of_power = 510 - horiz_num;
  if (index_of_power < 0) {
    index_L = 1;
    index_R = (510 - abs(index_of_power)) / 510;
  } else {
    index_R = 1;
    index_L = (510 - abs(index_of_power)) / 510;
  }


  //defining motors power and direction
  power = 255 - abs(vert_num - 3) / 2;
  if (power < 0) {
    digitalWrite(Motor_R_dir_pin, 0);
    digitalWrite(Motor_L_dir_pin, 1);
  } else {
    digitalWrite(Motor_R_dir_pin, 1);
    digitalWrite(Motor_L_dir_pin, 0);
  }

  analogWrite(Motor_L_pwm_pin, abs(power * index_L));
  analogWrite(Motor_R_pwm_pin, abs(power * index_R));

  delay(150);


  L_counter_prev = L_counter;
  R_counter_prev = R_counter;

  carIsMoving = false;
}

//===========END OF MAIN LOOP=================





//====== FUNCTIONS =======




//measuring distance with LIDAR - meadian values - eliminating 3 value on both ends and then taking the average of the rest 14 values
double distance() {
  double dist[20];
  for (int i = 0; i < 20; i++) {
    dist[i] = myLidarLite.distance(false) - 11;
    //dist = dist + myLidarLite.distance(false);
  }

  int dist_length = sizeof(dist) / sizeof(dist[0]);
  qsort(dist, dist_length, sizeof(dist[0]), sort_desc);

  double average = 0;
  for (int i = 5; i < 15; i++) {
    average += dist[i];
    //dist = dist + myLidarLite.distance(false);
  }

  average = average / 10;

  return average;
}




//Left motor interrupt
void interruptLeftMotor() {
  if (digitalRead(rotationL) == 1) {
    forward_L = true;  //true if the LEFT wheel moves forward
    L_counter++;
  } else {
    forward_L = false;
    forward_R = false;
    L_counter--;
  }
}


//Right motor interrupt
void interruptRightMotor() {

  if (digitalRead(rotationR) == 1) {
    forward_R = true;  //true if the RIGHT wheel moves forward
    R_counter++;
  } else {
    forward_R = false;
    forward_L = false;
    R_counter--;
  }
  carIsMoving = true;
}


//moving straight at a predefined speed - forward or backward
void moveForward(float dist) {
  dist_counter_L = L_counter;
  dist_counter_R = R_counter;

  if (dist < 0) {
    digitalWrite(Motor_R_dir_pin, 0);
    digitalWrite(Motor_L_dir_pin, 0);
  } else {
    digitalWrite(Motor_R_dir_pin, 1);
    digitalWrite(Motor_L_dir_pin, 1);
  }
  sei();  //actovating interrupts
  float initial_compass = CMPS_ReadAngle2() / 10;
  //int heading;
  while (abs(R_counter - dist_counter_R) < 13 * (abs(dist)) && abs(L_counter - dist_counter_L) < 13 * (abs(dist))) {

    heading = CMPS_ReadAngle2() / 10;

    if (abs(R_counter - dist_counter_R) < 0.97 * (12.9 * (abs(dist))) || abs(L_counter - dist_counter_L) < 0.97 * (12.9 * (abs(dist))) || abs(R_counter - dist_counter_R) > 0.00 * (12.9 * (abs(dist))) || abs(L_counter - dist_counter_L) > 0.00 * (12.9 * (abs(dist)))) {

      if (heading > initial_compass) {
        analogWrite(Motor_L_pwm_pin, 100);
        analogWrite(Motor_R_pwm_pin, 100);
      } else if (heading < initial_compass) {
        analogWrite(Motor_L_pwm_pin, 100);
        analogWrite(Motor_R_pwm_pin, 100);
      } else if (heading == initial_compass) {
        analogWrite(Motor_L_pwm_pin, 100);
        analogWrite(Motor_R_pwm_pin, 100);
      }

    } else {
      if (heading > initial_compass) {
        analogWrite(Motor_L_pwm_pin, 100);
        analogWrite(Motor_R_pwm_pin, 100);
      } else if (heading < initial_compass) {
        analogWrite(Motor_L_pwm_pin, 100);
        analogWrite(Motor_R_pwm_pin, 100);
      } else if (heading == initial_compass) {
        analogWrite(Motor_L_pwm_pin, 100);
        analogWrite(Motor_R_pwm_pin, 100);
      }

      //analogWrite(Motor_L_pwm_pin,220);
      //analogWrite(Motor_R_pwm_pin,220);
    }
  }
}







//stopping the robot function
void stopping() {
  analogWrite(Motor_L_pwm_pin, 0);
  analogWrite(Motor_R_pwm_pin, 0);
  stop_var = true;
}




//turning left 90 degrees using pulses - just for testing



//turning right 90 degrees using pulses - just for testing


//defining the current direction using compass raw values
int compass_position() {
  float temp = 0;
  Wire.begin();
  Wire.beginTransmission(CMPS14_address);
  Wire.write(1);
  Wire.endTransmission(false);
  Wire.requestFrom(CMPS14_address, 1, true);

  if (Wire.available() >= 1) {
    byte raw_value = Wire.read();
    //Serial.print("Compass = ");
    //Serial.println(raw_value);
    temp = float(raw_value);
    temp = temp * 360;
    temp = temp / 255;
  }

  return int(temp);
}






//turn RIGHT by amount of degrees
void turn_x_degrees_R(int deg, int initial) {
  //compass_position();
  int initial_angle = initial, current_angle = 0, passed = 0;
  Serial.print("Initial: ");
  Serial.println(initial_angle);

  digitalWrite(Motor_R_dir_pin, 1);
  digitalWrite(Motor_L_dir_pin, 0);
  sei();
  stop_var = false;

  while (passed + 5 < deg && !stop_var) {
    //Serial.println(stop_var);
    analogWrite(Motor_R_pwm_pin, 1);
    analogWrite(Motor_L_pwm_pin, 100);
    current_angle = CMPS_ReadAngle2() / 10;
    if ((current_angle - initial_angle) >= -5) {
      passed = current_angle - initial_angle;
      passed = passed * 1.03;
    } else {
      passed = (360 - initial_angle + current_angle);
      passed = passed * 1.03;
    }
    delay(20);
  }
  analogWrite(Motor_L_pwm_pin, 0);
  Serial.print("Current: ");
  Serial.println(current_angle);
}




//turn LEFT by amount of degrees
void turn_x_degrees_L(int deg, int initial) {
  //compass_position();
  int initial_angle = initial, current_angle = 0, passed = 0;
  Serial.print("Initial: ");
  Serial.println(initial_angle);

  digitalWrite(Motor_R_dir_pin, 1);
  digitalWrite(Motor_L_dir_pin, 0);
  sei();
  stop_var = false;

  while (passed < deg - 5 && !stop_var) {
    analogWrite(Motor_R_pwm_pin, 100);
    analogWrite(Motor_L_pwm_pin, 1);
    current_angle = CMPS_ReadAngle2() / 10;
    if ((initial_angle - current_angle) >= -10) {
      passed = initial_angle - current_angle;
      passed = passed * 1.03;
    } else {
      passed = (360 - current_angle + initial_angle);
      passed = passed * 1.03;
    }
    delay(20);
  }
  analogWrite(Motor_L_pwm_pin, 0);
  Serial.print("Current: ");
  Serial.println(current_angle);
}



//turn to specific direction - we just put required DEGREE 0-360 and choose the best direction - clockwise or CCW
//=====NOT USED at the moment===== JUST FOR TESTING
void turn_specific_direction(int deg, int initial) {

  int initial_angle = initial, final_angle = deg, passed = 0, difference = 0;

  if ((initial_angle - final_angle) < 0) {
    difference = 360 - (final_angle - initial_angle);
  } else difference = initial_angle - final_angle;

  if (difference < 180)  //then turn left
  {
    digitalWrite(Motor_R_dir_pin, 1);
    digitalWrite(Motor_L_dir_pin, 0);
    sei();

    while (abs(compass_position() - (deg)) > 4) {
      //Serial.print("Current: ");
      //Serial.println(current);
      analogWrite(Motor_R_pwm_pin, 150);
      analogWrite(Motor_L_pwm_pin, 0);
      delay(10);
    }


  }

  else  //then turn right
  {
    digitalWrite(Motor_R_dir_pin, 1);
    digitalWrite(Motor_L_dir_pin, 0);
    sei();

    while (abs(compass_position() - (deg)) > 4) {
      //Serial.print("Current: ");
      //Serial.println(current);
      analogWrite(Motor_R_pwm_pin, 0);
      analogWrite(Motor_L_pwm_pin, 150);
      delay(10);
    }
  }
}



//processing commands on the Serial Monitor
void SerialMonitorCommand() {


  //Checking Serial Monitor 3 (ESP)    RECEIVING FROM ESP8266
  if (Serial3.available() > 0) {
    String message = Serial3.readStringUntil('\n');
    //Serial3.print("Message from ESP: ");
    //Serial3.println(message);
    Serial.print("Message from ESP: ");
    Serial.println(message);
    bool double_command = false;


    int pos_s = message.indexOf(",");



    if (!double_command) {

      //checking PWM command
      pos_s = message.indexOf("Move");

      if (pos_s > -1) {
        Serial.println("Command = Move ");
        pos_s = message.indexOf(":");

        if (pos_s > -1) {
          String stat = message.substring(pos_s + 1);
          int driveforward = stat.toInt();
          moveForward(driveforward);
        }
      }



      pos_s = message.indexOf("Turn");

      if (pos_s > -1) {
        Serial.println("Command = Turn ");
        pos_s = message.indexOf(":");
        Serial.println(pos_s);

        if (pos_s > -1) {
          String stat = message.substring(pos_s + 1);
          int angle = stat.toInt();
          heading = CMPS_ReadAngle2() / 10;
          if (angle > 0) {
            turn_x_degrees_R(angle, heading);
            //turnRight();
          } else {
            turn_x_degrees_L(abs(angle), heading);
            //turnLeft();
            Serial.println(angle);
          }
        }
      }
    }
  }





  //not using SM commands on Arduino at the moment
  //Checking Serial Monitor 1 (Arduino)
  if (Serial.available() > 0) {
    String message = Serial.readStringUntil('\n');
    Serial.print("Message received, content: ");
    Serial.println(message);

    int pos_s = message.indexOf("Move");

    if (pos_s > -1) {
      Serial.println("Command = Move ");
      pos_s = message.indexOf(":");
      Serial.println(pos_s);

      if (pos_s > -1) {
        String stat = message.substring(pos_s + 1);
        int driveforward = stat.toInt();
        moveForward(driveforward);
      }
    }




    //if it's NOT double command on SM Arduino
    else {

      int pos_s = message.indexOf("Turn");

      if (pos_s > -1) {
        Serial.println("Command = Turn ");
        pos_s = message.indexOf(":");
        Serial.println(pos_s);

        if (pos_s > -1) {
          String stat = message.substring(pos_s + 1);
          int angle = stat.toInt();
          if (angle > 0) turn_x_degrees_R(angle, heading);
          else {
            turn_x_degrees_L(abs(angle), heading);
            Serial.println(angle);
          }
        }
      }
    }
  }
}


int sort_desc(const void *cmp1, const void *cmp2) {
  // Need to cast the void * to int *
  double a = *((double *)cmp1);
  double b = *((double *)cmp2);
  // The comparison
  return a > b ? -1 : (a < b ? 1 : 0);
  // A simpler, probably faster way:
  //return b - a;
}



uint16_t CMPS_ReadAngle2(void) {
  return CMPS_ReadData(CMPS_ANG2, 2);
}



int16_t CMPS_ReadData(int16_t reg, int16_t n) {
  uint8_t tmp8 = 0;
  int16_t tmp16 = 0;

  Wire.beginTransmission(CMPS_ADDR);
  Wire.write(reg);
  tmp8 = Wire.endTransmission();
  if (tmp8) {
    Serial.print("Error in I2C bus transmission: code ");
    Serial.println(tmp8);
  } else {
    tmp8 = Wire.requestFrom(CMPS_ADDR, n);
    if (tmp8 != n) {
      Serial.print("Incorrect response from I2C bus! Expected bytes: ");
      Serial.print(n);
      Serial.print(", received: ");
      Serial.println(tmp8);
    } else {
      if (n > 1) {
        tmp16 = Wire.read();
        tmp8 = Wire.read();
        tmp16 <<= 8;
        tmp16 += tmp8;
      } else {
        tmp8 = Wire.read();
        tmp16 = tmp8;
      }
    }
  }
  return tmp16;
}
