#include <AccelStepper.h>

#define IN1 8
#define IN2 9
#define IN3 10
#define IN4 11

// Define motor interface type
#define motorInterfaceType 8

// Initialize the stepper with pins and motor interface type
AccelStepper stepper = AccelStepper(motorInterfaceType, IN1, IN3, IN2, IN4);

// Constants
const int flexPin = A0; // Analog pin for the flex sensor
const int threshold = 500; // Threshold value for direction change

void setup() {
  Serial.begin(9600); // Begin serial communication

  // Set maximum speed for the motor
  stepper.setMaxSpeed(500); // Set a reasonable max speed
}

void loop() {
  // Read the flex sensor value
  int sensorValue = analogRead(flexPin);
  Serial.println(sensorValue); // Print the sensor value for debugging

  // Set motor speed and direction based on sensor value
  if (sensorValue < threshold) {
    // Move motor in one direction when value is below the threshold
    stepper.setSpeed(-1000); // Set negative speed for one direction
  } else if (sensorValue > threshold) {
    // Move motor in the opposite direction when value is above the threshold
    stepper.setSpeed(1000); // Set positive speed for the opposite direction
  } else {
    // Stop the motor when the value is around the threshold
    stepper.setSpeed(0); 
  }

  // Run the motor at the set speed
  stepper.runSpeed();

  delay(100); // Small delay to smooth out the readings
}
