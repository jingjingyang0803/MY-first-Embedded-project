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

#define CMPS14_ADDRESS 0x60
byte raw;

const int rs = 37, en = 36, d4 = 35, d5 = 34, d6 = 33, d7 = 32;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

const int pot_max_value = 982;
const int pot_min_value = 19;
const float distance_per_pot_step = 20.0 / (pot_max_value - pot_min_value);
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

int initialAngle = 0;
int currentAngle = 0;
int targetAngle = 0;
bool isRobotTurning = false;
bool isRobotTurningLeft = false;
bool isRobotTurningRight = false;

bool commenceSequence = false;
bool stage1completed = false;
bool stage2completed = false;
bool stage3completed = false;
bool stage4completed = false;
bool stage5completed = false;
bool stage6completed = false;
bool stage7completed = false;
bool sequenceCompleted = false;

int stageDelay = 3000;

void setup()
{
  Serial.begin(9600); // Initialize serial connection to display distance readings

  lcd.begin(20, 4);
  Wire.begin();
  lidarLite.begin(0, true); // Set configuration to default and I2C to 400 kHz
  lidarLite.configure(1); // Change this number to try out alternate configurations
}



bool moveUntil(int targetDistance) {
  
  bool returnVal = false;
  adjustedDist = targetDistance;

  if (abs(targetDistance - measuredDist) < 6) {

    y_speed = 0;
    motorStatus = "OFF";
    returnVal = true;

  } else if ((targetDistance - measuredDist) < -5) {

    digitalWrite(Motor_R_dir_pin, Motor_forward);
    digitalWrite(Motor_L_dir_pin, Motor_forward);

    y_speed = 100;
    motorStatus = "RUN FW";
    
    Serial.println("forward");

  } else if ((targetDistance - measuredDist) > 5) {

    digitalWrite(Motor_R_dir_pin, Motor_return);
    digitalWrite(Motor_L_dir_pin, Motor_return);

    y_speed = 100;
    motorStatus = "RUN RET";

    Serial.println("back");
    
  }

  analogWrite(Motor_L_pwm_pin, y_speed);
  analogWrite(Motor_R_pwm_pin, y_speed);

  return returnVal;
}



bool turnFor(int angular) {
  
  if(!isRobotTurning) {

    isRobotTurning = true;
    initialAngle = currentAngle;
    targetAngle = initialAngle + angular;

    // Start turning the robot - no need to check the condition here,
    // since this method only runs once when you move the joystick
    if (angular > 0) {  // turn right
      isRobotTurningRight = true;  
      digitalWrite(Motor_L_dir_pin, Motor_forward);
      analogWrite(Motor_L_pwm_pin, run_speed);
    } else if (angular < 0) { // turn left
      isRobotTurningLeft = true;  
      digitalWrite(Motor_R_dir_pin, Motor_forward);
      analogWrite(Motor_R_pwm_pin, run_speed);
    }
    
  } else {
    Serial.println(String(targetAngle) + " " + String(currentAngle));
    
    if((isRobotTurningRight && currentAngle >= targetAngle) ||
      isRobotTurningLeft && currentAngle <= targetAngle) {

      analogWrite(Motor_R_pwm_pin, 0);
      analogWrite(Motor_L_pwm_pin, 0);
      
      isRobotTurning = false;
      isRobotTurningRight = false;
      isRobotTurningLeft = false;
    }

  }

  return !isRobotTurning;
}



void loop()
{

  //================== Compass ===================
  Wire.beginTransmission(CMPS14_ADDRESS);  //starts communication with CMPS14
  Wire.write(1);                     //Sends the register 1 from where we wish to start reading heading value
  Wire.endTransmission(false);       // keep connection active

  // Request one byte from the CMPS14
  // this will give us the 8 bit bearing
  delay(1);
  Wire.requestFrom(CMPS14_ADDRESS, 1);

  // Wait for all bytes to come back
  if (Wire.available() >= 1) {
    raw = Wire.read();              // 0~255
  }
  currentAngle =  360.0 * raw / 255.0;
  
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
  potDist = ((potValue - pot_min_value) * distance_per_pot_step) + 1;
  adjustedDist = fixedDist + potDist;

  String msg = "null";
  while (Serial.available() > 0) {
    msg = Serial.readStringUntil('\n');
  }

  if (msg == "START") {
    commenceSequence = true;
    sequenceCompleted = false;
  }

  if (!commenceSequence) {
    Serial.println(currentAngle);
  }


  //=============== Motor control ==================

  if (commenceSequence && !stage1completed)  {

    stage1completed = moveUntil(20);

    if(stage1completed) {
      Serial.println("stage 1 completed, commencing stage 2 in " + String(stageDelay / 1000) + " seconds ...");
      delay(stageDelay);
    }
    
  } else if (stage1completed && !stage2completed) {
    stage2completed = turnFor(-90);

    if(stage2completed) {
      Serial.println("stage 2 completed, commencing stage 3 in " + String(stageDelay / 1000) + " seconds ...");
      delay(stageDelay);
    }

  } else if (stage2completed && !stage3completed) {
    stage3completed = moveUntil(25);

    if(stage3completed) {
      Serial.println("stage 3 completed, commencing stage 4 in " + String(stageDelay / 1000) + " seconds ...");
      delay(stageDelay);
    }

  } else if (stage3completed && !stage4completed) {
    stage4completed = turnFor(-90);

    if(stage4completed) {
      Serial.println("stage 4 completed, commencing stage 5 in " + String(stageDelay / 1000) + " seconds ...");
      delay(stageDelay);
    }

  } else if (stage4completed && !stage5completed) {
    stage5completed = moveUntil(20);

    if(stage5completed) {
      Serial.println("stage 5 completed, commencing stage 6 in " + String(stageDelay / 1000) + " seconds ...");
      delay(stageDelay);
    }

  } else if (stage5completed && !stage6completed) {
    stage6completed = turnFor(-90);

    if(stage6completed) {
      Serial.println("stage 6 completed, commencing stage 7 in " + String(stageDelay / 1000) + " seconds ...");
      delay(stageDelay);
    }

  } else if (stage6completed && !stage7completed) {
    stage7completed = moveUntil(25);

    if(stage7completed) {
      Serial.println("stage 7 completed, terminating sequence ...");
      commenceSequence = false;
      stage1completed = false;
      stage2completed = false;
      stage3completed = false;
      stage4completed = false;
      stage5completed = false;
      stage6completed = false;
      stage7completed = false;
      sequenceCompleted = true;
    }

  }


  //=============== LCD control ==================
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Adjusted: " + String(adjustedDist) + " cm");
  lcd.setCursor(0, 1);
  lcd.print("Measured: " + String(measuredDist) + " cm");
  lcd.setCursor(0, 2);
  lcd.print("Motor:    " + String(motorStatus));
  if (sequenceCompleted) {
    lcd.setCursor(0, 3);
    lcd.print("SEQUENCE COMPLETED");
  }

  // Increment reading counter
  cal_cnt++;
  cal_cnt = cal_cnt % 100;

  delay(100);
}