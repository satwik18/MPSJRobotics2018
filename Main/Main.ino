
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

#define FRONT_SS 6 // Front Super Sonic sensor
#define CLAW_SERVO_NO 2 // Claw servo number
#define ARM_SERVO_NO 1 // Arm servo number

#define MAX_SPEED 150 //  0 to 720 degrees per second (DPS)

#define ARM_DOWN 20
#define ARM_UP 105
#define CLAW_OPEN 170
#define CLAW_CLOSED 75

#define WHEEL_WIDTH 4.0 // the wheel is 4 inches tall
#define WHEEL_C (PI * WHEEL_WIDTH) // the wheel circumference in inches
#define WHEEL_BASE_WIDTH 11.0 // the distance between the wheels in inches
#define WHEEL_BASE_C (PI * WHEEL_BASE_WIDTH) // the circumference of the weel base ininches
#define ROTATION_CORRECTION_DIST 1.5 // The distance that the bot needs to go forward after detecting the line to line claw up for 90% turn in inches

PRIZM p; // Prizm library instance

void setup() {
  p.PrizmBegin();
  p.setMotorInvert(1,1);
  Serial.begin(9600); // For sending debug messages to a connected computer
}


void loop() {
  // this will goto the first line, turn, grab a pipe and come back
  setClaw(CLAW_OPEN);
  setArm(ARM_DOWN);
  
  p.setMotorSpeeds(MAX_SPEED, MAX_SPEED);
  
  waitForLine(50, 1.0);

  forwardBy(ROTATION_CORRECTION_DIST, MAX_SPEED);
  rotate(-90, 100);


  p.setMotorSpeeds(100, 100);
  waitForProximityBelow(FRONT_SS, 6.0); // TODO: the second paramater still needs to be adjusted for distance to the pipe
  p.setMotorSpeeds(0, 0);


  setClaw(CLAW_CLOSED);
  delay(900);
  forwardBy(-3, MAX_SPEED);
  rotate(-90, 100);
  forwardBy(24, MAX_SPEED);
  setArm(ARM_UP);

  delay(50000);
}

