#include "gantry.h"
#include "../includes/EV3Servo-lib-UW.c"
#include "../latch/latch.c"

bool configureSwitchs()
{
	SensorType[S1] = sensorI2CCustom9V;
	wait1Msec(100);
  SensorType[S2] = sensorEV3_Touch;
  SensorType[S3] = sensorEV3_Touch;
	return true;
}

/*
bool readSwitch(int touchSensor)
{
    if (touchSensor == TOUCH_Y2)
    {
        return SensorValue[touchSensor];
    }
    else
    {
    		displayString(5, "Testing");
        return readMuxSensor(touchSensor);
    }
}
*/

bool limitY1()
{
    return SensorValue[S3];
}
bool limitY2()
{
    return SensorValue[S3];
}

bool limitX1()
{
    return SensorValue[S2];
}
bool limitX2()
{
    return readMuxSensor(msensor_S4_2);
}

int calibrateAxis(bool yAxis)
{
    int currMotor = yAxis?motorA:motorB;

    motor[currMotor] = STOP;
    nMotorEncoder[currMotor] = 0;

    motor[currMotor] = HALF_POW;

    if (yAxis)
    {
        while(!(limitY1())
        {}
    }
    else
    {
        while(!(limitX1())
        {}
    }
    motor[currMotor] = 0;

    return nMotorEncoder[currMotor];
}

void calibrate()
{
		latch(false);
    BOARD_PARAMS.xAxisClicks = calibrateAxis(true);
    BOARD_PARAMS.yAxisClicks = calibrateAxis(false);
		latch(true);
}

void moveDist(float dist_cm, bool yAxis)
{
    const int STEP_CM = yAxis?BOARD_PARAMS.yAxisConv:BOARD_PARAMS.xAxisConv;

	nMotorEncoder[motorA]=STOP;
	nMotorEncoder[motorB]=STOP;

	if (dist_cm > 0)
	{
		if(yAxis)
			motor[motorA] = MAX_POW;
		else
			motor[motorB] = MAX_POW;
	}
	else{
		if(yAxis)
			motor[motorA] = -MAX_POW;
		else
			motor[motorB] = -MAX_POW;
	}

	if(yAxis)
	{
		while(abs(nMotorEncoder[motorA]) < abs(dist_cm * STEP_CM))
		{}
	}
	else
	{
		while(abs(nMotorEncoder[motorB]) < abs(dist_cm * STEP_CM))
		{}
	}

	motor[motorA] = 0;
	motor[motorB] = 0;

	return;
}

void moveSquare(Point_t *currentPoint, Point_t const *goalPoint)
{
    float diffX = goalPoint->xCord - currentPoint->xCord;
    float diffY = goalPoint->yCord - currentPoint->yCord;

    diffX *= SQUARE_DIM;
    diffY *= SQUARE_DIM;

		wait10Msec(100);
    moveDist(diffX, false);
    moveDist(diffY, true);

    currentPoint->xCord = goalPoint->xCord;
    currentPoint->yCord = goalPoint->yCord;

    return;
}
