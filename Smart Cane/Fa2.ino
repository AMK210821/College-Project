#include <ESP8266WiFi.h>

int buttonPin1 = 4; // button pin 1
int buttonPin2 = 5; // button pin 2
int ledPin = 2; // LED pin
int trigPin = 4; // ESP8266 pin 4 (GPIO 4)
int echoPin = 0; // ESP8266 pin 0 (GPIO 0)
int buzzerPin = 5; // ESP8266 pin 5 (GPIO 5)
int buttonState = 1;
int x = 1;
const char* ssid = "Galaxy F14";
const char* password = "14111411";
const char* host = "maker.ifttt.com";
const char* privateKey = "l3gSai7MTlRiBwx1I6T1v-Hoy6hENL2Tbwa45moAW96"; // replace with your IFTTT private key
const char* eventName1 = "Alert"; // replace with your first IFTTT event name
const char* eventName2 = "Alert"; // replace with your second IFTTT event name

void setup() {
  pinMode(buttonPin1, INPUT_PULLUP);
  pinMode(buttonPin2, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT); // assuming the built-in LED is used
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT) ;
  pinMode(buzzerPin, OUTPUT);
  Serial.begin(115200);

  Serial.println("Email from Node Mcu");
  delay(100);

  Serial.print("connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(ledPin, HIGH);
    delay(100);
    Serial.print(".");
    digitalWrite(ledPin, LOW);
    delay(100);
  }
  Serial.println("");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  int buttonState1 = digitalRead(buttonPin1);
  int buttonState2 = digitalRead(buttonPin2);

  int duration, distance;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");
  if (distance < 60) {
    digitalWrite(buzzerPin, HIGH);
  } else {
    digitalWrite(buzzerPin, LOW);
  }

  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    digitalWrite(ledPin, LOW);
    delay(100);
    digitalWrite(ledPin, HIGH);
    delay(100);
    return;
  }

  if (buttonState1 == 0 && buttonState2 == 0) {
    // Trigger the first event
    String url1 = "/trigger/" + String(eventName1) + "/with/key/" + String(privateKey);
    digitalWrite(ledPin, HIGH);
    Serial.print("Requesting URL for event 1: ");
    Serial.println(url1);

    client.print(String("GET ") + url1 + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" +
                 "Connection: close\r\n\r\n");

    delay(1000);
    
    // Trigger the second event
    String url2 = "/trigger/" + String(eventName2) + "/with/key/" + String(privateKey);
    Serial.print("Requesting URL for event 2: ");
    Serial.println(url2);

    client.print(String("GET ") + url2 + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" +
                 "Connection: close\r\n\r\n");

    delay(1000);
    digitalWrite(ledPin, LOW);
  }
  delay(50);
}
