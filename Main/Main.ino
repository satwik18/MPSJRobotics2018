/* Michael Power / St. Joseph Autonomous 2018
 * 
 * This file is the main program for the 2018 competition.
 * 
 *  Table of contents:
 *  
 *    - This file - contains the main setup and loop methods and set variabls
 *    - baseActions - contains the basic operations used to complete the problem
 */

#include <PRIZM.h>

#define SIDE_RIGHT -1 // a multiplyer to adjust angles for the right side
#define SIDE_LEFT 1 // a multiplyer to ajust angles for the left side

// Port numbers
#define RIGHT_MOTOR 0 // Right Motor Number // TODO: Fill In
#define LEFT_MOTOR 0 // Left motor number // TODO: Fill In
#define FRONT_SS 5 // Front Super Sonic sensor
#define RIGHT_SS 16 // Right Super Sonic sensor
#define LEFT_SS 15 // Left Super Sonic sensor
#define CLAW_SERVO_NO 2 // Claw servo number
#define ARM_SERVO_NO 1 // Arm servo number
#define TILT_SERVO_NO 0 // Tilting servo number // TODO: Fill In
#define CLAW_IR 17 // The claw IR sensor port number
#define RIGHT_IR 0 // The right correction IR sensor port number // TODO: Fill In
#define LEFT_IR 0 // The left correction IR sensor port number // TODO: Fill In


// Speeds
#define MAX_SPEED 150 //  0 to 720 degrees per second (DPS)
#define TURBO_SPEED 300 // DEEEEEEEPPPPPPPPSSSSSSS
#define CLAW_SERVO_SPEED 40 // In percentage where 100% is max speed
#define TILT_SERVO_SPEED 20 // % // TODO: ADJUST IF NECESSARY

// Servo positions
#define ARM_DOWN 20 // The arm's up position
#define ARM_UP 105 // The arm's down position
#define CLAW_OPEN 150 // The claw's open position
#define CLAW_CLOSED 70 // The claw's closed position
#define TILT_EXTENDED 0 // The extended tilt // TODO: Fill In
#define TILT_RETRACTED 0 // The position when tilt is off // TODO: Fill In


// Robot dimensions
#define WHEEL_WIDTH 4.0 // the wheel is 4 inches tall
#define WHEEL_C (PI * WHEEL_WIDTH) // the wheel circumference in inches
#define WHEEL_BASE_WIDTH 11.0 // the distance between the wheels in inches
#define WHEEL_BASE_C (PI * WHEEL_BASE_WIDTH) // the circumference of the weel base ininches

// Court Dimentions
#define ROTATION_CORRECTION_DIST 10 // The distance that the bot needs to go forward after detecting THE FARTHEST edge of line to line claw up for 90% turn in inches
#define SIDE_TAPE_TO_SKID_DIST 52.75 // The distance from the closest pipe rack tape to the skid // TODO: Fill In
#define WALL_PROX_DIST 9 // How close to the wall in inches to trigger "Parallel park"
#define WALL_FAR_DIST 18 // How far the bot can be from the wall to trigger "Parallel park" // TODO: Fill In
#define FOURTH_PIPE_RETURN_DIST 40.0 // How far back to return for 4th pipe to prep for transition // TODO: Fill In
#define PICKUP_TURBO_DURATION 1000 // Milliseconds to run turbo for (inclides acceleration and deceleration) // TODO: Fill In


PRIZM p; // Prizm library instance

void setup() {
  p.PrizmBegin();
  p.setMotorInvert(1,1);
  Serial.begin(9600); // For sending debug messages to a connected computer
  p.setServoSpeed(CLAW_SERVO_NO, CLAW_SERVO_SPEED);
  p.setServoSpeed(TILT_SERVO_NO, TILT_SERVO_SPEED);
  delay(100);
}

