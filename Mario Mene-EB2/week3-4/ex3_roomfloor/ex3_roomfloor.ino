// https://github.com/garmin/LIDARLite_Arduino_Library/blob/master/examples/v3/GetDistanceI2c/GetDistanceI2c.ino
// https://github.com/garmin/LIDARLite_Arduino_Library/issues/14


// include the library code:
#include <LiquidCrystal.h>

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 37, en = 36, d4 = 35, d5 = 34, d6 = 33, d7 = 32;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

double rlength, rwidth, rheight, area, volume;
String message = "";

#include <Wire.h>
#include <LIDARLite.h>

LIDARLite myLidarLite;

void setup()
{
  Wire.begin();
  lcd.begin(20, 4);
  Serial.begin(9600); // Initialize serial connection to display distance readings
  while (!Serial) {
    ;  // wait for serial port to connect. Needed for native USB port only
  }
  Serial.println("Write *l(measure length),w(measure width),A(measure area),h(measure height),V(measure volume) * to the serial monitor.");


  myLidarLite.begin(0, true); // Set configuration to default and I2C to 400 kHz

  myLidarLite.configure(0); // Change this number to try out alternate configurations
}

void loop()
{
  while (Serial.available() > 0) {
    message = Serial.readStringUntil('\n');

    if (message = "l")
      rlength = myLidarLite.distance(false) / 100.0;
    else if (message = "w")
      rwidth = myLidarLite.distance(false) / 100.0;
    else if (message = "A") {
      area = rlength * rwidth;
      Serial.print("Room lenght(m),width(m),area(m^2): ");
      Serial.print(rlength);
      Serial.print("  ");
      Serial.print(rwidth);
      Serial.print("  ");
      Serial.println(area);

      lcd.setCursor(0, 0);
      lcd.print("Area: ");
      lcd.print(area);
      lcd.print(" m^2");
    }
    else if (message = "h")
      rheight = myLidarLite.distance(false);
    else if (message = "V") {
      area = rlength * rwidth * rheight;
      Serial.print("Room lenght(m), width(m), height(m), volume(m^3): ");
      Serial.print(rlength);
      Serial.print("  ");
      Serial.print(rwidth);
      Serial.print("  ");
      Serial.print(rheight);
      Serial.print("  ");
      Serial.println(volume);

      lcd.setCursor(0, 1);
      lcd.print("Volume: ");
      lcd.print(volume);
      lcd.print(" m^3");
    }
  }
}
