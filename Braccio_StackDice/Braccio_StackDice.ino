#include "BraccioV2.h"
Braccio arm;

//Set these values from the min/max gripper joint values below.
#define GRIPPER_CLOSED 70
#define GRIPPER_OPENED 15

const int m1_p[4] = {170, 152, 133, 152};
const int m2_p[4] = {161, 152, 149, 109};
const int m3_p[4] = {163, 195, 197, 162};
const int m4_p[4] = {45, 34, 37, 190};

void goto_pickup_position(int index, int /*height*/) {
   arm.setOneAbsolute(BASE_ROT, m1_p[index]);
   arm.safeDelay(3000, 20);

   arm.setOneAbsolute(ELBOW,    m3_p[index]);
   arm.setOneAbsolute(WRIST,    m4_p[index]);
   arm.setOneAbsolute(WRIST_ROT, 0);

   arm.safeDelay(1000, 10);

   arm.setOneAbsolute(SHOULDER, m2_p[index]);
   arm.safeDelay(3000, 20);
}

void goto_safe_height() {
   arm.setOneAbsolute(SHOULDER, 100);
   arm.setOneAbsolute(ELBOW,    140);
   arm.setOneAbsolute(WRIST,    50);
   arm.setOneAbsolute(WRIST_ROT, 0);

   arm.safeDelay(3000, 40);
}

void setup() {

  //Update these lines with the calibration code outputted by the calibration program.
  arm.setJointCenter(WRIST_ROT, 90);
  arm.setJointCenter(WRIST, 90);
  arm.setJointCenter(ELBOW, 90);
  arm.setJointCenter(SHOULDER, 90);
  arm.setJointCenter(BASE_ROT, 90);
  arm.setJointCenter(GRIPPER, GRIPPER_OPENED);//Rough center of gripper, default opening position

  arm.setJointMax(GRIPPER, 80);
  arm.setJointMin(GRIPPER, 10);

  //Start to default position.
  arm.begin(true);// Start to default vertical position.
  //This method moves the arm to the values specified by setJointCenter
  //and by default will make the arm be roughly straight up.

  //NOTE: The begin method takes approximately 8 seconds to start, due to the time required
  //to initialize the power circuitry.
  Serial.println("Initialization Complete");
}

void end() {
  arm.setAllAbsolute(90, 90, 90, 90, 90, 0);
  arm.safeDelay(7000, 20);
  digitalWrite(SOFT_START_PIN, LOW);
  for (;;);
}

void loop() {
  goto_safe_height();
 
  goto_pickup_position(0, 0);
  closeGripper();

  goto_safe_height();
 
  goto_pickup_position(1, 0);
  openGripper();

  closeGripper();
  goto_safe_height();
 
  goto_pickup_position(2, 0);
  openGripper();

  closeGripper();
  goto_safe_height();

  goto_pickup_position(3, 0);
  openGripper();

  end();
}

void openGripper(){
  //Set the gripper position to open
  arm.setOneAbsolute(GRIPPER, GRIPPER_OPENED);
  arm.safeDelay(1000, 10);
}

void closeGripper(){
  //Set the gripper position to closed
  arm.setOneAbsolute(GRIPPER, 70);
  arm.safeDelay(2000, 10);
}
