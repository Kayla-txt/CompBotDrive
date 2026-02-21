
#include "vex.h"

using namespace vex;

//------------------------------------------------------DECLARATIONS------------------------------------------------------------------------

competition Competition;

brain Brain;

// left motors from front to back
motor lD1 = motor(PORT12, true);
motor lD2 = motor(PORT14, true);
motor lD3 = motor(PORT1, true);

// right motors from front to back
motor rD1 = motor(PORT6, false);
motor rD2 = motor(PORT11, false);
motor rD3 = motor(PORT20, false);

// create a motorgroup for both left and right sides of the drivetrain,
// as we might expand motor count to three on each side in the future
motor_group leftDrive = motor_group(lD1, lD2, lD3);
motor_group rightDrive = motor_group(rD1, rD2, rD3);

drivetrain drive = drivetrain(leftDrive, rightDrive, 320, 301, 201, mm, 60/36);

//bottom intake
motor output1 = motor(PORT5, false);
//top intake
motor output2 = motor(PORT7, false);
//side intake
motor output3 = motor(PORT4, true);

//descorer
pneumatics digout = pneumatics(Brain.ThreeWirePort.A);

pneumatics matchload = pneumatics(Brain.ThreeWirePort.B);

controller Controller = controller();

//integer for controlling which autonomous runs
bool descorer = false;
bool matchLoader = false;

int currentRight, currentLeft;

//-------------------------------------------------------HELPER METHODS--------------------------------------------------------------------------

//function for turning with inertial sensor
void wheee(double degrees){
  // lost sensors :(
}

//if bump sensor pressed, robot stops
void bump(){
  // lost more sensors :(
}

//pulls in the descorer
void descore(){
  descorer = !descorer;
  digout.set(descorer);
}

//toggles matchloader
void matchLoad(){
  matchLoader = !matchLoader;
  matchload.set(matchLoader);
}

//sets the auton integer based on button pressed on screen
/*void autonPicker(){
  if(Brain.Screen.xPosition() < 240 && Brain.Screen.yPosition() < 120){
    auton = 1;
  } else if(Brain.Screen.yPosition() < 120){
    auton = 2;
  } else if(Brain.Screen.xPosition() > 240){
    auton = 3;
  }
}*/

//-----------------------------------------------------AUTONOMOUS PROGRAMS-------------------------------------------------------------------
void autonRight(){


  //drive forward from parking zone to intake three objects
  drive.driveFor(forward, 20, inches);
  output1.spin(forward);
  output2.spin(forward);
  output3.spin(forward);
  drive.driveFor(forward, 11, inches);
  //turns towards goal
  output1.stop();
  output2.stop();
  output3.stop();

  drive.turnFor(left, 30, degrees);

  //drive towards center bottom goal
  drive.driveFor(forward, 7, inches);

  //scores scoring objects
  output1.setVelocity(-100, pct);
  output2.setVelocity(-100, pct);
  output3.setVelocity(-100, pct);
  output1.spin(forward);
  output2.spin(forward);
  output3.spin(forward);
}

void autonLeft(){

  //sets the intake velocity to intake scoring objects and spins intake
  drive.driveFor(forward, 20, inches);
  output1.spin(forward);
  output2.spin(forward);
  output3.spin(forward);
  drive.driveFor(forward, 11, inches);
  //turns towards goal
  output1.stop();
  output2.stop();
  output3.stop();
  
  drive.turnFor(right, 30, degrees);

  //drive towards center bottom goal
  drive.driveFor(forward, 7, inches);

  //scores scoring objects*
  //*disclaimer: has never actually worked
  output1.setVelocity(100, pct);
  output2.setVelocity(-100, pct);
  output3.setVelocity(100, pct);
  output1.spin(forward);
  output2.spin(forward);
  output3.spin(forward);
}

//drives forward three inches
void autonNothing() {
  drive.driveFor(forward, 3, inches);
}

