// https://www.circuitschools.com/diy-measuring-wheel-using-arduino-and-rotary-encoder/

// include the library code:
#include <LiquidCrystal.h>

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 37, en = 36, d4 = 35, d5 = 34, d6 = 33, d7 = 32;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);



// Mega2560 ISR pin: D2,D3,D18,D19,D20,D21
#define outputA 24  //CLK pin
#define outputB 2  //DT pin
#define rstbtn  19  //reset button pin//SW                   

int counter = 0;// amount of pulses
const float pi = 3.14; // Pi value

const int R = 3;  //Radius of the wheel from center to edge 
const int N = 70; //no of steps for one rotation          
float distance = 0;
unsigned long lastButtonPress = 0;

void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(20, 4);
  // Setup Serial Monitor
  Serial.begin(9600);

  // Initialize encoder pins
  pinMode(outputA, INPUT);
  pinMode(outputB, INPUT);
  pinMode(rstbtn, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(outputA), readCounter, FALLING);

  Serial.print("Radius of the wheel from center to edge ?    ");
  Serial.print(R);
  Serial.println("cm");

  Serial.print("How many pulses one round contain ?          ");
  Serial.println(N);

  Serial.print("What is wheel distance in one round ?        ");
  Serial.print(2 * pi * R);
  Serial.println("cm");

  Serial.print("How many pulses driving 50cm gives ?         ");
  Serial.println(50 / ((2 * pi * R) / N));

  Serial.print("How many pulses 1 cm driving gives ?         ");
  Serial.println(1 / ((2 * pi * R) / N));
}

void loop() {
  lcd.setCursor(0, 0);
  lcd.print("Distance: in cm");

  // The distance travelled for one step of Encoder = (2xπxR)/N
  distance = ((2 * pi * R) / N) * counter;
  lcd.setCursor(0, 1);
  lcd.print(distance);
  lcd.print("        ");
//  
//  Serial.print(counter);
//  Serial.print(" || ");
//  Serial.println(distance);

  if (digitalRead(rstbtn) == LOW) {
    resetCounter();

    //if 50ms have passed since last LOW pulse, it means that the
    //button has been pressed, released and pressed again
    if (millis() - lastButtonPress > 50) {
      Serial.println("Button pressed!");
    }

    // Remember last button press event
    lastButtonPress = millis();
  }
}

void readCounter() {
  //read state of outputB
  //if state B is still 1 when state A falls to 0
  // the encoder is rotating CW so increment
  if (digitalRead(outputB) == HIGH) {
    counter++; // Clockwise    
  }
  //if state B is already 0 when state A falls to 0
  // the encoder is rotating CCW so decrement
  else if (digitalRead(outputB) == LOW) {
    counter--; // Counterclockwise
  }
}

// Get the counter value, disabling interrupts.
// This make sure readEncoder() doesn't change the value
// while we're reading it.
int getCounter() {
  noInterrupts();
  int result = counter;
  interrupts();
  return result;
}

void resetCounter() {
  noInterrupts();
  counter = 0;
  interrupts();
}
