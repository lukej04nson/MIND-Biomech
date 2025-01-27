All 5 Test With EMG:

// Define pin connections for the flex sensors and EMG sensor
const int flexPins[4] = {A0, A1, A2, A3};
const int emgPin = A5;  // EMG sensor pin


// Arrays to store readings and calibration values for flex sensors
int flexValues[4];
int minValue[4] = {1023, 1023, 1023, 1023};  // Initialize to max possible value
int maxValue[4] = {0, 0, 0, 0};              // Initialize to min possible value


// Variables for EMG calibration and readings
int emgValue;
int emgMin = 1023;
int emgMax = 0;


void setup() {
 Serial.begin(9600);


 // Initialize flex sensor pins
 for (int i = 0; i < 4; i++) {
   pinMode(flexPins[i], INPUT);
 }
 pinMode(emgPin, INPUT);  // Initialize EMG pin


 // Calibration - sample each sensor to find min and max values
 Serial.println("Calibrating... Flex each sensor and contract muscles, then press any key to finish.");
 while (Serial.available() == 0) {
   for (int i = 0; i < 4; i++) {
     int reading = analogRead(flexPins[i]);
     if (reading < minValue[i]) minValue[i] = reading;
     if (reading > maxValue[i]) maxValue[i] = reading;
   }
  
   // Calibrate EMG sensor
   int emgReading = analogRead(emgPin);
   if (emgReading < emgMin) emgMin = emgReading;
   if (emgReading > emgMax) emgMax = emgReading;
  
   delay(50);  // Small delay for stability
 }
 Serial.read();  // Clear the input buffer
 Serial.println("Calibration complete.");
}


void loop() {
 // Read and calibrate flex sensor values
 for (int i = 0; i < 4; i++) {
   flexValues[i] = analogRead(flexPins[i]);


   // Map the calibrated value to a percentage
   int flexPercent = map(flexValues[i], minValue[i], maxValue[i], 0, 100);
   flexPercent = constrain(flexPercent, 0, 100);  // Ensure values stay within 0-100%


   // Print the calibrated output for flex sensor
   Serial.print("Flex Sensor ");
   Serial.print(i);
   Serial.print(": ");
   Serial.print(flexPercent);
   Serial.print("%  ");
 }


 // Read and calibrate EMG sensor value
 emgValue = analogRead(emgPin);
 int emgPercent = map(emgValue, emgMin, emgMax, 0, 100);
 emgPercent = constrain(emgPercent, 0, 100);  // Ensure values stay within 0-100%


 // Print the calibrated output for EMG sensor
 Serial.print(" | EMG: ");
 Serial.print(emgPercent);
 Serial.println("%");


 Serial.println();  // Newline for readability between loops
 delay(500);        // Adjust delay as needed for sampling rate
}





