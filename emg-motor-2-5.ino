#include <Stepper.h>

const int stepsPerRevolution = 1300;  // Steps per revolution for your motor

// initialize the stepper library on pins 8 through 11:
Stepper myStepper(stepsPerRevolution, 8, 9, 10, 11);

// Define how many sensors we have (original + EMG)
#define NUM_SENSORS 5
#define EMG_SENSOR_PIN A5 // Define the pin for the EMG sensor

// Assign the analog pins to each sensor
const int sensorPins[NUM_SENSORS] = {A0, A1, A2, A3, A4};

// Assign digital output pins corresponding to each sensor
const int outputPins[NUM_SENSORS] = {8, 9, 10, 11, 12};

// Store sensor values in an array
int sensorValues[NUM_SENSORS];

// Variable to store the EMG sensor value and baseline
int emgValue;
int emgBaseline = 0; // Baseline value for the EMG sensor
const int emgThreshold = 50; // Threshold for significant change

// Variable to track the motor direction
bool motorDirection = true;  // true = clockwise, false = counter-clockwise

void setup() {
  Serial.begin(9600);

  // Set up sensor pins (INPUT) and outputs
  for (int i = 0; i < NUM_SENSORS; i++) {
    pinMode(sensorPins[i], INPUT);
    pinMode(outputPins[i], OUTPUT);
  }

  // Set up the EMG sensor pin
  pinMode(EMG_SENSOR_PIN, INPUT);

  // Calculate the baseline EMG value by averaging multiple readings
  int emgSum = 0;
  for (int i = 0; i < 100; i++) {
    emgSum += analogRead(EMG_SENSOR_PIN);
    delay(10); // Small delay between readings
  }
  emgBaseline = emgSum / 100;

  // Initialize the motor speed (for smooth operation, we can set a reasonable default)
  myStepper.setSpeed(50);
}

void loop() {
  // Read each flex sensor, update its digital output
  for (int i = 0; i < NUM_SENSORS; i++) {
    sensorValues[i] = analogRead(sensorPins[i]);

    // Control corresponding digital output based on some threshold
    if (sensorValues[i] > 5) {  // Adjust threshold to your application
      digitalWrite(outputPins[i], HIGH);
    } else {
      digitalWrite(outputPins[i], LOW);
    }
  }

  // Read the EMG sensor value
  emgValue = analogRead(EMG_SENSOR_PIN);

  // Determine if there's a significant change in the EMG value
  if (abs(emgValue - emgBaseline) > emgThreshold) {
    // Significant change detected, motor spins in one direction
    motorDirection = true; // Clockwise
  } else {
    // EMG value back to baseline, motor spins in the opposite direction
    motorDirection = false; // Counter-clockwise
  }

  // Set motor direction and step
  if (motorDirection) {
    // Clockwise
    myStepper.step(stepsPerRevolution / 100); // Step motor in small increments
  } else {
    // Counter-clockwise
    myStepper.step(-stepsPerRevolution / 100); // Step motor in reverse
  }

  // Print all sensor readings
  for (int i = 0; i < NUM_SENSORS; i++) {
    Serial.print("Sensor ");
    Serial.print(i + 1);
    Serial.print(": ");
    Serial.print(sensorValues[i]);
    Serial.print("   ");
  }

  // Print the EMG sensor reading and baseline for comparison
  Serial.print("EMG Sensor: ");
  Serial.print(emgValue);
  Serial.print("   Baseline: ");
  Serial.println(emgBaseline);

  delay(100); // Wait 100 milliseconds before the next loop
}
