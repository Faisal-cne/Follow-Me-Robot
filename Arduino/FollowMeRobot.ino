Software Development
The following code was written in the Arduino IDE and is responsible for the robot's
primary functionalities. The RFID tag UID was updated to match the provided value:
A9 EC F4 B8.
#include <SPI.h>
#include <MFRC522.h>
// RFID RC522 Pins
#define SS_PIN 10
#define RST_PIN 9
MFRC522 rfid(SS_PIN, RST_PIN);
50
// Ultrasonic Sensor Pins
#define TRIG_PIN A1
#define ECHO_PIN A2
// Motor Driver Pins
#define IN1 4
#define IN2 5
#define IN3 6
#define IN4 7
// Define RFID tag UID for following mode
byte validUID[] = {0xA9, 0xEC, 0xF4, 0xB8}; // Updated UID
bool tagDetected = false;
// Function prototypes
void moveForward();
void stopMotors();
bool isObstacleDetected();
bool isTagDetected();
void setup() {
// Motor pins as outputs
pinMode(IN1, OUTPUT);
pinMode(IN2, OUTPUT);
pinMode(IN3, OUTPUT);
pinMode(IN4, OUTPUT);
// Ultrasonic sensor pins
pinMode(TRIG_PIN, OUTPUT);
pinMode(ECHO_PIN, INPUT);
// RFID initialization
SPI.begin();
rfid.PCD_Init();
Serial.begin(9600); // For debugging
Serial.println("Robot Ready");
}
void loop() {
tagDetected = isTagDetected();
if (tagDetected) {
if (isObstacleDetected()) {
stopMotors(); // Stop if obstacle is detected
51
Serial.println("Obstacle detected. Stopping.");
} else {
moveForward(); // Move forward if tag is detected and no obstacles
Serial.println("Tag detected. Moving forward.");
}
} else {
stopMotors(); // Stop if tag is not detected
Serial.println("No tag detected. Stopping.");
}
delay(100); // Loop delay for stability
}
// Function to check for obstacle using ultrasonic sensor
bool isObstacleDetected() {
digitalWrite(TRIG_PIN, LOW);
delayMicroseconds(2);
digitalWrite(TRIG_PIN, HIGH);
delayMicroseconds(10);
digitalWrite(TRIG_PIN, LOW);
long duration = pulseIn(ECHO_PIN, HIGH);
int distance = duration * 0.034 / 2; // Distance in cm
return distance < 20; // Obstacle detected if within 20 cm
}
// Function to detect RFID tag
bool isTagDetected() {
if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial()) {
return false; // No tag detected
}
// Compare detected UID with valid UID
for (byte i = 0; i < rfid.uid.size; i++) {
if (rfid.uid.uidByte[i] != validUID[i]) {
rfid.PICC_HaltA(); // Halt communication if UID doesn't match
return false;
}
}
rfid.PICC_HaltA(); // Halt communication after successful read
return true;
}
52
// Function to move forward
void moveForward() {
digitalWrite(IN1, HIGH);
digitalWrite(IN2, LOW);
digitalWrite(IN3, HIGH);
digitalWrite(IN4, LOW);
}
// Function to stop motors
void stopMotors() {
digitalWrite(IN1, LOW);
digitalWrite(IN2, LOW);
digitalWrite(IN3, LOW);
digitalWrite(IN4, LOW);
}
