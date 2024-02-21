/////////////////////////////////// Libraries included//////////////////////////////////////
#include <LiquidCrystal.h>                            // include LCD library

#include <Ethernet.h>                                 // include Ethernet library   

#include <PubSubClient.h>                             // include MQTT library

#include <TimerOne.h>                                 // include timer library

///////////////////////// LCD pin wiring settings for MEGA /////////////////////////////////
// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
//                RS  E   D4  D5  D6  D7
LiquidCrystal lcd(37, 36, 35, 34, 33, 32);

EthernetClient ethClient;                             // Ethernet object var


/////////////////////////////////// MAC number /////////////////////////////////////////////
static uint8_t mymac[6] = { 0x44, 0x76, 0x58, 0x10, 0x00, 0x73 };


////////////////////////////////// MQTT settings ///////////////////////////////////////////
unsigned int Port = 1883;            // MQTT port number
byte server[] = { 10, 6, 0, 21 };    // TAMK IP

char* deviceId  = "2020a72145";      // set device id (MQTT client username)
char* clientId     = "a771345";      // set a random string (max 23 chars, MQTT client id)
char* deviceSecret = "tamk1";        // set device secret (MQTT client password)

///////////////////////////////// MQTT Server settings /////////////////////////////////////
// subscription callback for received MQTTT messages
void callback(char* topic, byte* payload, unsigned int length);
// mqtt client
PubSubClient client(server, Port, callback, ethClient);


//////////////////////////////// MQTT topic names  /////////////////////////////////////////
#define inTopic    "ICT4_in_2020"              // * MQTT channel where data are received 
#define outTopic   "ICT4_out_2020"             // * MQTT channel where data is send 
////////////////////////////////////////////////////////////////////////////////////////////

///******************************* Variables *********************************************
// constants won't change. They're used here to set pin numbers:
const int sensorPin = 3;                 // number of the digital signal pin
const int buttonPin = 19;                // number of the pushbutton pin

// variables will change:
int buttonState = 0;                     // variable for reading the pushbutton state
// declaring variables volatile as they are used in ISR section
volatile double pulse = 0;               // counts the pulses
volatile double i_time = 0;
// set datatype double to keep precision
double  rainLevel = 0;                   // average rain amount,unit ml/area, area=55cm2
double rainfall = 0;                     // average rain depth, unit mm/s
///**************************************************************************************


//////////////////////////////// SETUP section /////////////////////////////////////////////
void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);                         // Serial monitor baudrate  = 9600

  lcd.begin(20, 4);                           // Display size definition 20 char 4 rows

  lcd.setCursor(0, 0);                       // set cursor to left upper corner
  //         01234567890123456789
  lcd.print("30.3.2020 Alyk jatk ");         // print to lCD

  Serial.println("Start 30.3.2020");         // print to serial monitor

  delay(500);

  fetch_IP();                               // initialize Ethernet connection

  Connect_MQTT_server();                    // connect to MQTT server

  ///***************************************************************************************
  pinMode(sensorPin, INPUT);       // initialize sensorPin as an input for signal sensing
  pinMode(buttonPin, INPUT);       // initialize buttonPin as an input for button state

  // With a function called attachInterrupt(), we can execute an ISR
  // when the state of the Arduino io-pin changes.
  // Attach an interrupt to the ISR vector // Pin 3, Routine:sensor_ISR, RISING Edge
  attachInterrupt(digitalPinToInterrupt(sensorPin), sensor_ISR, RISING);


  // initialize timer1, and set one-half second as a period
  Timer1.initialize(500000);
  //attaches callback() as a timer overflow
  Timer1.attachInterrupt(callback);
  ///***************************************************************************************
}

///************************* LOOP section **************************************************
void loop()
{
  lcd.setCursor(0, 1);                         // set cursor to 2nd row
  //         01234567890123456789
  lcd.print("Pulses in 10s:");
  lcd.print(pulse);                         // print to LCD the pulses per 10 seconds

  lcd.setCursor(0, 2);                    // set cursor to 3rd row
  //         01234567890123456789 
  lcd.print("                    ");    //clear the 3rd row to print updated rain amount
  lcd.setCursor(0, 2);                 // set cursor to 3rd row
  lcd.print("Rain amount:");
  lcd.print(rainLevel);                 // print value of rain amount to LCD
  lcd.print(" ml");

  send_MQTT_message(rainLevel);      // Send MQTT message the rain amount
  delay(1500);

  lcd.setCursor(0, 3);                   // set cursor to 4th row
  //         01234567890123456789
  lcd.print("                    ");    //clear the 4th row to print updated rain amount
  lcd.setCursor(0, 3);                  // set cursor to 4th row
  if (rainfall == 0) {                  // print the rain Level
    lcd.print("Sun^_^");
    digitalWrite(LED_BUILTIN, LOW);// turn the built-in LED off by making the voltage LOW
  }
  else if (rainfall <= 10) {
    lcd.print("Light>>");
    digitalWrite(LED_BUILTIN, LOW);
  }
  else if (rainfall <= 50) {
    lcd.print("Medium>>");
    digitalWrite(LED_BUILTIN, LOW);
  }
  else {
    lcd.print("Heavy>>");
    digitalWrite(LED_BUILTIN, HIGH);// turn the built-in LED on (HIGH is the voltage level)
  }
  lcd.print(rainfall);                // print value of rain depth to LCD
  lcd.print(" mm/s");
  delay(5000);

  // read the state of the pushbutton value:
  buttonState = digitalRead(buttonPin);
  // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
  if (buttonState == HIGH) {
    lcd.setCursor(0, 3);           // set cursor to 4th row
    lcd.print("Group B-->Rain 3"); // print group name and signal if button pressed
    delay(2000);// delay a little bit to improve simulation performance
  }
}

