// Create traffic lights, Use red/yellow/green leds, Use register control for leds
// prints to LCD-display the voltage of potentiometer and the voltage of 3.3V pin of Mega

/*
  The circuit:
  - potentiometer
    center pin of the potentiometer to the analog input 1
    one side pin (either one) to ground
    the other side pin to +5V
  - LED
    anode (long leg) attached to digital output 13 through 220 ohm resistor
    cathode (short leg) attached to ground
*/

// include the library code:
#include <LiquidCrystal.h>

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 37, en = 36, d4 = 35, d5 = 34, d6 = 33, d7 = 32;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

int red = 2, yellow = 3, green = 4;

void setup() {
  pinMode(red, OUTPUT);
  pinMode(yellow, OUTPUT);
  pinMode(green, OUTPUT);
}
void loop() {
  //create traffic lights: yellow 3s, red 5s, red+yello 2s, green 3s.
  changeLights();
  
  // set up the LCD's number of columns and rows:
  lcd.begin(20, 4);

  // read the input on analog pin 1 and 2:
  int sensorValue1 = analogRead(A0);
  int sensorValue2 = analogRead(A2);
 
  // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
  float voltage1 = sensorValue1 * (5.0 / 1023.0);
  // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 3.3V):
  float voltage2 = sensorValue2 * (5.0 / 1023.0);
 
  // print out the value you read:
  lcd.setCursor(0, 0);
  lcd.print("Pot.meter=");
  lcd.print(voltage1);
  lcd.setCursor(14, 0);
  lcd.print("V");
 
  lcd.setCursor(0, 1);
  lcd.print("3.3 level=");
  lcd.print(voltage2);
  lcd.setCursor(14, 1);
  lcd.print("V");

}
void changeLights() {
  // green off, yellow on for 3 seconds
  digitalWrite(green, LOW);
  digitalWrite(yellow, HIGH);
  delay(3000);
  // turn off yellow, then turn red on for 5 seconds
  digitalWrite(yellow, LOW);
  digitalWrite(red, HIGH);
  delay(5000);
  // red and yellow on for 2 seconds (red is already on though)
  digitalWrite(yellow, HIGH);
  delay(2000);
  // turn off red and yellow, then turn on green for 3 seconds
  digitalWrite(yellow, LOW);
  digitalWrite(red, LOW);
  digitalWrite(green, HIGH);
  delay(3000);
}
