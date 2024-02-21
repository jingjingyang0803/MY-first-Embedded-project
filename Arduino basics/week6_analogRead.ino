
/*
  Analog Input Demonstrates analog input by
  reading an analog sensor on analog pin 0 and
  turning on and off a light emitting diode(LED)
  connected to digital pin 13.  The LED will be
  on and off depends on the value obtained by analogRead().

  The circuit:
    potentiometer connected to analog pin 0.
    Center pin of the potentiometer goes to the
    analog pin. side pins of the potentiometer go
    to +5V and ground
    LED anode (long leg) attached to digital output 13,
    LED cathode (short leg) attached to ground
*/

//--------------------------week 6 task 1---------------------------------
// Read the value of the potentiometer/trimmer's middle pin/terminal with an Arduino
// using analogRead()-function. Print the value on to the LCD-display.
// include the library code:
#include <LiquidCrystal.h>

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 37, en = 36, d4 = 35, d5 = 34, d6 = 33, d7 = 32;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

int sensorValue = 0;
int analog_pin = A5;

void setup()
{
  pinMode(analog_pin, INPUT);
  lcd.begin(16, 2);
}

void loop()
{
  // read the value from the sensor
  sensorValue = analogRead(analog_pin);
  // print the sensor value to the LCD-display
  lcd.print("sensor: ");
  lcd.print(sensorValue);
  delay(1000); // Delay a little bit to improve simulation performance
  lcd.clear();
}




////--------------------------week 6 task 2---------------------------------
//// Change the read analog value (should range form 0 to 1023) into voltage and
//// print that value also on to the LCD-screen. Add a functionality that if the
//// voltage is over 3 volts, the built in led should turn on.
//
//#include <LiquidCrystal.h>
//// initialize the library by associating any needed LCD interface pin
//// with the arduino pin number it is connected to
//const int rs = 37, en = 36, d4 = 35, d5 = 34, d6 = 33, d7 = 32;
//LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
//
//int sensorValue = 0;
//float voltage = 0;
//int analog_pin = A5;
//
//void setup()
//{
//  pinMode(analog_pin, INPUT);
//  // initialize digital pin LED_BUILTIN as an output.
//  pinMode(LED_BUILTIN, OUTPUT);
//  lcd.begin(16, 2);
//}
//
//void loop()
//{
//  // read the value from the sensor
//  sensorValue = analogRead(analog_pin);
//  // change the read analog value (should range form 0 to 1023) into voltage
//  voltage = sensorValue * (5.0 / 1023.0);
//  // print the voltage value to the LCD-display
//  lcd.print("Voltage: ");
//  lcd.print(voltage);
//  lcd.print(" V");
//  delay(1000);
//  lcd.clear();
//  // if the voltage is over 3 volts
//  if (voltage > 3) {
//    digitalWrite(LED_BUILTIN, HIGH);// turn the built-in LED on (HIGH is the voltage level)
//  } else {
//    digitalWrite(LED_BUILTIN, LOW);// turn the LED off by making the voltage LOW
//  }
//}






////--------------------------week 6 task 3---------------------------------
////3. Add an external LED similarly to the picture (upper part). A good resistor size is somewhere around 200-300 ohms.
//// Change the code, so that the external LED lights up when measured voltage is over 4 V.
//
//#include <LiquidCrystal.h>
//// initialize the library by associating any needed LCD interface pin
//// with the arduino pin number it is connected to
//const int rs = 37, en = 36, d4 = 35, d5 = 34, d6 = 33, d7 = 32;
//LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
//
//int sensorValue = 0;
//float voltage = 0;
//int analog_pin = A5;
//int LED_pin = 19;
//
//void setup()
//{
//  pinMode(analog_pin, INPUT);
//  // initialize digital pin LED_BUILTIN as an output.
//  pinMode(LED_pin, OUTPUT);
//  lcd.begin(16, 2);
//}
//
//void loop()
//{
//  // read the value from the sensor
//  sensorValue = analogRead(analog_pin);
//  // change the read analog value (should range form 0 to 1023) into voltage
//  voltage = sensorValue * (5.0 / 1023.0);
//  // print the voltage value to the LCD-display
//  lcd.print("voltage: ");
//  lcd.print(voltage);
//  lcd.print("V");
//  delay(1000);
//  lcd.clear();
//  // if the voltage is over 4 volts
//  if (voltage > 4) {
//    digitalWrite(LED_pin, HIGH);// turn the external LED on (HIGH is the voltage level)
//  } else {
//    digitalWrite(LED_pin, LOW);// turn the LED off by making the voltage LOW
//  }
//}
//
//



