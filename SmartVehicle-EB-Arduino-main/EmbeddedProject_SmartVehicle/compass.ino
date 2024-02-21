// SCL is the clock line. It is used to synchronize all data transfers over the I2C bus. SDA is the data line.
// A pull-up resistor is needed when connecting SDA/SCL pins.
// MEGA 2560 board has pull-up resistors on pins 20 and 21 onboard.

// https://www.arduino.cc/reference/en/language/functions/communication/wire/
// https://www.arduino.cc/reference/en/language/functions/communication/wire/read/
// https://www.arduino.cc/reference/en/language/functions/communication/wire/requestfrom/
// https://www.arduino.cc/reference/en/language/functions/communication/wire/available/

// add LCD library
#include <LiquidCrystal.h>
// add I2C library
#include <Wire.h>

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 37, en = 36, d4 = 35, d5 = 34, d6 = 33, d7 = 32;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

#define CMPS14_ADDRESS 0x60  // Address of CMPS14 shifted right one bit for arduino wire library

byte raw;
int bearing;
String dir;

void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(20, 4);
  // Setup Serial Monitor
  Serial.begin(9600);

  // join I2C bus
  Wire.begin();
}

void loop() {
  Wire.beginTransmission(CMPS14_ADDRESS);  //starts communication with CMPS14
  Wire.write(1);                     //Sends the register 1 from where we wish to start reading heading value
  Wire.endTransmission(false);       // keep connection active

  // Request one byte from the CMPS14
  // this will give us the 8 bit bearing
  delay(1);
  Wire.requestFrom(CMPS14_ADDRESS, 1, true);

  // Wait for all bytes to come back
  if (Wire.available() >= 1) {
    raw = Wire.read();              // 0~255
    Serial.println(raw);
  }
  bearing =  360.0 * raw / 255.0;

  if (bearing == 3600) {
    bearing =  360;
  }
  else if (bearing >= 180) {
    bearing =  360.0 * raw / 255.0 - 180;
  }
  else if (bearing < 180) {
    bearing = 360.0 * raw / 255.0 + 180;
  }


  lcd.setCursor(0, 0);
  lcd.print(bearing);
  delay(5);

  if (bearing  == 0 || bearing  == 360)
    dir = "North !!!";
  else if (bearing  > 0 && bearing  < 90)
    dir = "NE";
  else if (bearing  == 90)
    dir = "East -->";
  else if (bearing  > 90 && bearing  < 180)
    dir = "SE";
  else if (bearing  == 180)
    dir = "South !!!";
  else if (bearing  > 180 && bearing  < 270)
    dir = "SW";
  else if (bearing  == 270)
    dir = "West <--";
  else if (bearing  > 270 && bearing  < 360)
    dir = "NW";

  lcd.setCursor(0, 1);
  lcd.print(dir);
  delay(5);
  lcd.clear();
}
