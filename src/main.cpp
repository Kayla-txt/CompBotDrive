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
bumper Bump = bumper(Brain.ThreeWirePort.D);
pneumatics digout = pneumatics(Brain.ThreeWirePort.A);


controller Controller = controller();

int auton = 0;

//-------------------------------------------------------HELPER METHODS--------------------------------------------------------------------------

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

void bump(){
  while(true){
    drive.drive(forward);
    if(Bump.pressing()){
      drive.stop();
      break;
    }
  }
}

void pneumaticIn(){
  digout.set(false);
  Controller.Screen.print("set");
}

void pneumaticOut(){
  digout.set(true);
  Controller.Screen.print("set2");
}

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

  output1.setVelocity(55, pct);
  output2.setVelocity(55, pct);
  output3.setVelocity(55, pct);

  output1.spin(forward);
  output2.spin(forward);
  output3.spin(forward);

  drive.driveFor(forward, 37, inches);
  drive.turnFor(left, 40, degrees);

  drive.driveFor(forward, 10, inches);

  output1.setVelocity(-50, pct);
  output2.setVelocity(-50, pct);
  output3.setVelocity(-50, pct);

}

void autonLeft(){

  output1.setVelocity(55, pct);
  output2.setVelocity(55, pct);
  output3.setVelocity(55, pct);

  output1.spin(forward);
  output2.spin(forward);
  output3.spin(forward);

  drive.driveFor(forward, 37, inches);
  drive.turnFor(right, 40, degrees);

  drive.driveFor(forward, 12, inches);
  drive.driveFor(reverse, 1, inches);

  output1.setVelocity(50, pct);
  output2.setVelocity(-50, pct);
  output3.setVelocity(50, pct);
}

void autonNothing(){
  drive.driveFor(forward, 3, inches);
}

//--------------------------------------------------------COMPETITION CODE-----------------------------------------------------------------

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
    int32_t rightMotor = (Controller.Axis3.position() - Controller.Axis1.position()) * 0.75;
    int32_t leftMotor = (Controller.Axis3.position() + Controller.Axis1.position()) * 0.75;

    rightDrive.spin(forward, rightMotor, pct);
    leftDrive.spin(forward, leftMotor, pct);

    output1.spin(forward);
    output2.spin(forward);
    output3.spin(forward);

    if(Controller.ButtonR2.pressing()){
      output1.setVelocity(100, pct);
      output2.setVelocity(100, pct);
      output3.setVelocity(100, pct);
    } else if(Controller.ButtonR1.pressing()){
      output1.setVelocity(100, pct);
      output2.setVelocity(-100, pct);
      output3.setVelocity(100, pct);
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

    Controller.ButtonA.pressed(pneumaticIn);
    Controller.ButtonB.pressed(pneumaticOut);
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
