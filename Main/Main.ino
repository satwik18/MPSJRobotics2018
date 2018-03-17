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

#define MAX_SPEED 150 //  0 to 720 degrees per second (DPS)

#define ARM_DOWN 20
#define ARM_UP 105
#define CLAW_OPEN 170
#define CLAW_CLOSED 73

#define WHEEL_WIDTH 4.0 // the wheel is 4 inches tall
#define WHEEL_C (PI * WHEEL_WIDTH) // the wheel circumference in inches
#define WHEEL_BASE_WIDTH 11.0 // the distance between the wheels in inches
#define WHEEL_BASE_C (PI * WHEEL_BASE_WIDTH) // the circumference of the weel base ininches
#define ROTATION_CORRECTION_DIST_RIGHT 1.5 // The distance that the bot needs to go forward after detecting the line to line claw up for 90% turn in inches
#define ROTATION_CORRECTION_DIST_LEFT 5.75 // Same as above but for the left // was 5.375

#define SIDE_RIGHT -1 // a multiplyer to adjust angles for the right side
#define SIDE_LEFT 1 // a multiplyer to ajust angles for the left side

PRIZM p; // Prizm library instance

void setup() {
  p.PrizmBegin();
  p.setMotorInvert(1,1);
  Serial.begin(9600); // For sending debug messages to a connected computer
  //p.setServoPositions(90, 90, 0, 0, 0, 0);
//  delay(1500);
}

void loop() {


  forwardBy(4.25, MAX_SPEED);
  rotate(90, 100);
  
  pickupSidePipe(1, SIDE_RIGHT); // * 1.001);
  returnPipe();
  rotate(179, 100);
  pickupSidePipe(2, SIDE_RIGHT);// * 0.9999);
  returnPipe();
  rotate(179, 100);
  pickupSidePipe(3, SIDE_RIGHT);
  returnPipe();
  rotate(179, 100);
  pickupSidePipe(4, SIDE_RIGHT);

  forwardBy(58, MAX_SPEED);
  
  rotate(90, 100);
  forwardBy(2.5, MAX_SPEED);
  p.setMotorSpeeds(MAX_SPEED, MAX_SPEED);
  waitForLineNum(1); // the led will likely get detected on the first line so 2 lines should be detected
  // adjust for claw offset
  forwardBy(ROTATION_CORRECTION_DIST_RIGHT + 1.7, MAX_SPEED);
  rotate(-90, 100);
  returnPipe();


  
  rotate(90, 100);
  delay(100);
  forwardBy(2.5, MAX_SPEED);
  
  p.setMotorSpeeds(MAX_SPEED, MAX_SPEED);
  waitForLineNum(1); // the led will likely get detected on the first line so 2 lines should be detected
  // adjust for claw offset
  forwardBy(ROTATION_CORRECTION_DIST_RIGHT + 3.0, MAX_SPEED);
  rotate(89, 100);

  forwardBy(19, MAX_SPEED); // Move so we dont detect the line

  // Do the left side
  pickupSidePipe(1, SIDE_LEFT); // * 1.001);
  returnPipe();
  rotate(180, 100);
  pickupSidePipe(2, SIDE_LEFT);// * 0.9999);
  returnPipe();
  rotate(180, 100);
  pickupSidePipe(3, SIDE_LEFT);
  returnPipe();
  pickupSidePipe(4, SIDE_LEFT);
  
  p.PrizmEnd();
}

void pickupSidePipe(int ithPipe, double side) {
  double correctionDist;
  int sideSensor;
  // adjust for claw offset
  if (side == SIDE_LEFT) {
    correctionDist = ROTATION_CORRECTION_DIST_LEFT;
    sideSensor = LEFT_SS;
  } else {
    correctionDist = ROTATION_CORRECTION_DIST_RIGHT;
    sideSensor = RIGHT_SS;
  }
  
  // Make sure claw is ready
  setClaw(CLAW_OPEN);
  setArm(ARM_DOWN);
  delay(1500);


  // move to the ith line
  p.setMotorSpeeds(MAX_SPEED, MAX_SPEED);
  waitForLineNumWCorrection(ithPipe, side);
  // adjust for claw offset
  forwardBy(correctionDist, MAX_SPEED);
  

  // rotate towards pipe based on side
  rotate(side * 90, 100);

  delay(100);
  // Move toward pipe and into grabbing distance
  p.setMotorSpeeds(MAX_SPEED, MAX_SPEED);
  waitForProximityBelow(FRONT_SS, 14); // original 14
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

void runPart1(int line_num){
  setClaw(CLAW_OPEN);
  setArm(ARM_DOWN);
  delay(1500);
  
  p.setMotorSpeeds(MAX_SPEED, MAX_SPEED);
  
  waitForLineNum(line_num);

  forwardBy(ROTATION_CORRECTION_DIST_RIGHT, MAX_SPEED);
  rotate(-90, 100);

  delay(100);
  //forwardBy(2.5, MAX_SPEED);
  p.setMotorSpeeds(MAX_SPEED, MAX_SPEED);
  waitForProximityBelow(FRONT_SS, 14);
  p.setMotorSpeeds(0,0);
  delay(100);


  setClaw(CLAW_CLOSED);
  delay(900);
  forwardBy(-2.2, MAX_SPEED);
  setArm(ARM_UP);
  rotate(-90, 100);
  //forwardBy(53, MAX_SPEED);
  p.setMotorSpeeds(MAX_SPEED, MAX_SPEED);
  waitForProximityBelow(FRONT_SS, 17);
  p.setMotorSpeeds(0, 0);

  setClaw(CLAW_OPEN);
  delay(500);
  forwardBy(-7.0, MAX_SPEED);
  rotate(180, 100);
}

