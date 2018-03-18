
/* Michael Power / St. Joseph Autonomous 2018
 * 
 * This file contains the basic ations the robot can 
 * preform. These actions are to be used in other tabs 
 * to make the robot do interesting things.
 */

#define SENSOR_DELAY 50 // The ammount of mills to wait in between sensor checks

// Rotates the robot by deg clockwise at a spd
void rotate(double deg, int spd) {
  const double threshold = 2.5;
  p.resetEncoders(); // reset encoders to clear the previous rotations
  
  // Read the current angle of the wheel (may be always 0 since line above)
  long rightDeg = p.readEncoderDegrees(1); 
  long leftDeg = p.readEncoderDegrees(2);

  // Using the circumference of the base, calculate how far the robot needs to spin to turn deg

  double rotateDistance = (deg / 360.0) * WHEEL_BASE_C;

  // Convert the distance needed to turn to degrees the wheels must turn
  double degreesToTurn = rotateDistance / WHEEL_C * 360;

  // Add the degrees to turn to the wheel
  rightDeg += degreesToTurn;
  leftDeg -= degreesToTurn;

  // Tell the robot to make the adjustments at a spd
  //p.setMotorDegrees(spd, rightDeg, spd, leftDeg);
  p.setMotorTargets(spd, rightDeg * 4.0, spd, leftDeg * 4.0);

  while(abs(p.readEncoderDegrees(1) - rightDeg) > threshold && abs(p.readEncoderDegrees(2) - leftDeg) > threshold) {
    delay(SENSOR_DELAY);
  }
  delay(1000);
}


// Moves the robot forward at a spd for a time in mills
void forward(int mills, int spd) {
  p.setMotorSpeeds(MAX_SPEED, MAX_SPEED);
  delay(mills);
  p.setMotorSpeeds(0, 0); // turns the moters off when done
}

void forwardBy(double inches, int spd) {
  double deg = inches / WHEEL_C * 360.0;
  p.resetEncoders(); // reset encoders to clear the previous rotations
  p.setMotorDegrees(spd, deg, spd, deg);

  while(abs(p.readEncoderDegrees(1) - deg) > 2.5 && abs(p.readEncoderDegrees(2) - deg) > 2.5) {
    delay(SENSOR_DELAY);
  }
  delay(1000);  
}

// Accelerates over mills from 0 dps to toSpeed
void accelerateFor(int mills, int toSpeed) {
  double stepCount = 10; // How many steps in the acceleration
  double timeStep = mills / stepCount; // how long does each step run for
  double speedStep = toSpeed / stepCount; // what is the difference in speed per step
  
  double totalSpeed = 0; // a counter for the current speed. Increases as the speed increases.

  for (int i = 0; i < stepCount; i++) { // For each step
    totalSpeed += speedStep; // Add the difference in speed to the total
    p.setMotorSpeeds(totalSpeed, totalSpeed); // Update the moter speed
    delay(timeStep); // Wait for the duration of this step
  }
}

// Waits for the next line and corrects if it gets too close to the wall with the sideSensor
void waitForLineWCorrection(double threshold, int sideSensor) {
  int rotationDeg = 10;
  if (sideSensor == LEFT_SS) rotationDeg *= -1;
  while(p.readLineSensor(17) < threshold) {
    if (p.readSonicSensorCM(sideSensor) < 9) {
      p.setMotorSpeeds(0, 0);
      rotate(rotationDeg, 100);
      forwardBy(-7, 100);
      rotate(-rotationDeg, 100);
      p.setMotorSpeeds(MAX_SPEED, MAX_SPEED);
    }
    delay(SENSOR_DELAY);
  }
}

// Waits for the lnNumb line and corrects if it gets too close to the wall with the sideSensor
void waitForLineNumWCorrection(int lnNumb, int sideSensor) {
  int count = 0;
  int rotationDeg = 10;
  if (sideSensor == LEFT_SS) rotationDeg *= -1;
  while(count < (lnNumb - 1)){
    p.setGreenLED(1);
    while(p.readLineSensor(17) < 1.0) {
      if (p.readSonicSensorCM(sideSensor) < 9 && count == 0) {
        p.setMotorSpeeds(0, 0);
        rotate(rotationDeg, 100);
        forwardBy(-7, 100);
        rotate(-rotationDeg, 100);
        p.setMotorSpeeds(MAX_SPEED, MAX_SPEED);
      }
      delay(SENSOR_DELAY);
    }
    p.setRedLED(1);
    waitForSpace();
    p.setGreenLED(0);
    p.setRedLED(0);
    count += 1;
  }
  waitForLineWCorrection(1.0, sideSensor);
}

// Waits for the next line
void waitForLine() {
  const double threshold = 1.0;
  while(p.readLineSensor(17) < threshold) {
    delay(SENSOR_DELAY);
  }
}

// Waits for the next lnNumb line
void waitForLineNum(int lnNumb) {
  int count = 0;
  while(count < (lnNumb - 1)){
    p.setGreenLED(1);
    waitForLine();
    p.setRedLED(1);
    waitForSpace();
    p.setGreenLED(0);
    p.setRedLED(0); 
    count += 1;
  }
  waitForLine();
}

// Waits for the next space
void waitForSpace() {
  const double threshold = 1.0;
  while(p.readLineSensor(17) >= threshold) {
    delay(SENSOR_DELAY);
  }
}

// Waits for the front proximity to be below dist with distance correction
void waitForProximityBelowWCorrection(int sensorNo, double dist, int sideSensor) {
  const int rotationDeg = 10;
  const double threshold = 0.5;
  while(abs(p.readSonicSensorCM(sensorNo) - dist) < threshold) {
    if (p.readSonicSensorCM(sideSensor) < 9) {
      p.setMotorSpeeds(0, 0);
      rotate(rotationDeg, 100);
      forwardBy(-7, 100);
      rotate(-rotationDeg, 100);
      p.setMotorSpeeds(MAX_SPEED, MAX_SPEED);
    }
    delay(SENSOR_DELAY);
  }
}

// Waits for the front proximity to be below dist
void waitForProximityBelow(int sensorNo, double dist) {
  const double threshold = 0.5;
  while(abs(p.readSonicSensorCM(sensorNo) - dist) < threshold) {
    delay(SENSOR_DELAY);
  }
}

// Set the claw to deg (note constants)
void setClaw(int deg) {
    p.setServoPosition(2,deg);
}

// Set the arm to deg (note constants)
void setArm(int deg) {
    p.setServoPosition(1,deg);
}
