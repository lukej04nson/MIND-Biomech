#include <Stepper.h>

const int stepsPerRevolution = 200;  // Steps per revolution of the motor
Stepper myStepper(stepsPerRevolution, 8, 9, 10, 11);

const int sensorPin = A0;  // Flex sensor pin
int sensorValue;           // Variable to store flex sensor reading

const int targetSensorValue = 250;  // Target flex sensor value
bool isReturning = false;           // Flag to track if motor is returning
bool reverseStepTaken = false;      // Flag to track if the reverse step has been taken

void setup() {
  Serial.begin(9600);
  myStepper.setSpeed(30); // Set motor speed (RPM)
}

void loop() {
  // Read the flex sensor value
  sensorValue = analogRead(sensorPin);
  Serial.println(sensorValue);

  if (!isReturning && !reverseStepTaken) {
    // Move motor forward
    myStepper.step(1);

    // Check if sensor value has reached the target
    if (sensorValue >= targetSensorValue) {
      isReturning = true; // Start returning
    }
  } else if (isReturning && !reverseStepTaken) {
    // Take one reverse step
    myStepper.step(-1);
    reverseStepTaken = true; // Mark that the reverse step has been completed
  }

  // No further motor movement after taking the reverse step
}
