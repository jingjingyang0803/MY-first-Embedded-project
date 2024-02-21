/*
  LiquidCrystal Library - Autoscroll

  Demonstrates the use a 16x2 LCD display.  The LiquidCrystal
  library works with all LCD displays that are compatible with the
  Hitachi HD44780 driver. There are many of them out there, and you
  can usually tell them by the 16-pin interface.

  This sketch demonstrates the use of the autoscroll()
  and noAutoscroll() functions to make new text scroll or not.

  The circuit:
   LCD RS pin to digital pin 12
   LCD Enable pin to digital pin 11
   LCD D4 pin to digital pin 5
   LCD D5 pin to digital pin 4
   LCD D6 pin to digital pin 3
   LCD D7 pin to digital pin 2
   LCD R/W pin to ground
   10K resistor:
   ends to +5V and ground
   wiper to LCD VO pin (pin 3)

  Library originally added 18 Apr 2008
  by David A. Mellis
  library modified 5 Jul 2009
  by Limor Fried (http://www.ladyada.net)
  example added 9 Jul 2009
  by Tom Igoe
  modified 22 Nov 2010
  by Tom Igoe
  modified 7 Nov 2016
  by Arturo Guadalupi

  This example code is in the public domain.

  http://www.arduino.cc/en/Tutorial/LiquidCrystalAutoscroll

*/
//--------------------------week 3 exercise 2-----------------------------------
// 2. Make a code that loops around your LCD and prints the alphabet (A-Z) one letter at a time. 

// include the library code:
#include <LiquidCrystal.h>

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 37, en = 36, d4 = 35, d5 = 34, d6 = 33, d7 = 32;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
}


//For 16*2 LCD
void loop() {
  char letter = 65;
  while (1) {
    for (int column = 0; column < 16; column++) {
      lcd.setCursor(column, 0);
      // print letters:
      if (letter < 90) {
        letter = (char)(letter % 90);
      }
      else if (letter % 90 == 0) {
        letter = (char)(letter % 90 + 90);
      }
      else {
        letter = (char)(letter % 90 + 64);
      }
      lcd.print(letter);
      letter++;
      delay(255);
      // clear screen for the next letter:
      lcd.clear();
      // move the cursor to the end of the char printed previously
    }
    for (int column = 15; column >= 0; column--) {
      lcd.setCursor(column, 1);
      if (letter < 90) {
        letter = (char)(letter % 90);
      }
      else if (letter % 90 == 0) {
        letter = (char)(letter % 90 + 90);
      }
      else {
        letter = (char)(letter % 90 + 64);
      }
      // print letters:
      lcd.print(letter);
      letter++;
      delay(255);
      // clear screen for the next letter:
      lcd.clear();
      // move the cursor to the end of the char printed previously
    }
  }
}
