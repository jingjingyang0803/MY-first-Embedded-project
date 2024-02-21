#include <LiquidCrystal.h>
const int rs = 37, en = 36, d4 = 35, d5 = 34, d6 = 33, d7 = 32;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
String message = "";

void setup() {
  lcd.begin(20, 4);
  Serial.begin(9600);
  Serial.println("Write something to the serial monitor.");
}

void loop() {
  while (Serial.available() > 0) {
    message = Serial.readStringUntil('\n');
    Serial.print("Message received, content: ");
    Serial.println(message);
    
    // clear the string for new input:
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("I received: ");
    // about the 1324 printing problem of 20*4 LCD ---> https://forum.arduino.cc/t/20x4-lcd-screen-only-prints-to-line-0-and-2/121300/8
    lcd.setCursor(0, 1);
    lcd.print(message);
  }
}
