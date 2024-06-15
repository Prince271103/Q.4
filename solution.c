#include <Servo.h>

const int soilMoisturePin = A0;
const int pumpPin = 9;
const int potentiometerPin = A1;
const int buzzerPin = 8;
const int ledPin = 7;
const int buttonPin = 2;

Servo servoMotor;

int threshold = 0; // Custom threshold set by potentiometer

void setup() {
  pinMode(soilMoisturePin, INPUT);
  pinMode(pumpPin, OUTPUT);
  pinMode(potentiometerPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  servoMotor.attach(10);
}

void loop() {
  // Read potentiometer value and adjust threshold
  int potValue = analogRead(potentiometerPin);
  threshold = map(potValue, 0, 1023, 0, 1023); // Map potentiometer value to 0-1023 range

  // Check if button is pressed to wake up from sleep mode
  if (digitalRead(buttonPin) == LOW) {
    // Wake up and start monitoring soil moisture
    while (true) {
      int soilMoisture = analogRead(soilMoisturePin);
      
      if (soilMoisture < threshold) {
        // Watering starts
        digitalWrite(ledPin, HIGH);
        tone(buzzerPin, 1000, 50); // Short beep
        digitalWrite(pumpPin, HIGH);
        servoMotor.write(90);
        delay(5000); // Watering duration
        digitalWrite(pumpPin, LOW);
        servoMotor.write(0);
        // Watering stops
        digitalWrite(ledPin, LOW);
        tone(buzzerPin, 1000, 50); // Short beep
      }
      
      delay(1000); // Check soil moisture every second
    }
  } else {
    // Sleep mode: stop all operations and wait for button press
    digitalWrite(pumpPin, LOW);
    servoMotor.write(0);
    digitalWrite(ledPin, LOW);
    noTone(buzzerPin);
    delay(1000); // Wait for 1 second before checking again
  }
}