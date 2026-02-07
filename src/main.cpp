
#include "vex.h"

using namespace vex;

//------------------------------------------------------DECLARATIONS------------------------------------------------------------------------

competition Competition;

brain Brain;

motor leftDrive = motor(PORT12, false);
motor rightDrive = motor(PORT11, true);
drivetrain drive = drivetrain(leftDrive, rightDrive, 320, 320, 130, mm, 1);

//bottom intake
motor output1 = motor(PORT5, false);
//top intake
motor output2 = motor(PORT7, false);
//side intake
motor output3 = motor(PORT1, true);

inertial Inertial = inertial(PORT2);
//bump sensor
bumper Bump = bumper(Brain.ThreeWirePort.D);
//descorer
pneumatics digout = pneumatics(Brain.ThreeWirePort.A);

controller Controller = controller();

//integer for controlling which autonomous runs
int auton = 0;

//-------------------------------------------------------HELPER METHODS--------------------------------------------------------------------------

//function for turning with inertial sensor
void wheee(double degrees){
  Inertial.resetHeading();
  drive.stop();

  while(true){
    Brain.Screen.setCursor(0, 0);
    Brain.Screen.print(Inertial.heading());
    drive.turn(right);
    if(Inertial.heading() >= degrees){
      break;
    }
  }

  drive.stop();
}

//if bump sensor pressed, robot stops
void bump(){
  while(true){
    drive.drive(forward);
    if(Bump.pressing()){
      drive.stop();
      break;
    }
  }
}

//pulls in the descorer
void pneumaticIn(){
  digout.set(false);
  Controller.Screen.print("set");
}

//extends the descorer
void pneumaticOut(){
  digout.set(true);
  Controller.Screen.print("set2");
}

//sets the auton integer based on button pressed on screen
void autonPicker(){
  if(Brain.Screen.xPosition() < 240 && Brain.Screen.yPosition() < 120){
    auton = 1;
  } else if(Brain.Screen.yPosition() < 120){
    auton = 2;
  } else if(Brain.Screen.xPosition() > 240){
    auton = 3;
  }
}

//-----------------------------------------------------AUTONOMOUS PROGRAMS-------------------------------------------------------------------
void autonRight(){

  //sets the intake velocity to intake scoring objects and spins intake
  output1.setVelocity(55, pct);
  output2.setVelocity(55, pct);
  output3.setVelocity(55, pct);

  output1.spin(forward);
  output2.spin(forward);
  output3.spin(forward);

  //drive forward from parking zone to intake three objects
  drive.driveFor(forward, 37, inches);
  //turns towards goal
  drive.turnFor(left, 40, degrees);

  //drive towards center bottom goal
  drive.driveFor(forward, 10, inches);

  //scores scoring objects
  output1.setVelocity(-50, pct);
  output2.setVelocity(-50, pct);
  output3.setVelocity(-50, pct);

}

void autonLeft(){

  //sets the intake velocity to intake scoring objects and spins intake
  output1.setVelocity(55, pct);
  output2.setVelocity(55, pct);
  output3.setVelocity(55, pct);

  output1.spin(forward);
  output2.spin(forward);
  output3.spin(forward);

  //drives forward from parking zone to intake three objects
  drive.driveFor(forward, 37, inches);
  drive.turnFor(right, 40, degrees);

  //drives towards center top goal
  drive.driveFor(forward, 12, inches);
  drive.driveFor(reverse, 1, inches);

  //scores scoring objects*
  //*disclaimer: has never actually worked
  output1.setVelocity(50, pct);
  output2.setVelocity(-50, pct);
  output3.setVelocity(50, pct);
}

//drives forward three inches
void autonNothing(){
  drive.driveFor(forward, 3, inches);
}

//--------------------------------------------------------COMPETITION CODE-----------------------------------------------------------------

//so far useless code for creating buttons
void pre_auton(void) {
  /*Inertial.calibrate();

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

  autonRight();
}

void usercontrol(void) {

  digout.set(false);
  
  output1.setVelocity(0, pct);
  output2.setVelocity(0, pct);
  output3.setVelocity(0, pct);

  while (1) {
    //code for the drivetrain
    int32_t rightMotor = (Controller.Axis3.position() - Controller.Axis1.position()) * 0.75;
    int32_t leftMotor = (Controller.Axis3.position() + Controller.Axis1.position()) * 0.75;

    rightDrive.spin(forward, rightMotor, pct);
    leftDrive.spin(forward, leftMotor, pct);

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
      output1.setVelocity(100, pct);
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
    Controller.ButtonA.pressed(pneumaticOut);
    Controller.ButtonA.released(pneumaticIn);
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
