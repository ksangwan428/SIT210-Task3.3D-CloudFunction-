#include <ArduinoMqttClient.h>

// Include the appropriate Wi-Fi library based on the board being used.
#if defined(ARDUINO_SAMD_MKRWIFI1010) || defined(ARDUINO_SAMD_NANO_33_IOT) || defined(ARDUINO_AVR_UNO_WIFI_REV2)
#include <WiFiNINA.h>
#elif defined(ARDUINO_SAMD_MKR1000)
#include <WiFi101.h>
#elif defined(ARDUINO_ARCH_ESP8266)
#include <ESP8266WiFi.h>
#elif defined(ARDUINO_ARCH_ESP32)
#include <WiFi.h>
#endif


char ssid[] = "Kartik";    // your network SSID (name)
char pass[] = "87654321";

// Define the pin connected to an LED.
int ledPin = 2;

// To establish an SSL/TLS connection (optional):
// 1) Change WiFiClient to WiFiSSLClient.
// 2) Change port value from 1883 to 8883.
// 3) Change broker value to a trusted SSL/TLS server.

// Initialize a Wi-Fi client and MQTT client.
WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);

// Define the MQTT broker information.
const char broker[] = "mqtt-server.com"; // Replace with your MQTT broker server address
int port = 1883;                          // MQTT port (default is 1883)
const char topic[] = "MyTopic/waves";     // MQTT topic to subscribe to

void setup() {
  // Initialize the serial communication and wait for the port to open.
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);

  // Wait for the serial port to connect (only for boards with native USB).
  while (!Serial) {
    ;
  }

  // Attempt to connect to the Wi-Fi network.
  Serial.print("Connecting to SSID: ");
  Serial.println(ssid);

  while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
    // Connection failed, retry every 5 seconds.
    Serial.print(".");
    delay(5000);
  }

  Serial.println("Connected to the network");
  Serial.println();

  // You can set a unique client ID; otherwise, the library uses Arduino-millis()
  // Each client should have a unique client ID
  // mqttClient.setId("myClientId");

  // You can use a username and password for authentication
  // mqttClient.setUsernamePassword("myUsername", "myPassword");

  Serial.print("Connecting to MQTT broker: ");
  Serial.println(broker);

  if (!mqttClient.connect(broker, port)) {
    Serial.print("MQTT connection failed! Error code = ");
    Serial.println(mqttClient.connectError());

    while (1);
  }

  Serial.println("Connected to the MQTT broker!");
  Serial.println();

  Serial.print("Subscribing to topic: ");
  Serial.println(topic);
  Serial.println();

  // Subscribe to the specified MQTT topic.
  mqttClient.subscribe(topic);

  // Topics can be unsubscribed using:
  // mqttClient.unsubscribe(topic);

  Serial.print("Waiting for messages on topic: ");
  Serial.println(topic);
  Serial.println();
}

void loop() {
  int messageSize = mqttClient.parseMessage();

  if (messageSize) {
    // Received a message, print out the topic and contents
    Serial.print("Received a message with topic '");
    Serial.print(mqttClient.messageTopic());
    Serial.print("', length ");
    Serial.print(messageSize);
    Serial.println(" bytes:");

    // Use the Stream interface to print the contents
    while (mqttClient.available()) {
      Serial.print((char)mqttClient.read());
    }

    Serial.println();
    
    // Blink the LED as a visual indicator of receiving a message.
    digitalWrite(ledPin, HIGH);   // Turn the LED on (HIGH is the voltage level)
    delay(200);                       // Wait for a second
    digitalWrite(ledPin, LOW);    // Turn the LED off by making the voltage LOW
    delay(200);
    digitalWrite(ledPin, HIGH);   // Turn the LED on (HIGH is the voltage level)
    delay(200);                       // Wait for a second
    digitalWrite(ledPin, LOW);    // Turn the LED off by making the voltage LOW
    delay(200);
    digitalWrite(ledPin, HIGH);   // Turn the LED on (HIGH is the voltage level)
    delay(200);                       // Wait for a second
    digitalWrite(ledPin, LOW);    // Turn the LED off by making the voltage LOW
    delay(200);

    Serial.println();
  }
}
