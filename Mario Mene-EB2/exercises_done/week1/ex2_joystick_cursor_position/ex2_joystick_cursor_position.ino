/*
  reference: https://learn.sparkfun.com/tutorials/thumb-joystick-hookup-guide/all

*/

// include the library code:
#include <LiquidCrystal.h>
//#include <math.h>

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 37, en = 36, d4 = 35, d5 = 34, d6 = 33, d7 = 32;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// constants won't change. They're used here to set pin numbers:
volatile int buttonPin = 19;     // the number of the pushbutton pin
const int x_pin = A8;
int x_value = 0;
int cursorplace = 10;

void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(20, 4);

  lcd.setCursor(0, 0);
  lcd.print("X-position");
  lcd.setCursor(0, 1);
  //         01234567890123456789
  lcd.print("-100%    0%    +100%");

  Serial.begin(9600);

  // With a function called attachInterrupt(), we can execute an ISR
  // when the state of the Arduino io-pin changes.
  // Attach an interrupt to the ISR vector // Pin 19, Routine:button_pressed_ISR, FALLING Edge
  attachInterrupt(digitalPinToInterrupt(buttonPin), button_pressed_ISR, FALLING);
}

void loop() {
  x_value = analogRead(x_pin);
  cursorplace = x_value / 1023.0 * 20 - 1 == -1 ? 0 : x_value / 1023.0 * 20 ;
  Serial.println(cursorplace);// to check is range from 0 to 19
  lcd.setCursor(cursorplace, 2);
  lcd.print("|");
  delay(50);
  lcd.setCursor(cursorplace, 2);
  lcd.print(" ");
}

void button_pressed_ISR() {
  // print "pressed" on serial monitor:
  Serial.println("pressed");

//  // print "button pressed" on LCD
//  lcd.setCursor(0, 3);
//  lcd.print("buttton pressed");
//  delay(800);
//  lcd.setCursor(0, 3);
//  lcd.print("               ");
}
