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

  lcd.setCursor(0, 0);
  lcd.print("Distance: ");
  lcd.print(dist);
  lcd.print("cm");
  

  int spaces = 0;
  if (dist > 80) {
    spaces = 19;
  } else if (dist > 10) {
    spaces = dist * 0.3;
  }

  lcd.setCursor(spaces, 1);
  lcd.print("+");

  lcd.setCursor(0, 2);
  lcd.print(20);
  lcd.setCursor(7, 2);
  lcd.print(40);
  lcd.setCursor(13, 2);
  lcd.print(60);
  lcd.setCursor(18, 2);
  lcd.print(80);

  delay(100);
}