void autonSkills(){
  //code for skills goes here
}

void accelerator(int RightMotor, int LeftMotor) {
  if (abs(RightMotor - currentRight) < 1){
    currentRight = RightMotor;
  } else if (RightMotor > currentRight) {
    currentRight += 0.5;
  } else if (RightMotor < currentRight) {
    currentRight -= 0.5;
  }
  if (abs(LeftMotor - currentLeft) < 1){
    currentLeft = LeftMotor;
  } else if (LeftMotor > currentLeft) {
    currentLeft += 0.5;
  } else if (LeftMotor < currentLeft) {
    currentLeft -= 0.5;
  }
  
}

//--------------------------------------------------------COMPETITION CODE-----------------------------------------------------------------

//so far useless code for creating buttons
void pre_auton(void) {
  /*
  Inertial.calibrate();

  Brain.Screen.setFillColor(red);
  Brain.Screen.drawRectangle(0, 0, 240, 120);
  Brain.Screen.setFillColor(green);
  Brain.Screen.drawRectangle(240, 0, 240, 120);
  Brain.Screen.setFillColor(blue);
  Brain.Screen.drawRectangle(0, 120, 480, 120);

  Brain.Screen.setFillColor(black);
  Brain.Screen.printAt(10, 10, "right match auton");
  Brain.Screen.printAt(210, 10, "left match auton");
  Brain.Screen.printAt(10, 100, "skills auton");

  Brain.Screen.pressed(autonPicker);*/
}

void autonomous(void) {
  /*if(auton == 1){
    autonRight();
  } else if(auton == 2){
    autonLeft();
  } else {
    autonNothing();
  }*/
  output1.setVelocity(27.5, pct);
  output2.setVelocity(27.5, pct);
  output3.setVelocity(27.5, pct);
  autonNothing();
}

void usercontrol(void) {

  digout.set(false);
  
  output1.setVelocity(0, pct);
  output2.setVelocity(0, pct);
  output3.setVelocity(0, pct);
  currentLeft, currentRight = 0, 0;
  int8_t counter = 0;
  while (1) {
    //code for the drivetrain
    int32_t rightMotor = (Controller.Axis3.position() - (Controller.Axis1.position() * 0.8)) * 0.8;
    int32_t leftMotor = (Controller.Axis3.position() + (Controller.Axis1.position() * 0.8)) * 0.8;
    

    accelerator(rightMotor, leftMotor);
    counter++;
    output1.spin(forward);
    output2.spin(forward);
    output3.spin(forward);

    //intakes objects or scores in top goal
    if(Controller.ButtonR2.pressing()){
      output1.setVelocity(100, pct);
      output2.setVelocity(100, pct);
      output3.setVelocity(100, pct);
    //scores in middle goal
    } else if(Controller.ButtonR1.pressing()){
      output1.setVelocity(100   , pct);
      output2.setVelocity(-100, pct);
      output3.setVelocity(100, pct);
    //scores in bottom goal
    } else if(Controller.ButtonL2.pressing()){
      output1.setVelocity(-100, pct);
      output2.setVelocity(-100, pct);
      output3.setVelocity(-100, pct);
    } else if(Controller.ButtonL1.pressing()){
      output1.setVelocity(50, pct);
      output2.setVelocity(0, pct);
      output3.setVelocity(-50, pct);
    } else {
      output1.setVelocity(0, pct);
      output2.setVelocity(0, pct);
      output3.setVelocity(0, pct);
    }

    //penumatic controllers
    Controller.ButtonA.pressed(descore);
    Controller.ButtonX.pressed(matchLoad);

  }

  wait(20, msec); 
}

int main() {
  // Set up callbacks for autonomous and driver control periods.
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  // Run the pre-autonomous function.
  pre_auton();

  // Prevent main from exiting with an infinite loop.
  while (true) {
    wait(100, msec);
  }
}
