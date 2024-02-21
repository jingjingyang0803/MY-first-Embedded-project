////-------------week 8 exercise 1----------------------------------------
////1. Have the Ethernet module hooked up. Print the IP-address and your groups weather data on to the LCD.
//#include <LiquidCrystal.h>
//
//// initialize the library by associating any needed LCD interface pin
//// with the arduino pin number it is connected to
//const int rs = 37, en = 36, d4 = 35, d5 = 34, d6 = 33, d7 = 32;
//LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
//
//#include <Ethernet2.h>                              // include Ethernet libarty W5500 library
//
//EthernetClient ethClient;                               // Ethernet object var
//
//void fetch_IP(void);
//
//static uint8_t mymac[6] = { 0x44, 0x76, 0x58, 0x10, 0x00, mac_6 };
//
//void setup() {
//  Serial.begin(9600);
//  lcd.begin(16, 2);                                   // Display size defination 20 char  4 row
//  fetch_IP();                                         // initialize Ethernet connection
//  // get IP number from DHCP
//}
//
//void loop() { }
//
//void fetch_IP(void)
//{
//  byte rev = 1;
//  lcd.setCursor(0, 0);
//  //         0123456789012345
//  lcd.print("   Waiting IP   ");
//
//  rev = Ethernet.begin(mymac);                // get IP number
//  Serial.print( F("\nW5100 Revision ") );
//  if ( rev == 0) {
//    Serial.println( F( "Failed to access Ethernet controller" ) );
//    //                              0123456789012345
//    lcd.setCursor(0, 0); lcd.print("Ethernet failed ");
//  }
//  Serial.println( F( "Setting up DHCP" ));
//  Serial.print("Connected with IP: ");
//  Serial.println(Ethernet.localIP());
//
//  lcd.setCursor(0, 0);
//  //         0123456789012345
//  lcd.print("Connected with ");
//
//  lcd.setCursor(0, 1);
//  lcd.print("myIP=");
//  lcd.print(Ethernet.localIP());
//  delay(1500);
//}



//-------------week 8 exercise 2----------------------------------------
//2. Average your groups weather data for a period of 10 seconds and print it on to the LCD.

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
int pulse = 0;    // counts how many times the switch has been pushed.
double i_time = 0;
double frequency = 0;
double rainLevel = 0;

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

  //print the frequency on the LCD
  lcd.setCursor(0, 0);
  lcd.print(frequency);
  lcd.print(" Hz");
  lcd.setCursor(0, 1);
  rainLevel = frequency * 3;
  lcd.print(rainLevel);
  lcd.print(" ml");
}

// Interrupt service routine
void pin_ISR() {
  // code to be executed when button is pressed
  // Falling endge of pin voltage will activate interrupt service routine
  pulse++;//increment variable number_of_pressing
}

// Timer interrupt settings
// Interrupt routine called in each 0.5 sec by HW timer
void callback() {
  i_time++;//increment variable i_time in each 0.5 sec
  if (i_time > 19) {
    i_time = 0; //reset variable i_time in each 10 sec

    frequency = pulse / 10; //save varibale buttonPushCounter value to variable frequency
    //pulse couting time is 10 sec

    pulse = 0; // reset variable buttonPushCounter value
  }
}





////-------------week 8 exercise 3----------------------------------------
////3. Using the bitwise operation and the IO port registers, when a button is pressed, only the maximum value of your weather data is displayed on the LCD.
//
//// include the library code:
//#include <LiquidCrystal.h>
//#include <TimerOne.h>  // inclue Timer library
//
//// initialize the library by associating any needed LCD interface pin
//// with the arduino pin number it is connected to
//const int rs = 37, en = 36, d4 = 35, d5 = 34, d6 = 33, d7 = 32;
//LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
//
//
//// constants won't change. They're used here to set pin numbers:
//const int buttonPin = 19;     // the number of the pushbutton pin
//
//// variables will change:
//int buttonState = 0;       // variable for reading the pushbutton status
//int buttonPushCounter = 0;    // counts how many times the switch has been pushed.
//double i_time = 0;
//double frequency = 0;
//
//// intialize interrupts
//void setup() {
//  // initialize the pin 19 in MEGA(=INT2) as an input for button sensing
//  pinMode(buttonPin, INPUT);
//  // Attach an interrupt to the ISR vector
//  // Pin 19, Routine:pin_ISR, Falling Edge
//  attachInterrupt(digitalPinToInterrupt(buttonPin), pin_ISR, FALLING);
//  Timer1.initialize(5000000); //// initialize timer1, and set a 1/2 second period
//  Timer1.attachInterrupt(callback);//attaches callback() as a timer overflow
//  lcd.begin(16, 2);
//}
//
//void loop() {
//  // read the state of the pushbutton value:
//  buttonState = digitalRead(buttonPin);
//
//  //print the frequency on the LCD
//  lcd.setCursor(0, 1);
//  lcd.print(frequency);
//  lcd.print(" Hz");
//}
//
//// Interrupt service routine
//void pin_ISR() {
//  // code to be executed when button is pressed
//  // Falling endge of pin voltage will activate interrupt service routine
//  buttonPushCounter++;//increment variable number_of_pressing
//}
//
//// Timer interrupt settings
//// Interrupt routine called in each 0.5 sec by HW timer
//void callback() {
//  i_time++;//increment variable i_time in each 0.5 sec
//  if (i_time > 9) {
//    i_time = 0; //reset variable i_time in each 5 sec
//
//    frequency = buttonPushCounter / 5; //save varibale buttonPushCounter value to variable frequency
//    //pulse couting time is 5 sec
//    //5 sec to pulse/sec>device freq with 5
//
//    buttonPushCounter = 0; // reset variable buttonPushCounter value
//  }
//}
