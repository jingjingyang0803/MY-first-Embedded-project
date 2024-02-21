/*
  Button
  The circuit:
  - pushbutton attached to pin 19 from +5V
  - 10K resistor attached to pin 19 from ground
*/

////--------------------week 5 exercise 1---------------------------------------
//// 1. Build the circuit according to the picture below. Resistor size is 10kohm.
//// Using a function called digitalRead(), read the state of the switch and print
//// "pressed" or "not pressed" based on it's state on to the LCD.
//
//// include the library code:
//#include <LiquidCrystal.h>
//
//// initialize the library by associating any needed LCD interface pin
//// with the arduino pin number it is connected to
//const int rs = 37, en = 36, d4 = 35, d5 = 34, d6 = 33, d7 = 32;
//LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
//
//
//// constants won't change. They're used here to set pin numbers:
//const int buttonPin = 19;     //the number of the pushbutton pin
//
//// variables will change:
//int buttonState = 0;       // variable for reading the pushbutton status
//
//void setup() {
//  // initialize the pushbutton pin as an input:
//  pinMode(buttonPin, INPUT);
//  lcd.begin(16, 2);
//}
//
//void loop() {
//  // read the state of the pushbutton value:
//  buttonState = digitalRead(buttonPin);
//
//  // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
//  if (buttonState == HIGH) {
//    lcd.print("pressed");
//    delay(500);// Delay a little bit to improve simulation performance
//    // clear screen for the next letter:
//    lcd.clear();
//  } else {
//    lcd.print("not pressed");
//    delay(500);
//    // clear screen for the next letter:
//    lcd.clear();
//  }
//}

//
////--------------------week 5 exercise 2---------------------------------------
//// 2. With a function called attachInterrupt(), we can execute an interrupt service routine (ISR)
//// when the state of the Arduino io-pin changes. Make an ISR that counts how many times the switch
//// has been pushed. Print counter value to the LCD.
//
//// include the library code:
//#include <LiquidCrystal.h>
//
//// initialize the library by associating any needed LCD interface pin
//// with the arduino pin number it is connected to
//const int rs = 37, en = 36, d4 = 35, d5 = 34, d6 = 33, d7 = 32;
//LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
//
//// constants won't change. They're used here to set pin numbers:
//const int buttonPin = 19;     // the number of the pushbutton pin
//
//// variables will change:
//int buttonState = 0;       // variable for reading the pushbutton status
//int buttonPushCounter = 0;    // counts how many times the switch has been pushed.
//
//// intialize interrupts
//void setup() {
//  // initialize the pin 19 in MEGA(=INT2) as an input for button sensing
//  pinMode(buttonPin, INPUT);
//  // Attach an interrupt to the ISR vector
//  // Pin 19, Routine:pin_ISR, Falling Edge
//  attachInterrupt(digitalPinToInterrupt(buttonPin), pin_ISR, FALLING);
//  lcd.begin(16,2);
//}
//
//void loop() {
//  // read the state of the pushbutton value:
//  buttonState = digitalRead(buttonPin);
//
//  // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
//  if (buttonState == HIGH) {
//    lcd.setCursor(0, 0);
//    lcd.print("pressed");
//    delay(500);
//    // clear screen for the next letter:
//    lcd.clear();
//  }
//  else {
//    lcd.setCursor(0, 0);
//    lcd.print("not pressed");
//    delay(500);// delay in between reads for stability
//    // clear screen for the next letter:
//    lcd.clear();
//  }
//
//  //print value of variable number_of_pressing to the LCD second row
//  lcd.setCursor(0, 1);
//  lcd.print("Pressed ");
//  lcd.print(buttonPushCounter);
//  lcd.print(" times");
//}
//
//// Interupt service routine
//void pin_ISR() {
//  // code to be executed when button is pressed
//  // Falling endge of pin voltage will activate interrupt service routine
//  buttonPushCounter++;//increment variable number_of_pressing
//}


//--------------------week 5 exercise 3---------------------------------------
// 3. Write code that measures the time difference between two falling edges and transforms it into a frequency.
// Print the frequency to the screen and test your code with a signal generator.

// include the library code:
#include <LiquidCrystal.h>
#include <TimerOne.h>  // inclue Timer library

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 37, en = 36, d4 = 35, d5 = 34, d6 = 33, d7 = 32;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);


// constants won't change. They're used here to set pin numbers:
const int buttonPin = 19;     // the number of the pushbutton pin

// variables will change:
int buttonState = 0;       // variable for reading the pushbutton status
int buttonPushCounter = 0;    // counts how many times the switch has been pushed.
double i_time = 0;
double frequency = 0;

// intialize interrupts
void setup() {
  // initialize the pin 19 in MEGA(=INT2) as an input for button sensing
  pinMode(buttonPin, INPUT);
  // Attach an interrupt to the ISR vector
  // Pin 19, Routine:pin_ISR, Falling Edge
  attachInterrupt(digitalPinToInterrupt(buttonPin), pin_ISR, FALLING);
  Timer1.initialize(5000000); //// initialize timer1, and set a 1/2 second period
  Timer1.attachInterrupt(callback);//attaches callback() as a timer overflow
  lcd.begin(16, 2);
}

void loop() {
  // read the state of the pushbutton value:
  buttonState = digitalRead(buttonPin);

  //print value of variable number_of_pressing to the LCD second row
  lcd.setCursor(0, 0);
  lcd.print("Pressed ");
  lcd.print(buttonPushCounter);
  lcd.print(" times");
  //print the frequency on the LCD
  lcd.setCursor(0, 1);
  lcd.print(frequency);
  lcd.print(" Hz");
}

// Interrupt service routine
void pin_ISR() {
  // code to be executed when button is pressed
  // Falling endge of pin voltage will activate interrupt service routine
  buttonPushCounter++;//increment variable number_of_pressing
}

// Timer interrupt settings
// Interrupt routine called in each 0.5 sec by HW timer
void callback() {
  i_time++;//increment variable i_time in each 0.5 sec
  if (i_time > 9) {
    i_time = 0; //reset variable i_time in each 5 sec

    frequency = buttonPushCounter / 5; //save varibale buttonPushCounter value to variable frequency
    //pulse couting time is 5 sec
    //5 sec to pulse/sec>device freq with 5

    buttonPushCounter = 0; // reset variable buttonPushCounter value
  }
}
