// a program which uses the analog values read from two potentiometers to change the brightness of two leds using PWM. (Contact group can use the joystick)
// One potentiometer determines the brightness and the other direction
 
// a button turns the leds entirely on and off using interrupts

/*
  https://roboticsbackend.com/arduino-turn-led-on-and-off-with-button/
*/

// constants won't change. They're used here to set pin numbers:
volatile int buttonPin = 19;     // the number of the pushbutton pin
const int x_pin = A8, y_pin = A9;
const int leftLedPin = 3, rightLedPin = 2;
int brightness = 0;

boolean ledState = LOW;

void setup() {
  Serial.begin(9600);
  pinMode(buttonPin, INPUT);
  pinMode(leftLedPin, OUTPUT);
  pinMode(rightLedPin, OUTPUT);
  pinMode(A14, OUTPUT);
  pinMode(A15, OUTPUT);
  // With a function called attachInterrupt(), we can execute an ISR
  // when the state of the Arduino io-pin changes.
  // Attach an interrupt to the ISR vector // Pin 19, Routine:button_pressed_ISR, FALLING Edge
  attachInterrupt(digitalPinToInterrupt(buttonPin), button_control, FALLING);
}

void loop() {
  brightness = (analogRead(y_pin) / 1023.0) * 255 ;
  //Serial.println(brightness);
  //Serial.println(analogRead(x_pin));
    if (analogRead(x_pin) < 490) {
      digitalWrite(leftLedPin, HIGH);//turn left led on
      digitalWrite(rightLedPin, LOW);
      analogWrite(leftLedPin, brightness);
    }
    else if (analogRead(x_pin) > 533)
    {
      digitalWrite(leftLedPin, LOW);
      digitalWrite(rightLedPin, HIGH);//turn right led on
      analogWrite(rightLedPin, brightness);
    }
    else {
      digitalWrite(leftLedPin, ledState);
      digitalWrite(rightLedPin, ledState);
    }
}

void button_control() {
  Serial.println("pressed");
  ledState = !ledState;//toggle the variable ledState
  digitalWrite(leftLedPin, ledState);
  digitalWrite(rightLedPin, ledState);
}
