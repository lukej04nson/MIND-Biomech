// Define how many sensors we have (original + EMG)
#define NUM_SENSORS 5
#define EMG_SENSOR_PIN A5 // Define the pin for the EMG sensor

// Assign the analog pins to each sensor
const int sensorPins[NUM_SENSORS] = {A0, A1, A2, A3, A4};

// Assign digital output pins corresponding to each sensor
const int outputPins[NUM_SENSORS] = {8, 9, 10, 11, 12};

// Store sensor values in an array
int sensorValues[NUM_SENSORS];

// Variable to store the EMG sensor value
int emgValue;

void setup() {
  Serial.begin(9600);
  
  // Set up sensor pins (INPUT) and outputs
  for (int i = 0; i < NUM_SENSORS; i++) {
    pinMode(sensorPins[i], INPUT);
    pinMode(outputPins[i], OUTPUT);
  }
  
  // Set up the EMG sensor pin
  pinMode(EMG_SENSOR_PIN, INPUT);
}

void loop() {
  // Read each sensor, update its digital output
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

  // Print all sensor readings
  for (int i = 0; i < NUM_SENSORS; i++) {
    Serial.print("Sensor ");
    Serial.print(i + 1);
    Serial.print(": ");
    Serial.print(sensorValues[i]);
    Serial.print("   ");
  }

  // Print the EMG sensor reading
  Serial.print("EMG Sensor: ");
  Serial.println(emgValue);

  delay(100); // Wait 100 milliseconds
}