void loop() {
  // open claw
  setClaw(CLAW_OPEN);
  setArm(ARM_DOWN);
  setTilt(TILT_RETRACTED);
  delay(500);

  // lineup for first run
  forwardBy(4.25, MAX_SPEED);
  rotate(89.0, 100);
  

  // pickup pipes 1-3 on the right side
  pickupSidePipe(1, SIDE_RIGHT);
  returnSidePipe(1, SIDE_LEFT);
  rotate(180, 100);
  turboFor(PICKUP_TURBO_DURATION, TURBO_SPEED);
  pickupSidePipe(2, SIDE_RIGHT);
  returnSidePipe(2, SIDE_LEFT);
  rotate(180, 100);
  turboFor(PICKUP_TURBO_DURATION, TURBO_SPEED);
  pickupSidePipe(3, SIDE_RIGHT);
  returnSidePipe(3, SIDE_LEFT);
  rotate(180, 100);
  turboFor(PICKUP_TURBO_DURATION, TURBO_SPEED);


  // Pickup 4th pipe
  pickupSidePipe(4, SIDE_RIGHT);


  driveToNthAdjustedLine(4, SIDE_LEFT, WALL_PROX_DIST, WALL_FAR_DIST);


  forwardBy(FOURTH_PIPE_RETURN_DIST, MAX_SPEED); // Move back to skids enough to catch the tape after turn
  rotate(90, 100); // turn towards the next skid

  // move to next line
  forwardBy(2.5, MAX_SPEED); // Make sure its over the black tape
  driveToNthAdjustedLine(pipeNum, sideSensor, 0, 500); // 0 and 100 should cancel out the ss adjustments

  forwardBy(ROTATION_CORRECTION_DIST, MAX_SPEED); // adjust for claw offset
  


  // rotate to the middle skid and then drop
  rotate(-90, 100);
  forwardBy(2.0, MAX_SPEED); // TODO: Fill In
  delay(250);
  setClaw(CLAW_OPEN);
  delay(1000);
  forwardBy(-11.0, MAX_SPEED);


  // rotate to the third skid
  rotate(90, 100);
  delay(100);
  forwardBy(2.5, MAX_SPEED); // make sure does not read tape
  
  // Move to the third skid line
  driveToNthAdjustedLine(pipeNum, sideSensor, 0, 500); // 0 and 100 should cancel out the ss adjustments
  forwardBy(ROTATION_CORRECTION_DIST + 3.0, MAX_SPEED); // Adjust for offset

  // prepare for first run on the left side
  rotate(90, 100);
  forwardBy(19, MAX_SPEED); // Move so we dont detect the line

  // Do the left side pipes 1-3
  pickupSidePipe(1, SIDE_LEFT);
  returnSidePipe(1, SIDE_RIGHT);
  rotate(180, 100);
  turboFor(PICKUP_TURBO_DURATION, TURBO_SPEED);
  pickupSidePipe(2, SIDE_LEFT);
  returnSidePipe(2, SIDE_RIGHT);
  rotate(180, 100);
  turboFor(PICKUP_TURBO_DURATION, TURBO_SPEED);
  pickupSidePipe(3, SIDE_LEFT);
  returnSidePipe(3, SIDE_RIGHT);
  rotate(180, 100);
  turboFor(PICKUP_TURBO_DURATION, TURBO_SPEED);

  // pickup 4th pipe
  pickupSidePipe(4, SIDE_LEFT);

  driveToNthAdjustedLine(4, SIDE_RIGHT, WALL_PROX_DIST, WALL_FAR_DIST);

  forwardBy(FOURTH_PIPE_RETURN_DIST, MAX_SPEED); // Move back to skids enough to catch the tape after turn
  rotate(-90, 100); // turn towards the prev skid

  driveToNthAdjustedLine(pipeNum, sideSensor, 0, 500); // 0 and 100 should cancel out the ss adjustments
  forwardBy(ROTATION_CORRECTION_DIST, MAX_SPEED); // Adjust for offset

  // rotate to the middle skid and then drop
  rotate(90, 100);
  forwardBy(2.0, MAX_SPEED); // TODO: Fill In
  delay(250);
  setClaw(CLAW_OPEN);
  delay(1000);
  forwardBy(-11.0, MAX_SPEED);

  // rotate for last pipe
  rotate(180, 100);

  // TODO: Finish here
  
  p.PrizmEnd();
}

void pickupSidePipe(int ithPipe, int side) {
  int sideSensor, deliverySideSensor;
  // adjust for claw offset
  if (side == SIDE_LEFT) {
    sideSensor = LEFT_SS;
    deliverySideSensor = RIGHT_SS;
  } else {
    sideSensor = RIGHT_SS;
    deliverySideSensor = LEFT_SS;
  }
  
  // Make sure claw is ready
  setClaw(CLAW_OPEN);
  setArm(ARM_DOWN);
  setTilt(TILT_RETRACTED);
  delay(500);

  // move to the ith line
  driveToLineNumWCorrection(ithPipe, sideSensor); // line adjustment goes here
  // adjust for claw offset
  forwardBy(ROTATION_CORRECTION_DIST, MAX_SPEED);

  // rotate towards pipe based on side
  rotate(side * 90, 100);

  delay(100);
  // Move toward pipe and into grabbing distance
  p.setMotorSpeeds(MAX_SPEED, MAX_SPEED);
  //waitForProximityBelow(FRONT_SS, 15);
  waitForSpace();
  p.setMotorSpeeds(0,0);
  delay(100);

  // TODO: Dial in delays
  setTilt(TILT_EXTENDED);
  delay(900);
  setClaw(CLAW_CLOSED);
  delay(900);
  setTilt(TILT_EXTENDED);
  delay(900);

  // DIAL THIS IN
  forwardBy(-2.5, MAX_SPEED);

  setArm(ARM_UP);
  rotate(side * 90, 100);
}

void returnSidePipe(int pipeNum, int side) {
  int sideSensor = RIGHT_SS;
  if (side == SIDE_LEFT) sideSensor = LEFT_SS;

  // go past pipeNum lines
  // TODO: may need to wait for space or backup
  // p.setMotorSpeeds(MAX_SPEED, MAX_SPEED); waitForSpace();
  forwardBy(-1.5, 100);

  // Travels back across tape lines to the start of the first
  waitForNthAdjustedLine(pipeNum, sideSensor, WALL_PROX_DIST, WALL_FAR_DIST);

  // From the start of the first line, travel back exact distance
  forwardBy(SIDE_TAPE_TO_SKID_DIST, MAX_SPEED);
  delay(250);
  setClaw(CLAW_OPEN);
  delay(1000);
  forwardBy(-11.0, MAX_SPEED);
}

void turboFor(int mills, int speed) {
  int rampupTime = 500;
  int turboTime = mills - 2 * rampupTime;
  if (turboTime < 0) {
    rampupTime = mills / 2;
    turboTime = 0;
  }
  accelerateFor(rampupTime, speed);
  delay(turboTime);
  decelerateFor(rampupTime, speed);
}
