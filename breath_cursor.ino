#include <Wire.h> // I2C communication for MPU6050
#include <MPU6050.h> // MPU6050 library
#include <Mouse.h> // Mouse emulation for USB HID

MPU6050 mpu; // Initialize MPU6050

const int pressureSensorPin = A0; // Pin for the air pressure sensor
const int pressureThreshold = 500; // Adjust this threshold based on your sensor
const int clickInterval = 500; // Time interval to differentiate between click types

unsigned long lastBlowTime = 0; // Timestamp of the last detected blow
int blowCount = 0; // Counter for the number of blows within the click interval

void setup() {
  Serial.begin(9600); // Start serial communication for debugging
  Mouse.begin(); // Start mouse emulation
  Wire.begin(); // Initialize I2C communication
  mpu.initialize(); // Initialize MPU6050 (if used)
}

void loop() {
  // Read the pressure sensor to detect air blows
  int pressureValue = analogRead(pressureSensorPin);

  if (pressureValue > pressureThreshold) {
    unsigned long currentTime = millis(); // Current time in milliseconds
    
    if (currentTime - lastBlowTime < clickInterval) {
      blowCount++;
    } else {
      blowCount = 1; // Reset the count if the time interval has passed
    }
    
    lastBlowTime = currentTime; // Update the last blow time

    // Determine the action based on the number of blows
    if (blowCount == 1) {
      Mouse.click(MOUSE_LEFT); // Single click
    } else if (blowCount == 2) {
      Mouse.click(MOUSE_LEFT); // Double click
    } else if (blowCount == 3) {
      Mouse.click(MOUSE_RIGHT); // Right-click
    }
  }

  // (Optional) Implement motion tracking with MPU6050 for cursor movement
  int16_t ax, ay, az; // Acceleration values
  if (mpu.testConnection()) {
    mpu.getAcceleration(&ax, &ay, &az); // Get accelerometer data

    // Scale the acceleration values for mouse movement
    int deltaX = ax / 1000; // Horizontal movement
    int deltaY = ay / 1000; // Vertical movement
    Mouse.move(deltaX, deltaY); // Move the cursor based on acceleration
  }

  delay(100); // Short delay to avoid rapid re-triggering
}