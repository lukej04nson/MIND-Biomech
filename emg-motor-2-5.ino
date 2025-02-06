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
int motorSpeed = 100;  // Motor speed for movement (adjust as needed)
bool motorDirection = true;  // Track motor direction

void setup() {
  Serial.begin(9600);

  // Set up sensor pins (INPUT)
  for (int i = 0; i < NUM_SENSORS; i++) {
    pinMode(sensorPins[i], INPUT);
  }

  // Set up the EMG sensor pin
  pinMode(EMG_SENSOR_PIN, INPUT);

  // Stepper motor initialization
  myStepper.setSpeed(motorSpeed);  // Initial speed for stepper motor

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

  // Motor control based on EMG sensor deviation from baseline with hysteresis
  if (emgValue > emgBaseline + HYSTERESIS) {
    // If EMG value is significantly higher than baseline, rotate motor in one direction
    if (!motorRunning || !motorDirection) {  // If motor is not already running or is in the wrong direction
      motorRunning = true;
      motorDirection = true;
      myStepper.setSpeed(motorSpeed);
    }
    myStepper.step(stepsPerRevolution / 10);  // Move the motor continuously
  } else if (emgValue < emgBaseline - HYSTERESIS) {
    // If EMG value drops significantly below baseline, reverse the motor direction
    if (!motorRunning || motorDirection) {  // If motor is not already running or is in the wrong direction
      motorRunning = true;
      motorDirection = false;
      myStepper.setSpeed(motorSpeed);
    }
    myStepper.step(-stepsPerRevolution / 10);  // Move the motor in the opposite direction
  } else {
    motorRunning = false;  // Stop the motor when the EMG value is near the baseline
  }

  delay(100);  // Short delay for sensor reading and motor control
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
