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

#define FRONT_SS 5 // Front Super Sonic sensor
#define RIGHT_SS 16 // Right Super Sonic sensor
#define LEFT_SS 15 // Left Super Sonic sensor
#define CLAW_SERVO_NO 2 // Claw servo number
#define ARM_SERVO_NO 1 // Arm servo number
#define CLAW_SERVO_SPEED 40 // In percentage where 100% is max speed

#define MAX_SPEED 150 //  0 to 720 degrees per second (DPS)

#define ARM_DOWN 20 // The arm's up position
#define ARM_UP 105 // The arm's down position
#define CLAW_OPEN 150 // The claw's open position
#define CLAW_CLOSED 70 // The claw's closed position

#define WHEEL_WIDTH 4.0 // the wheel is 4 inches tall
#define WHEEL_C (PI * WHEEL_WIDTH) // the wheel circumference in inches
#define WHEEL_BASE_WIDTH 11.0 // the distance between the wheels in inches
#define WHEEL_BASE_C (PI * WHEEL_BASE_WIDTH) // the circumference of the weel base ininches
#define ROTATION_CORRECTION_DIST_RIGHT 0.955 // The distance that the bot needs to go forward after detecting the line to line claw up for 90% turn in inches
#define ROTATION_CORRECTION_DIST_LEFT 5.0 // Same as above but for the left // was 5.375

#define SIDE_RIGHT -1 // a multiplyer to adjust angles for the right side
#define SIDE_LEFT 1 // a multiplyer to ajust angles for the left side

PRIZM p; // Prizm library instance

void setup() {
  p.PrizmBegin();
  p.setMotorInvert(1,1);
  Serial.begin(9600); // For sending debug messages to a connected computer
  p.setServoSpeed(CLAW_SERVO_NO, CLAW_SERVO_SPEED);
  delay(100);
}

void loop() {
  /*
  // open claw
  setClaw(CLAW_OPEN);
  setArm(ARM_DOWN);
  delay(500);

  // lineup for first run
  forwardBy(4.25, MAX_SPEED);
  rotate(89.0, 100);
  
  // pickup pipes 1-3 on the right side
  pickupSidePipe(1, SIDE_RIGHT);
  returnSidePipe(1);
  rotate(179, 100);
  pickupSidePipe(2, SIDE_RIGHT);
  returnSidePipe(2);
  rotate(180, 100);
  pickupSidePipe(3, SIDE_RIGHT);
  returnSidePipe(3);

  // Pickup 4th pipe
  rotate(179, 100);
  pickupSidePipe(4, SIDE_RIGHT);


  forwardBy(58, MAX_SPEED); // Move back to skids enough to catch the tape after turn
  rotate(90, 100); // turn towards the next skid

  // move to next line
  forwardBy(2.5, MAX_SPEED); // Make sure its over the black tape
  p.setMotorSpeeds(MAX_SPEED, MAX_SPEED);
  waitForLineNum(1); // the led will likely get detected on the first line so 2 lines should be detected
  forwardBy(ROTATION_CORRECTION_DIST_RIGHT + 1.7, MAX_SPEED); // adjust for claw offset
  
  // rotate to the middle skid and then drop
  rotate(-90, 100);
  returnPipe();

  */

  // rotate to the third skid
  rotate(90, 100);
  delay(100);
  forwardBy(2.5, MAX_SPEED); // make sure does not read tape
  
  // Move to the third skid line
  p.setMotorSpeeds(MAX_SPEED, MAX_SPEED);
  waitForLineNum(1); // the led will likely get detected on the first line so 2 lines should be detected
  forwardBy(ROTATION_CORRECTION_DIST_RIGHT + 3.0, MAX_SPEED); // Adjust for offset

  // prepare for first run on the left side
  rotate(89, 100);
  forwardBy(19, MAX_SPEED); // Move so we dont detect the line

  // Do the left side pipes 1-3
  pickupSidePipe(1, SIDE_LEFT);
  returnPipe();
  rotate(180, 100);
  pickupSidePipe(2, SIDE_LEFT);
  returnPipe();
  rotate(180, 100);
  pickupSidePipe(3, SIDE_LEFT);
  returnPipe();

  // pickup 4th pipe
  rotate(180, 100);
  pickupSidePipe(4, SIDE_LEFT);

  forwardBy(58, MAX_SPEED); // Move back to skids enough to catch the tape after turn
  rotate(-90, 100); // turn towards the prev skid

  // rotate to the middle skid and then drop
  rotate(90, 100);
  returnPipe();

  // rotate for last pipe
  rotate(180, 100);

  // TODO finish here
  
  p.PrizmEnd();
}

void pickupSidePipe(int ithPipe, double side) {
  double correctionDist;
  int sideSensor, deliverySideSensor;
  // adjust for claw offset
  if (side == SIDE_LEFT) {
    correctionDist = ROTATION_CORRECTION_DIST_LEFT;
    sideSensor = LEFT_SS;
    deliverySideSensor = RIGHT_SS;
  } else {
    correctionDist = ROTATION_CORRECTION_DIST_RIGHT;
    sideSensor = RIGHT_SS;
    deliverySideSensor = LEFT_SS;
  }
  
  // Make sure claw is ready
  setClaw(CLAW_OPEN);
  setArm(ARM_DOWN);
  delay(1500);

  // move to the ith line
  p.setMotorSpeeds(MAX_SPEED, MAX_SPEED);
  waitForLineNumWCorrection(ithPipe, sideSensor);
  // adjust for claw offset
  forwardBy(correctionDist, MAX_SPEED);

  // rotate towards pipe based on side
  rotate(side * 90, 100);

  delay(100);
  // Move toward pipe and into grabbing distance
  p.setMotorSpeeds(MAX_SPEED, MAX_SPEED);
  waitForProximityBelow(FRONT_SS, 15);
//  waitForProximityBelowWCorrection(FRONT_SS, 14, deliverySideSensor);
  p.setMotorSpeeds(0,0);
  delay(100);


  setClaw(CLAW_CLOSED);
  delay(900);

  forwardBy(-2.5, MAX_SPEED);

  setArm(ARM_UP);
  rotate(side * 91, 100);
}

void returnPipe() {
  // forward until at dropping distance
  p.setMotorSpeeds(MAX_SPEED, MAX_SPEED);
  waitForProximityBelow(FRONT_SS, 14);
  p.setMotorSpeeds(0, 0);

  setClaw(CLAW_OPEN);
  delay(1000);
  forwardBy(-11.0, MAX_SPEED);
}

void returnSidePipe(int pipeNum) {
  const double baseDist = 52.75;
  const double lineDist = 5.0;

  forwardBy(baseDist + (pipeNum - 1) * lineDist, MAX_SPEED);
  setClaw(CLAW_OPEN);
  delay(1000);
  forwardBy(-11.0, MAX_SPEED);
}
