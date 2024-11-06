#include <AccelStepper.h>

#define IN1 8
#define IN2 9
#define IN3 10
#define IN4 11

// Define motor interface type
#define motorInterfaceType 8

// Initialize with pins and motor interface type
AccelStepper stepper = AccelStepper(motorInterfaceType, IN1, IN3, IN2, IN4);

void setup() {
  Serial.begin(9600);
  
  // Set maximum speed
  stepper.setMaxSpeed(2000); // Maximum speed, positive for direction control
  stepper.setSpeed(-1000);   // Set speed to a negative value to reverse direction
}

void loop() {
  // Run the motor continuously in the opposite direction
  stepper.runSpeed();
}
