#include <Wire.h>
#include <LIDARLite.h>
#include <LiquidCrystal.h>

const int rs = 37, en = 36, d4 = 35, d5 = 34, d6 = 33, d7 = 32;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// Globals
LIDARLite lidarLite;
int cal_cnt = 0;

int measurements[] = {0, 0, 0};
int measurementCounter = 0;

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

  String msg = "null";
  while (Serial.available() > 0) {
    msg = Serial.readStringUntil('\n');
  }

  //=============== Control the vehicle according to the given command and value ==================

  if (msg == "m") {
    Serial.println("counter: " + String(measurementCounter));
    measurements[measurementCounter] = dist;
    measurementCounter++;
    if(measurementCounter > 3) {
      measurementCounter = 0;
    }
  }

  lcd.clear();
  for (int i = 0; i < 3; i++) {
    if (i < measurementCounter) {
      lcd.setCursor(0, i);
      lcd.print("M" + String(i + 1) + ": " + String(measurements[i]) + " cm");
    }
  }

  lcd.setCursor(0, 3);
  //lcd.print("counter: " + String(measurementCounter));
  
  if (measurementCounter < 2) {
    lcd.print("Not enough data ...");
  } else if (measurementCounter == 2) {
    double area = ((measurements[0] / 100.0) * (measurements[1] / 100.0));
    lcd.print("Area: " + String(area) + " m2");
  } else if (measurementCounter == 3) {
    double volume = ((measurements[0] / 100.0) * (measurements[1] / 100.0) * (measurements[2] / 100.0));
    lcd.print("Volume: " + String(volume) + " m3");
  }

  // Increment reading counter
  cal_cnt++;
  cal_cnt = cal_cnt % 100;

  delay(100);
}