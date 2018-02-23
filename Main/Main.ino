
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

// Constant variables
#define MAX_SPEED 360 //  0 to 720 degrees per second (DPS)

#define WHEEL_WIDTH 4.0 // the wheel is 4 inches tall
#define WHEEL_C (2.0 * PI * WHEEL_WIDTH / 2.0) // the wheel circumference in inches
#define WHEEL_BASE_WIDTH 11.0 // the distance between the wheels in inches
#define WHEEL_BASE_C (2.0 * PI * WHEEL_BASE_WIDTH / 2.0) // the circumference of the weel base ininches


PRIZM p; // Prizm library instance

void setup() {
  p.PrizmBegin();
  p.setMotorInvert(1,1);
  Serial.begin(9600); // For sending debug messages to a connected computer
}


void loop() {
  // Place our high-level algorithm here

  // For now, it will rotate 90 degrees every 5 seconds
  rotate(90, 100);
  delay(5000);
}


