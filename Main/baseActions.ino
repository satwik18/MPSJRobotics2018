
/* Michael Power / St. Joseph Autonomous 2018
 * 
 * This file contains the basic ations the robot can 
 * preform. These actions are to be used in other tabs 
 * to make the robot do interesting things.
 */

#define SENSOR_DELAY 50 // The ammount of mills to wait in between sensor checks

void rotate(double deg, int spd) {
  p.resetEncoders(); // reset encoders to clear the previous rotations

  // Using the circumference of the base, calculate how far the robot needs to spin to turn deg

  double rotateDistance = (deg / 360.0) * WHEEL_BASE_C;

  // Convert the distance needed to turn to degrees the wheels must turn
  double degreesToTurn = rotateDistance / WHEEL_C * 360;

  // Add the degrees to turn to the wheel
  double rightDeg = degreesToTurn * 4.0;
  double leftDeg = -degreesToTurn * 4.0;

  // Tell the robot to make the adjustments at a spd
  //p.setMotorDegrees(spd, rightDeg, spd, leftDeg);
  p.setMotorTargets(spd, rightDeg, spd, leftDeg);

  while(p.readMotorBusy(RIGHT_MOTOR) || p.readMotorBusy(LEFT_MOTOR)) {
    delay(SENSOR_DELAY);
  }
  delay(100);
}

// Moves the robot forward at a spd for a time in mills
void forward(int mills, int spd) {
  p.setMotorSpeeds(MAX_SPEED, MAX_SPEED);
  delay(mills);
  p.setMotorSpeeds(0, 0); // turns the moters off when done
}

// Moves forward by inches at a spd
void forwardBy(double inches, int spd) {
  double deg = inches / WHEEL_C * 360.0;
  p.resetEncoders(); // reset encoders to clear the previous rotations
  p.setMotorDegrees(spd, deg, spd, deg);

  while(p.readMotorBusy(RIGHT_MOTOR) || p.readMotorBusy(LEFT_MOTOR)) {
    delay(SENSOR_DELAY);
  }
  delay(100);  
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

// Decelerates over mills from fromSpeed to 0 dps
void decelerateFor(int mills, int fromSpeed) {
  double stepCount = 10; // How many steps in the decceleration
  double timeStep = mills / stepCount; // how long does each step run for
  double speedStep = fromSpeed / stepCount;

  double totalSpeed = fromSpeed;
  for (int i = 0; i < stepCount; i++) {
    totalSpeed -= speedStep;
    p.setMotorSpeeds(totalSpeed, totalSpeed);
    delay(timeStep);
  }
  p.setMotorSpeeds(0, 0); // Just ensure we always end at 0 dps
}



void parallelCorrection(int sideSensor, int closeDist, int farDist) {
  int rotationDeg = 10;
  double backupDist = 7.0;
  if (sideSensor == LEFT_SS) rotationDeg *= -1;

    if (p.readSonicSensorCM(sideSensor) < closeDist) {
      p.setMotorSpeeds(0, 0);
      rotate(rotationDeg, 100);
      forwardBy(-backupDist, 100);
      rotate(-rotationDeg, 100);
      forwardBy(-sin(rotationDeg) * backupDist, MAX_SPEED);
      p.setMotorSpeeds(MAX_SPEED, MAX_SPEED);
    }
    if (p.readSonicSensorCM(sideSensor) > farDist) {
      p.setMotorSpeeds(0, 0);
      rotate(-rotationDeg, 100);
      forwardBy(-backupDist, 100);
      rotate(rotationDeg, 100);
      forwardBy(-sin(-rotationDeg) * backupDist, MAX_SPEED);
      p.setMotorSpeeds(MAX_SPEED, MAX_SPEED);
    }
}

// Waits for the next line and corrects if it gets too close to the wall with the sideSensor
void waitForLineWCorrection(double threshold, int sideSensor) {
  int rotationDeg = 10;
  if (sideSensor == LEFT_SS) rotationDeg *= -1;
  while(p.readLineSensor(CLAW_IR) < threshold) {
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
    while(p.readLineSensor(CLAW_IR) < 1.0) {
      if (p.readSonicSensorCM(sideSensor) < 9 && count == 0) {
        p.setMotorSpeeds(0, 0);
        rotate(rotationDeg, 100);
        forwardBy(-7, 100);
        rotate(-rotationDeg, 100);
        p.setMotorSpeeds(MAX_SPEED, MAX_SPEED);
      }
//      if (p.readSonicSensorCM(sideSensor) > 14 && count == 0) {
//        p.setMotorSpeeds(0, 0);
//        rotate(-rotationDeg, 100);
//        forwardBy(-7, 100);
//        rotate(rotationDeg, 100);
//        p.setMotorSpeeds(MAX_SPEED, MAX_SPEED);
//      }
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

void waitForNLine(int n) {
  for (int i = 1; i < n; i++) {
    waitForLine();
    waitForSpace();
  }
  waitForLine();
}

void waitForLine() {
  while(p.readLineSensor(CLAW_IR) < 1.0) {
    delay(SENSOR_DELAY);
  }
}

// Waits for the next space
void waitForSpace() {
  const double threshold = 1.0;
  while(p.readLineSensor(CLAW_IR) >= threshold) {
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
    p.setServoPosition(CLAW_SERVO_NO, deg);
}

// Set the arm to deg (note constants)
void setArm(int deg) {
    p.setServoPosition(ARM_SERVO_NO, deg);
}