///************************* Interrupt service routine *************************************
// This is an ISR that counts the pulses.
// code to be executed for each occurred pulse
// Rising endge of pin voltage will activate interrupt service routine
void sensor_ISR() {
  pulse++;                        //increment variable “pulse”
}

///************************* Timer interrupt settings **************************************
void callback() {                 // Interrupt routine called in each 0.5 sec by HW timer
  i_time++;                       //increment variable "i_time" in each 0.5 sec

  if (i_time >= 20) {
    //reset value of variable "i_time" to ZERO for the next 10 sec
    i_time = 0;

    rainLevel = (pulse / 10.0) * 3;             //save rain amount to variable "rainLevel"
    //collection area 55cm2(5500mm2),period 10s
    rainfall = ((pulse / 10.0) * 3000) / 5500; //save rain depth to variable "rainfall"

    // reset value of variable "pulse" to ZERO for the next 10 sec
    pulse = 0;
  }
}


//////////////////////////////// Ethernet routine section //////////////////////////////////
void fetch_IP(void)
{
  byte rev = 1;



  lcd.setCursor(0, 0);

  //         01234567890123456789
  lcd.print("     Waiting IP     ");

  rev = Ethernet.begin( mymac);            // get IP number from DHCP server

  Serial.print( F("\nW5100 Revision ") );

  if ( rev == 0) {

    Serial.println( F( "Failed to access Ethernet controller" ) );

    // 0123456789012345689
    lcd.setCursor(0, 0); lcd.print(" Ethernet failed   ");
  }


  Serial.println( F( "Setting up DHCP" ));
  Serial.print("Connected with IP: ");
  Serial.println(Ethernet.localIP());


  lcd.setCursor(0, 0);
  //         012345678901234567890
  lcd.print("                     ");

  lcd.setCursor(0, 0);                   // set cursor to 1st row
  lcd.print("myIP=");
  lcd.print(Ethernet.localIP());         // print IP number from DHCP server
  delay(1500);

}


//////////////////////////////// MQTT routine section //////////////////////////////////////

void send_MQTT_message(int rainLevel)
{
  char bufa[50];

  // create message with header and data
  // sprintf(bufa,"IOTJS={\"S_name\":\"group_name_here\",\"S_value\":%s}",str_signal_value);
  sprintf(bufa, "IOTJS={\"S_name\":\"Rain_3\",\"S_value\":%d}", rainLevel);

  Serial.println( bufa );            // Print message to serial monitor

  if (client.connected())            // send message to MQTT server
  {
    client.publish(outTopic, bufa);
  }
  else                               // Reconnect if connection is lost
  {
    delay(500);

    lcd.setCursor(0, 1);
    //         01234567890123456789
    lcd.print("   RE Connecting    ");

    Serial.println(" RE Connecting" );

    client.connect(clientId, deviceId, deviceSecret);

    delay(1000);                    // wait for reconnecting
  }

}



///  MQTT server connection

void Connect_MQTT_server()
{
  Serial.println(" Connecting to MQTT" );

  // Print MQTT server IP number to Serial monitor
  Serial.print(server[0]); Serial.print(".");
  Serial.print(server[1]); Serial.print(".");
  Serial.print(server[2]); Serial.print(".");
  Serial.println(server[3]);

  lcd.setCursor(0, 1);
  //         01234567890123456789
  lcd.print("                    ");

  lcd.setCursor(0, 1);
  lcd.print("MQTT=");
  lcd.print(server[0]); lcd.print(".");           // Print MQTT server IP number to LCD
  lcd.print(server[1]); lcd.print(".");
  lcd.print(server[2]); lcd.print(".");
  lcd.print(server[3]);

  delay(500);

  if (!client.connected())                        // check if allready connected
  {
    // connection to MQTT server
    if (client.connect(clientId, deviceId, deviceSecret))
    {
      lcd.setCursor(0, 1);
      lcd.print("Conn");                          // Connection is OK
      Serial.println(" Connected OK " );

      client.subscribe(inTopic);                  // subscript to in topic
    }
    else
    {
      lcd.setCursor(0, 1);
      //         01234567890123456789
      lcd.print("  MQTT Error        ");          // error in connection
      Serial.println(" MQTT Connection ERROR " );
    }
  }
}


/// Receive incoming MQTT message

void callback(char* topic, byte * payload, unsigned int length)
{
  // copu the payload content into a char*
  char* receiv_string;
  receiv_string = (char*) malloc(length + 1);
  memcpy(receiv_string, payload, length);         // copy received message to receiv_string
  receiv_string[length] = '\0';

  lcd.setCursor(0, 0);
  //         01234567890123456789
  lcd.print("Mess=               ");

  lcd.setCursor(5, 0);
  lcd.print(receiv_string);                       // print reveived message to LCD
  Serial.println( receiv_string );

  free(receiv_string);
}
