
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

// Moves forward by inches at a spd
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

// Waits for the next line. While waiting for the line, the distance from the wall 
// will be maintained between closeCorrection and farCorrection. When the bot arrives
// at the line, it will align itself perpendicular to the line by turning off whichever
// weel arrives first.
void waitForNextLineAdjusted(int sideSensor, int closeCorrection, int farCorrection) {
  int rotationDeg = 10;
  double backupDist = 7.0;
  if (sideSensor == LEFT_SS) rotationDeg *= -1;

  double rightReading = p.readLineSensor(RIGHT_IR);
  double leftReading = p.readLineSensor(LEFT_IR);
  while(rightReading < LINE_DETECTION_THRESHOLD && leftReading < LINE_DETECTION_THRESHOLD) {
    if (p.readSonicSensorCM(sideSensor) < closeCorrection && sideSensor != -1) {
      p.setMotorSpeeds(0, 0);
      rotate(rotationDeg, 100);
      forwardBy(-backupDist, 100);
      rotate(-rotationDeg, 100);
      forwardBy(sin(rotationDeg) * backupDist, MAX_SPEED);
      p.setMotorSpeeds(MAX_SPEED, MAX_SPEED);
    }
    if (p.readSonicSensorCM(sideSensor) > farCorrection && sideSensor != -1) {
      p.setMotorSpeeds(0, 0);
      rotate(-rotationDeg, 100);
      forwardBy(-backupDist, 100);
      rotate(rotationDeg, 100);
      forwardBy(sin(-rotationDeg) * backupDist, MAX_SPEED);
      p.setMotorSpeeds(MAX_SPEED, MAX_SPEED);
    }
    delay(SENSOR_DELAY);
    rightReading = p.readLineSensor(RIGHT_IR);
    leftReading = p.readLineSensor(LEFT_IR);
  }

  while(rightReading < LINE_DETECTION_THRESHOLD || leftReading < LINE_DETECTION_THRESHOLD) {
    if (rightReading >= LINE_DETECTION_THRESHOLD) p.setMotorSpeed(RIGHT_MOTOR, 0);
    else p.setMotorSpeed(RIGHT_MOTOR, MAX_SPEED);
    if (leftReading >= LINE_DETECTION_THRESHOLD) p.setMotorSpeed(LEFT_MOTOR, 0);
    else p.setMotorSpeed(LEFT_MOTOR, MAX_SPEED);
    // no delay for high accuracy
    rightReading = p.readLineSensor(RIGHT_IR);
    leftReading = p.readLineSensor(LEFT_IR);
  }
}

// Moves forward at max speed until the nth line is detected and adjusted on.
// "Parallel parking" adjustments by the close and far correction distances in CM.
void driveToNthAdjustedLine(int nth, int sideSensor, int closeCorrection, int farCorrection) {
  p.setMotorSpeeds(MAX_SPEED, MAX_SPEED);
  for (int i = 0; i < sideSensor - 1; i++) {
    waitForNextLineAdjusted(sideSensor, closeCorrection, farCorrection);
    p.setMotorSpeeds(MAX_SPEED, MAX_SPEED);
    waitForSpace();
  }
  waitForNextLineAdjusted(sideSensor, closeCorrection, farCorrection);
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

