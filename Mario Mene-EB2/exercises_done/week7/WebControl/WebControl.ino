//**********************Mario Mene***********************************

/*********
  https://randomnerdtutorials.com/esp32-async-web-server-espasyncwebserver-library/
*********/

// include the libraries to build the asynchronous web server
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <FS.h>
#include <Wire.h>

// declare two global variables which will hold the credentials of the WiFi network to which we are going to connect the ESP8266
//const char* ssid = "SOURCE";  // network name
//const char* password = "Pelle!23";
const char* ssid = "Martin Router King";  // network name
const char* password = "vsesoba-148b";

bool spiffsActive = false;

// declare an object of class AsyncWebServer, which will expose the methods needed for us to setup the HTTP server and handle the incoming requests.
AsyncWebServer server(80);  // the default HTTP port 80

String from_mega = "";                                                  // lidar distance value in String format
int is = 0;
char buf[40];                                                           //  Serial bus buffer

void setup() {
  Serial.begin(9600);
  // Start filing subsystem
  if (SPIFFS.begin()) {
    Serial.println(" SPIFFS Activated ^_^");
    spiffsActive = true;
  } else {
    Serial.println(" Unabale to activate SPIFFS -_-");
  }
  // open a serial connection and then we will
  // connect the ESP8266 to the WiFi network
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println(" Connecting to WiFi... ... ");
  }
  // After the connection is established, we will print the ESP8266 IP on the local network
  // so we can then reach the server from inside the local network.
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());


  // setup a server route and a handling function that will be executed when a request is made on that route
  // by calling the 'on' method on our previously declared server object

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
    //// Handling function implementation

    request->send(SPIFFS, "/index.html", "text/html", false, processor);
    Serial.println(" HTML GET ");

    int paramsNr = request->params();  //get number of params
    //    Serial.print(" Number of params: ");
    //    Serial.println(paramsNr);

    for (byte i = 0; i < paramsNr; i++) {
      AsyncWebParameter* p = request->getParam(0);
      Serial.print(" Name of param: ");
      Serial.println(p->name());

      if ((p->name()) == "speed")
        Serial.print("SPEED:");
      else if ((p->name()) == "dist")
        Serial.print("MOVE:");
      else if ((p->name()) == "dire")
        Serial.print("TURN:");
      else if ((p->name()) == "led_state")
        Serial.print("LED:");

      Serial.println((p->value()));
    }
  });  //end of server on GET

  server.on("/script.js", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/script.js", "text/javascript");
    Serial.println(" JS script GET ");
  });

  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/style.css", "text/css");
    Serial.println(" CSS style GET ");
  });

  server.on("/compass.png", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/compass.png", "image/x-png");
    Serial.println(" PNG compass GET ");
  });

  //Route to load style.css file
  server.on("/jquery-1.11.3.min.js", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/jquery-1.11.3.min.js", "text/javascript");
    Serial.println(" JS jquery GET ");
  });
  // Route to load style.css file
  server.on("/roundslider.min.js", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/roundslider.min.js", "text/javascript");
    Serial.println(" min JS GET ");
  });
  server.on("/roundslider.min.css", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/roundslider.min.css", "text/css");
    Serial.println(" min css GET ");
  });

  server.on("/from_MEGA", HTTP_GET, [](AsyncWebServerRequest * request)  {
    request->send_P(200, "text/plain", string_to_JS().c_str());               // return value to web page JS
  });

  // To finalize the server setup, we need to call the 'begin' method of the server object
  // so the server starts listening for incoming requests.
  server.begin();
}

// Replace placeholder with A/D value
String processor(const String& var) {
  ;
  return "";
}

String string_to_JS()  {                                                         // funtion to be called with GET from_MEGA tag
  return from_mega;
}

void loop() {
  if (Serial.available() > 0)
  {
    serial_read();                                            // read string from Mega
    from_mega = buf;

    String buf_tem(buf);

    int pos1 = buf_tem.indexOf("=");                              // parse mega string for check syntax in serial monitor
    int len = buf_tem.length();                                   // remember to ignore in final solution

    String COM, VAL;

    COM = buf_tem.substring(0, pos1);
    VAL = buf_tem.substring(pos1 + 1, len);
    // print command and value
    Serial.print(" COM=<"); Serial.print(COM); Serial.println("> ");  // print command
    Serial.print(" VAL=<"); Serial.print(VAL); Serial.println("> ");  // print value
  }
}

void serial_read() {                                // read serial in string, save received characters to buf[]
  boolean done = 0;

  while (done == 0)                                   // wait end of message LF(left feed,new line)
  {
    while (Serial.available() > 0)
    {
      char char_in = Serial.read();

      if (char_in > 13)                           // is input char above CR(carriage return)
      {
        buf[is] = char_in;
        is++;
      }

      if (char_in == 10) {                        // check if end of message  LF
        buf[is] = 0;                              // end of string
        is = 0;
        done = 1;
      }

    }                                            // end of while serial available

    if (done == 1) {
      done = 0;
      break;
    }

  }                                                 // while END , End of message  LF

  Serial.println(buf);

}                                                   // end of serial read
