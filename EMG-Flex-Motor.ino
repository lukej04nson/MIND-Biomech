#include <Stepper.h>

// Define the number of sensors (flex + EMG)
#define NUM_SENSORS 5
#define EMG_SENSOR_PIN A5  // Define the pin for the EMG sensor
#define BASELINE_SAMPLES 100  // Number of samples to calculate the baseline
#define HYSTERESIS 100  // Hysteresis value to prevent oscillation

// Assign the analog pins to each sensor (flex sensors)
const int sensorPins[NUM_SENSORS] = {A0, A1, A2, A3, A4};

// Variables for sensor values and EMG baseline
int sensorValues[NUM_SENSORS];
int emgValue;
int emgBaseline = 0;
bool motorRunning = false;  // Track motor state

// Stepper motor setup
const int stepsPerRevolution = 1300;  // Adjust this to fit your motor
Stepper myStepper(stepsPerRevolution, 8, 9, 10, 11);
int motorSpeed = 25;  // Motor speed for movement (adjust as needed)

bool motorActive = false; // Track if the motor is currently running
unsigned long motorStartTime = 0;  // Store the time when the motor starts
const unsigned long motorRunDuration = 10000;  // 10 seconds (in milliseconds)

void setup() {
  Serial.begin(9600);

  // Set up sensor pins (INPUT)
  for (int i = 0; i < NUM_SENSORS; i++) {
    pinMode(sensorPins[i], INPUT);
  }

  // Set up the EMG sensor pin
  pinMode(EMG_SENSOR_PIN, INPUT);

  // Stepper motor initialization
  myStepper.setSpeed(motorSpeed);  // Set motor speed once

  // Calculate the EMG sensor baseline
  calculateEMGBaseline();
}

void loop() {
  // Read each flex sensor and update their values
  for (int i = 0; i < NUM_SENSORS; i++) {
    sensorValues[i] = analogRead(sensorPins[i]);
  }

  // Print the flex sensor readings
  for (int i = 0; i < NUM_SENSORS; i++) {
    Serial.print("Sensor ");
    Serial.print(i + 1);
    Serial.print(": ");
    Serial.print(sensorValues[i]);
    Serial.print("   ");
  }

  // Read the EMG sensor value
  emgValue = analogRead(EMG_SENSOR_PIN);
  Serial.print("EMG Sensor: ");
  Serial.println(emgValue);

  // Check if EMG value exceeds threshold and motor is not running
  if (emgValue > 100 && !motorActive) {
    motorStartTime = millis();  // Record motor start time
    motorActive = true;
    Serial.println("Motor started");

    // Move the motor continuously for a fixed number of steps
    myStepper.step(stepsPerRevolution * 5);  // Adjust step count for desired motion
  }

  // Stop motor after 10 seconds
  if (motorActive && millis() - motorStartTime >= motorRunDuration) {
    motorActive = false;  // Stop the motor
    Serial.println("Motor stopped after 10 seconds");
  }

  delay(100);  // Short delay for sensor reading
}

// Function to calculate EMG baseline
void calculateEMGBaseline() {
  long sum = 0;
  
  // Take multiple readings to get an average baseline
  for (int i = 0; i < BASELINE_SAMPLES; i++) {
    sum += analogRead(EMG_SENSOR_PIN);
    delay(10);  // Short delay between readings
  }

  emgBaseline = sum / BASELINE_SAMPLES;
  Serial.print("EMG Baseline: ");
  Serial.println(emgBaseline);
}
