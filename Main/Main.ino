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
  for (int i = 1; i < 4; i++){
    runPart1(i);
  }
  p.PrizmEnd();
}

void runPart1(int line_num){
  setClaw(CLAW_OPEN);
  setArm(ARM_DOWN);
  delay(1500);
  
  p.setMotorSpeeds(MAX_SPEED, MAX_SPEED);
  
  waitForLineNum(line_num);

  forwardBy(ROTATION_CORRECTION_DIST, MAX_SPEED);
  rotate(-90, 100);

  delay(100);
  p.setMotorSpeeds(MAX_SPEED, MAX_SPEED);
  waitForProximityBelow(FRONT_SS, 14);
  p.setMotorSpeeds(0,0);
  delay(100);


  setClaw(CLAW_CLOSED);
  delay(900);
  forwardBy(-2.2, MAX_SPEED);
  setArm(ARM_UP);
  rotate(-90, 100);
  p.setMotorSpeeds(MAX_SPEED, MAX_SPEED);
  waitForProximityBelow(FRONT_SS, 15);
  p.setMotorSpeeds(0, 0);

  setClaw(CLAW_OPEN);
  delay(500);
  forwardBy(-7.0, MAX_SPEED);
  rotate(180, 100);
}

