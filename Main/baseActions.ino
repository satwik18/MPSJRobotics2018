
/* Michael Power / St. Joseph Autonomous 2018
 * 
 * This file contains the basic ations the robot can 
 * preform. These actions are to be used in other tabs 
 * to make the robot do interesting things.
 */

// Rotates the robot by deg clockwise at a spd
void rotate(double deg, int spd) {
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
  p.setMotorDegrees(spd, rightDeg, spd, leftDeg);
}


// Moves the robot forward at a spd for a time in mills
void forward(int mills, int spd) {
  p.setMotorSpeeds(MAX_SPEED, MAX_SPEED);
  delay(mills);
  p.setMotorSpeeds(0, 0); // turns the moters off when done
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

