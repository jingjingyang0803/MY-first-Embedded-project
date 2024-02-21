#include <Wire.h>
#include <LIDARLite.h>
#include <LiquidCrystal.h>

const int rs = 37, en = 36, d4 = 35, d5 = 34, d6 = 33, d7 = 32;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// Globals
LIDARLite lidarLite;
int cal_cnt = 0;

void setup()
{
  Serial.begin(9600); // Initialize serial connection to display distance readings

  lcd.begin(20, 4);

  lidarLite.begin(0, true); // Set configuration to default and I2C to 400 kHz
  lidarLite.configure(1); // Change this number to try out alternate configurations
}

void loop()
{
  int dist;

  // At the beginning of every 100 readings,
  // take a measurement with receiver bias correction
  if ( cal_cnt == 0 ) {
    dist = lidarLite.distance();      // With bias correction
  } else {
    dist = lidarLite.distance(false); // Without bias correction
  }

  // Increment reading counter
  cal_cnt++;
  cal_cnt = cal_cnt % 100;

  // Display distance
  Serial.print(dist);
  Serial.println(" cm");

  //display distance to LCD
  lcd.clear();
  lcd.print("Distance: ");
  lcd.print(dist);
  lcd.print("cm");
  lcd.setCursor(10,1);

  lcd.print(dist / 2.54);
  lcd.print("in");
  delay(100);
}