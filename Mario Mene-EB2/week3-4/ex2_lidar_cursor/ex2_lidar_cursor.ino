// https://github.com/garmin/LIDARLite_Arduino_Library/blob/master/examples/v3/GetDistanceI2c/GetDistanceI2c.ino
// https://github.com/garmin/LIDARLite_Arduino_Library/issues/14


// include the library code:
#include <LiquidCrystal.h>

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 37, en = 36, d4 = 35, d5 = 34, d6 = 33, d7 = 32;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

int distance, cursorplace;

#include <Wire.h>
#include <LIDARLite.h>

LIDARLite myLidarLite;

void setup()
{
  Wire.begin();
  lcd.begin(20, 4);

  lcd.setCursor(10, 0);
  lcd.print("Distance: ");
  lcd.print(distance);
  lcd.print(" cm");

  lcd.setCursor(0, 2);
  //         01234567890123456789
  lcd.print("20    40    60    80");


  Serial.begin(9600); // Initialize serial connection to display distance readings

  myLidarLite.begin(0, true); // Set configuration to default and I2C to 400 kHz

  myLidarLite.configure(0); // Change this number to try out alternate configurations
}

void loop()
{
  distance = myLidarLite.distance(false);
  // Take a measurement with receiver bias correction and print to serial terminal
  Serial.println();   // unit:cm????????????

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  if (distance <= 20)
    cursorplace = 0;
  else if (distance < 80)
    cursorplace = (distance - 20) / 3 - 1; //move cursor to the next column for every 3cm, (80-60)/20=3cm
  else
    cursorplace = 19;

  lcd.setCursor(cursorplace, 1);
  lcd.print("*");
  delay(50);
  lcd.setCursor(cursorplace, 1);
  lcd.print(" ");
}
