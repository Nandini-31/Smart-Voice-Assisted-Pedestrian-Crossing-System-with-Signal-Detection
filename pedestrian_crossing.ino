// 🚦 Smart Voice-Assisted Pedestrian Crossing System with Signal Detection
// Hardware: ESP32, Ultrasonic Sensors, Servo Motor, Voice Playback Module
// Software: Arduino IDE, Blynk IoT

#define BLYNK_TEMPLATE_ID "YourTemplateID"
#define BLYNK_TEMPLATE_NAME "Pedestrian"
#define BLYNK_AUTH_TOKEN "YourBlynkAuthToken"

#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <Servo.h>

// WiFi credentials (replace with your own)
char ssid[] = "YourWiFiName";
char pass[] = "YourWiFiPassword";

// Servo
static const int servoPin = 15;
Servo myservo;

// Ultrasonic sensor pins
#define trigPin1 25
#define echoPin1 26
#define trigPin2 18
#define echoPin2 19
#define trigPin3 16
#define echoPin3 17

// LED indicators
#define ledR 23
#define ledG 21
#define ledY 22

// Voice module pins
#define v1 27
#define v2 14

// Distance variables
int distance1, distance2, distance3;
int duration1, duration2, duration3;

void setup() {
  // Ultrasonic pins
  pinMode(trigPin1, OUTPUT); pinMode(echoPin1, INPUT);
  pinMode(trigPin2, OUTPUT); pinMode(echoPin2, INPUT);
  pinMode(trigPin3, OUTPUT); pinMode(echoPin3, INPUT);

  // LEDs
  pinMode(ledR, OUTPUT);
  pinMode(ledG, OUTPUT);
  pinMode(ledY, OUTPUT);

  // Servo
  myservo.attach(servoPin);

  // Voice module
  pinMode(v1, OUTPUT);
  pinMode(v2, OUTPUT);
  digitalWrite(v1, 1);
  digitalWrite(v2, 1);

  // Serial + Blynk
  Serial.begin(9600);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
}

void loop() {
  ultra1();
  ultra2();
  ultra3();
  Blynk.run();

  if (distance1 < 10) {
    digitalWrite(ledY, 1); digitalWrite(ledR, 0); digitalWrite(ledG, 0);
    digitalWrite(v1, 0); delay(100); digitalWrite(v1, 1);
  } 
  else if (distance2 < 10) {
    digitalWrite(ledY, 0); digitalWrite(ledR, 1); digitalWrite(ledG, 0);
    digitalWrite(v1, 1);
    myservo.write(90);
  } 
  else if (distance3 < 10) {
    digitalWrite(ledY, 0); digitalWrite(ledR, 0); digitalWrite(ledG, 1);
    digitalWrite(v1, 1);
    myservo.write(0);
    Blynk.logEvent("notify", "Use Pedestrian to cross the platform");
  }
}

// --- Ultrasonic distance functions ---
void ultra1() {
  digitalWrite(trigPin1, LOW); delayMicroseconds(2);
  digitalWrite(trigPin1, HIGH); delayMicroseconds(10);
  digitalWrite(trigPin1, LOW);
  duration1 = pulseIn(echoPin1, HIGH);
  distance1 = duration1 * 0.034 / 2;
  Serial.print("Dist1 = "); Serial.println(distance1);
}

void ultra2() {
  digitalWrite(trigPin2, LOW); delayMicroseconds(2);
  digitalWrite(trigPin2, HIGH); delayMicroseconds(10);
  digitalWrite(trigPin2, LOW);
  duration2 = pulseIn(echoPin2, HIGH);
  distance2 = duration2 * 0.034 / 2;
  Serial.print("Dist2 = "); Serial.println(distance2);
}

void ultra3() {
  digitalWrite(trigPin3, LOW); delayMicroseconds(2);
  digitalWrite(trigPin3, HIGH); delayMicroseconds(10);
  digitalWrite(trigPin3, LOW);
  duration3 = pulseIn(echoPin3, HIGH);
  distance3 = duration3 * 0.034 / 2;
  Serial.print("Dist3 = "); Serial.println(distance3);
}