////--------------------------week 6 task BONUS AnalogRead speed---------------------------------
//// Compare AD-conversion speed, AnalogRead vs. register control (slide 22, ADconverison).
//// Measure the time it takes to do AD- conversion using AnalogRead function and then do the same
//// with funciton using register control. You can use function micros() on the main loop to measure the speed.
//// Show conversion times on the LCD.
//
//#include <LiquidCrystal.h>
//// initialize the library by associating any needed LCD interface pin
//// with the arduino pin number it is connected to
//const int rs = 37, en = 36, d4 = 35, d5 = 34, d6 = 33, d7 = 32;
//LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
//
//int sensorValue = 0;
//int analog_pin = A5;
//
//void setup()
//{
//  pinMode(analog_pin, INPUT);
//  lcd.begin(16, 2);
//}
//
//void loop()
//{
//  unsigned long t0;
//  const int READ_COUNT = 1000;
//  t0 = micros();
//  for (int i = 0; i < READ_COUNT; i++) {
//    analogRead(analog_pin);
//  }
//  unsigned long  t1 = micros();
//  unsigned long delta_t = t1 - t0; // calculate elapsed time
//
//  lcd.print("AnalogRead speed: ");
//  lcd.setCursor(0, 1);
//  lcd.println(delta_t / READ_COUNT); //it takes about 100 microseconds (0.0001 s) to read an analog input
//  delay(2000);
//  lcd.clear();
//}


////--------------------------week 6 task BONUS register control speed---------------------------------
//#include <LiquidCrystal.h>
//// initialize the library by associating any needed LCD interface pin
//// with the arduino pin number it is connected to
//const int rs = 37, en = 36, d4 = 35, d5 = 34, d6 = 33, d7 = 32;
//LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
//
//void setup()
//{
//  lcd.begin(16, 2);
//
//  //http://9bit.se/category/arduino/arduino-tutorials/
//  ADCSRA = 0;             // clear ADCSRA register
//  ADCSRB = 0;             // clear ADCSRB register
//  ADMUX |= (0 & 0x07);    // set A0 analog input pin
//  ADMUX |= (1 << REFS0);  // set reference voltage
//  ADMUX |= (1 << ADLAR);  // left align ADC value to 8 bits from ADCH register
//
//  // sampling rate is [ADC clock] / [prescaler] / [conversion clock cycles]
//  // for Arduino Uno ADC clock is 16 MHz and a conversion takes 13 clock cycles
//  //ADCSRA |= (1 << ADPS2) | (1 << ADPS0);    // 32 prescaler for 38.5 KHz
//  ADCSRA |= (1 << ADPS2);                     // 16 prescaler for 76.9 KHz
//  //ADCSRA |= (1 << ADPS1) | (1 << ADPS0);    // 8 prescaler for 153.8 KHz
//
//  ADCSRA |= (1 << ADATE); // enable auto trigger
//  ADCSRA |= (1 << ADIE);  // enable interrupts when measurement complete
//  ADCSRA |= (1 << ADEN);  // enable ADC
//  ADCSRA |= (1 << ADSC);  // start ADC measurements
//}
//
//// https://www.renesas.com/eu/zh/document/apn/measuring-voltages-4-channel-ad-conversion?language=en
//// https://web.ics.purdue.edu/~jricha14/Port_Stuff/PortA_ADC.htm
//// http://garretlab.web.fc2.com/en/arduino/inside/hardware/arduino/avr/cores/arduino/wiring_analog.c/analogRead.html
//
//int AD_conversion(byte ch)
//{
//  long t0 = micro();
//  ch = A5;
//  //       76543210
//  DIDR0 = B11111111; //All AD pints to AD mode
//  //       76543210
//  ADMUX = B01000101 | ch; //Reference voltage=5V/input channel=ch
//  //        76543210
//  ADCSRA = B11000111;
//  bitSet(ADCSRA, ADEN); //enable AD-converter
//  bitSet(ADCSRA, ADSC); //starting signal conversion
//  delay(500);
//  while (ADCSRA & B01000000 != 0); //loop to wait until conversion is ready
//  return ADC;//function returns AD converted value
//
//  long t1 = micro();
//  lcd.print(t1 - t0);
//}
