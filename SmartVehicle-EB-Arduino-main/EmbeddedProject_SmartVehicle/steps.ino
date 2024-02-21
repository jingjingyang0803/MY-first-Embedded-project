// https://www.circuitschools.com/rotary-encoder-with-arduino-in-detail-with-example-codes/#Arduino_Rotary_encoder_Example_code_to_read_position_and_direction_of_Rotation
// https://lastminuteengineers.com/rotary-encoder-arduino-tutorial/

//DT (Output B) is similar to CLK output, but it lags behind CLK by a 90° phase shift. 
//This output is used to determine the direction of rotation.
//CLK (Output A) is the primary output pulse to determine the amount of rotation. 
//Each time the knob is turned in either direction by just one detent (click), the ‘CLK’ output goes through one cycle of going HIGH and then LOW.

// Rotary Encoder Inputs
///////Left motor
//#define outputA 23     //CLK to D23
//#define outputB 3     //DT to D3

//////Right motor
#define outputA 24     //CLK to D24
#define outputB 2     //DT to D2
//#define SW 19

// include the library code:
#include <LiquidCrystal.h>

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 37, en = 36, d4 = 35, d5 = 34, d6 = 33, d7 = 32;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

int counter = 0;
int currentStateA;
int lastStateA;
String currentDir = "";

void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(20, 4);
  // Setup Serial Monitor
  Serial.begin(9600);

  // Set encoder pins as inputs
  pinMode(outputA, INPUT);
  pinMode(outputB, INPUT);
  
  // Read the initial state of outputA
  lastStateA = digitalRead(outputA);
}

void loop() {
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
    Serial.print(" | Counter: ");
    Serial.println(counter);// counter for one round is the steps number

    lcd.setCursor(0, 0);
    lcd.print("Position: ");
    lcd.setCursor(10, 0);
    lcd.print(counter);

    lcd.setCursor(0, 1);
    lcd.print("Dir: ");
    lcd.setCursor(5, 1);
    lcd.print(currentDir);
  }

  // Remember last outputA state
  lastStateA = currentStateA;

  // Put in a slight delay to help debounce the reading
  delay(1);
}
