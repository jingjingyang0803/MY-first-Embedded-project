#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

int pinRed = 5;
int pinYellow = 4;
int pinGreen = 0;

// CHANGE THE WIFI SSID AND PASSWORD TO MATCH YOUR WIFI
const char *WIFI_SSID = "WiFi Hotspot";
const char *WIFI_PASSWORD = "wifipasswd";

// CHANGE TO THE IP ADDRESS OF THE COMPUTER WITH THE MQTT BROKER (MOSQUITTO)
const char *MQTT_HOST = "192.168.1.23";
const int MQTT_PORT = 1883;
const char *MQTT_CLIENT_ID = "ESP8266 NodeMCU";
const char *MQTT_USER = "";
const char *MQTT_PASSWORD = "";
const char *TOPIC = "espTopic";

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

    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("Connected to Wi-Fi");

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

    mqttClient.subscribe(TOPIC);
}

void loop()
{
    mqttClient.loop();
}
