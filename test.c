/**********************************************************************
Filename: ServoDemoEV3.c
Date: November 3, 2017
File Version: 1.1

Demonstrate the operation of the Tetrix Prime motors (continous and
standard servo motors, including the gripper).

The standard servo can be used to position for positioning (from -90 to 90
degrees).

The gripper can be set for positions between 0 and 70.
  CAUTION:  The value to close the gripper is close to or at zero.  It
  differs based on the slight variations in gripper assembly.  If you
  hear the motor running while in the closed position, increase the
  commanded value slightly.

The continuous servos can be set to values between -100 and +100.
There is a deadband for the continous servos, which can be determined
using the program.  To determine the deadband offsets, run the program and
gradually increase the reverse speed by pushing the left button to
subtract from the speed setting.  Record the value just before the
motors start to move.  Similarly, use the right button to increase
speed and determine the value for the positive direction.  For example,
if the servo is stationary at -35 and moving at -36, the reverse offset is -35.
These negative and positive values can be sent to the setServoSpeed function.

History
Ver  Date       Comment
1.0  Nov  3/17  port to use EV3
1.0  Jun  6/15  original release

**********************************************************************/
#include "/includes/EV3Servo-lib-UW.c"

// Motor locations on interface board
const int SV_CSERVO1 = 1;
const int SV_CSERVO2 = 2;
const int SV_STANDARD = 3;
const int SV_GRIPPER = 4;

// Demo types
const int CONTINUOUS = buttonUp;
const int GRIPPER = buttonEnter;
const int STANDARD = buttonDown;

int getButton()
{
    while (!getButtonPress(buttonAny))
    {}

    int buttonNum = 0;
    for (int buttonTest = 1; buttonTest <= (int)buttonLeft && buttonNum == 0; buttonTest++)
        if (getButtonPress(buttonTest))
            buttonNum = buttonTest;

    while (getButtonPress(buttonAny))
    {}

    return buttonNum;
}

void setDemoMotor(int motorChoice, int motorSetting)
{
		if (motorChoice == CONTINUOUS)
		{
			// set servo motors with desired speed
			setServoSpeed(S1,SV_CSERVO1, motorSetting);
			setServoSpeed(S1,SV_CSERVO2, motorSetting);
		}
		else if (motorChoice == STANDARD)
		{
			// set servo motor with desired position
			setServoPosition(S1,SV_STANDARD,motorSetting);
		}
		else
		{
			// set gripper with desired position
			setGripperPosition(S1,SV_GRIPPER,motorSetting);
		}
}

task main()
{
	// configure servo controller port
	SensorType[S1] = sensorI2CCustom9V;

	// display information on the screen
	displayBigTextLine(0,"EV3 S1: control");
	displayBigTextLine(2,"SV%d: C servo 1",SV_CSERVO1);
	displayBigTextLine(4,"SV%d: C servo 2",SV_CSERVO2);
	displayBigTextLine(6,"SV%d: S servo",SV_STANDARD);
	displayBigTextLine(8,"SV%d: Gripper",SV_GRIPPER);
	displayBigTextLine(10,"SV8: Battery");
	displayBigTextLine(14,"Confirm - button");
	getButton();

	// select demo
	eraseDisplay();
	displayBigTextLine(0,"Push button to");
	displayBigTextLine(2,"select demo");
	displayBigTextLine(6,"^ : continuous");
	displayBigTextLine(8,"enter : gripper");
	displayBigTextLine(10,"v : standard");
	int choice = getButton();

	// menu
	eraseDisplay();
	if (choice == CONTINUOUS)
		displayBigTextLine(0,"Continuous Servo");
	else if (choice == STANDARD)
		displayBigTextLine(0,"Standard Servo");
	else
		displayBigTextLine(0,"Gripper");
	displayBigTextLine(4,"Enter to end");
	displayBigTextLine(6,"^ increase");
	displayBigTextLine(8,"v decrease");

	// initial setting for motors
	int motorSetting = 0;
	if (choice == GRIPPER)
		motorSetting = 40;

	// keep looping until the orange button is pressed
	while (!getButtonPress(buttonEnter))
	{
		// adjust setting up or down
		if (getButtonPress(buttonUp))
		{
			motorSetting += 1;
			setDemoMotor(choice,motorSetting);
		}
		else if (getButtonPress(buttonDown))
		{
			motorSetting -= 1;
			setDemoMotor(choice,motorSetting);
		}
		displayBigTextLine(12, "setting: %d      ", motorSetting);
		wait1Msec(300);
	}

	eraseDisplay();
	displayBigTextLine(0,"Done");

	// move motors to stop/neutral positions
	setServoPosition(S1,SV_STANDARD,0);
	resetGripper(S1,SV_GRIPPER);
	wait1Msec(10000);
}
/**********************************************************************
Copyright(c) 2014-2017 D. Lau, M. Stachowsky, and C.C.W. Hulls, P.Eng.
Students, staff, and faculty members at the University of Waterloo
are granted a non-exclusive right to copy, modify, or use this
software for non-commercial teaching, learning, and research purposes
provided the author(s) are acknowledged except for as noted below.

GENE 121 and ME 101 students can copy, modify, or use this software
in their current term course assignments without acknowledging
the source.
**********************************************************************/
