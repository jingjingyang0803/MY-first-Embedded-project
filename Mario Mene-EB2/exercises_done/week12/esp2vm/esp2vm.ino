#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

int pinRed = 5;
int pinYellow = 4;
int pinGreen = 0;
int pinPot = A0;
int oldState = 0;
int newState = 0;

// CHANGE THE WIFI SSID AND PASSWORD TO MATCH YOUR WIFI
const char *WIFI_SSID = "SOURCE";
const char *WIFI_PASSWORD = "Pelle!23";

// CHANGE TO THE IP ADDRESS OF THE COMPUTER WITH THE MQTT BROKER (MOSQUITTO)
const char *MQTT_HOST = "10.5.1.69";
const int MQTT_PORT = 1883;
const char *MQTT_CLIENT_ID = "randomEspId873219";
const char *MQTT_USER = "";
const char *MQTT_PASSWORD = "";
#define TOPIC "espTopic"
#define TOPIC2 "anotherTopic"

/*
IMPORTANT: configure your mqtt broker (mosquitto) to allow anonymous connections

open the mosquitto config file:
$ sudo nano /etc/mosquitto/mosquitto.conf

and add these 2 lines to the end of the file to allow anonymous
MQTT connections (without username and password):

allow_anonymous true
listener 1883

restart your mqtt broker (mosquitto):
$ sudo service mosquitto restart

make sure your mqtt broker (mosquitto) is running:
$ sudo service mosquitto status

************************************************
**                                            **
** $ mosquitto_pub -t "espTopic" -m "{TFL:2}" **
**                                            **
************************************************

replace the number 2 in "{TFL:2}" with desired number (1, 2 or 3)

*/

WiFiClient client;
PubSubClient mqttClient(client);

void trafficLights(int state) {
  switch(state) {
    case 1:
      digitalWrite(pinRed, HIGH);
      digitalWrite(pinYellow, LOW);
      digitalWrite(pinGreen, LOW);
      break;
    case 2:
      digitalWrite(pinRed, LOW);
      digitalWrite(pinYellow, HIGH);
      digitalWrite(pinGreen, LOW);
      break;
    case 3:
      digitalWrite(pinRed, LOW);
      digitalWrite(pinYellow, LOW);
      digitalWrite(pinGreen, HIGH);
      break;
    default:
      digitalWrite(pinRed, LOW);
      digitalWrite(pinYellow, LOW);
      digitalWrite(pinGreen, LOW);
      break;
  }
}

void callback(char* topic, byte* payload, unsigned int length)
{
    Serial.println("***");
    
    String topicStr = String(topic);
    Serial.println("TOPIC: " + topicStr);
    
    payload[length] = '\0';
    String valueStr = String((char*) payload);
    Serial.println("JSON: " + valueStr);

    String jsonStr = valueStr.substring(valueStr.indexOf('{') + 1, valueStr.indexOf('}'));
    int separator = jsonStr.indexOf(':');
    
    String propStr = jsonStr.substring(0, separator);
    if(propStr[0] == '"' || propStr[0] == '\'') {
      propStr = propStr.substring(1, propStr.length() - 1);
    }
    Serial.println("JSON Property: " + propStr);

    String valStr = jsonStr.substring(separator + 1);
    if(valStr[0] == '"' || valStr[0] == '\'') {
      valStr = valStr.substring(1, valStr.length() - 1);
    }
    Serial.println("JSON Value: " + valStr);

    if(propStr == "TFL") {
      trafficLights(valStr.toInt());
    } else {
      Serial.println("Unknown command: " + propStr);
    }

    Serial.println("***");
    Serial.println();
}

void setup()
{
    Serial.begin(9600);

    pinMode(pinRed, OUTPUT);
    pinMode(pinYellow, OUTPUT);
    pinMode(pinGreen, OUTPUT);

    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("Connected to Wi-Fi");
    
    Serial.println(WiFi.localIP());

    mqttClient.setServer(MQTT_HOST, MQTT_PORT);
    mqttClient.setCallback(callback);

    while (!client.connected()) {
        if (mqttClient.connect(MQTT_CLIENT_ID)) {
            Serial.println("Connected to MQTT broker");
        } else {
            delay(500);
            Serial.print(".");
        }
    }

    mqttClient.subscribe(TOPIC2);
}

void sendMqttMsg(int state) {
  if(state == 1) {
    mqttClient.publish("espTopic", "{TFL:1}");
  } else if (state == 2) {
    mqttClient.publish("espTopic", "{TFL:2}");
  } else if (state == 3) {
    mqttClient.publish("espTopic", "{TFL:3}");
  } else {
    mqttClient.publish("espTopic", "{TFL:0}");
  }
}

void setTFL(int potVal) {

  if (potVal < 300) {
    newState = 0;
  } else if (potVal < 600) {
    newState = 1;
  } else if (potVal < 900) {
    newState = 2;
  } else {
    newState = 3;
  }

  if (oldState == newState) {
    return;
  }

  oldState = newState;
  trafficLights(newState);
  sendMqttMsg(newState);
}

void loop()
{
    mqttClient.loop();
    
    setTFL(analogRead(pinPot));
}
