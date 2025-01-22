#include <Stepper.h>

const int stepsPerRevolution = 215;  // Steps per revolution of the motor
Stepper myStepper(stepsPerRevolution, 8, 9, 10, 11);

const int flexSensorPin = A0;        // Flex sensor pin
const int muscleSensorPin = A1;      // MyoWare muscle sensor pin

int flexSensorValue = 0;             // Variable to store flex sensor reading
int muscleSensorValue = 0;           // Variable to store muscle sensor reading
int baselineMuscleValue = 0;         // Baseline muscle sensor value

const int flexTargetValue = 210;     // Target flex sensor value
bool isReturning = false;            // Flag to track if motor is returning
bool motorActive = false;            // Flag to track if the motor is currently moving

void setup() {
  Serial.begin(9600);
  myStepper.setSpeed(30);            // Set motor speed (RPM)

  // Calibrate the muscle sensor for 5 seconds
  Serial.println("Calibrating muscle sensor...");
  long sum = 0;
  int numReadings = 0;
  unsigned long startTime = millis();
  while (millis() - startTime < 5000) { // 5 seconds calibration
    muscleSensorValue = analogRead(muscleSensorPin);
    sum += muscleSensorValue;
    numReadings++;
    delay(10); // Short delay for consistent readings
  }
  baselineMuscleValue = sum / numReadings; // Calculate average baseline value
  Serial.print("Calibration complete. Baseline muscle value: ");
  Serial.println(baselineMuscleValue);
}

void loop() {
  // Read sensor values
  flexSensorValue = analogRead(flexSensorPin);
  muscleSensorValue = analogRead(muscleSensorPin);

  // Debugging: Print sensor values to Serial Monitor
  Serial.print("Flex Sensor: ");
  Serial.print(flexSensorValue);
  Serial.print(" | Muscle Sensor: ");
  Serial.println(muscleSensorValue);

  // Check for significant deviation from baseline
  if (!motorActive && abs(muscleSensorValue - baselineMuscleValue) > baselineMuscleValue * 0.2) {
    motorActive = true;              // Activate motor
    isReturning = false;             // Ensure forward direction
    Serial.println("Muscle activation detected. Starting motor.");
  }

  if (motorActive) {
    if (!isReturning) {
      // Move motor forward
      myStepper.step(1);

      // Check if flex sensor has reached the target
      if (flexSensorValue >= flexTargetValue) {
        isReturning = true;          // Reverse direction
        Serial.println("Target flex value reached. Reversing motor.");
      }
    } else {
      // Move motor backward
      myStepper.step(-1);

      // Stop motor if it returns below the flex target value
      if (flexSensorValue < flexTargetValue - 10) { // Adjust threshold for stopping
        motorActive = false;
        Serial.println("Motor returned to starting position. Stopping motor.");
      }
    }
  }

  delay(10); // Small delay to stabilize readings
}
