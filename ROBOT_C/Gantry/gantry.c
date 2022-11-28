#include "gantry.h"
#include "../includes/EV3Servo-lib-UW.c"
#include "../latch/latch.c"
#include "../comms/comms.h"

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

bool limitX1()
{
    return SensorValue[S2];
}

void executeMove(Point_t *currentPoint, Point_t *initial, Point_t *final, int state)
{
    const int CENTER = 4;
    if (state == STATE_CASTLE)
    {
        Point_t endKing;
        Point_t endRook;
        endKing.yCord = initial->yCord;
        endRook.yCord = final->yCord;
        if(final->xCord < CENTER)
        {
            endKing.xCord = 1;
            endRook.xCord = 2;
	      }
	      else
	      {
	          endKing.xCord = 5;
	          endRook.xCord = 4;
	      }

	      Point_t initialCopy;
	      Point_t finalCopy;

	      initialCopy.xCord = initial->xCord;
	      initialCopy.yCord = initial->yCord;

	      finalCopy.xCord = final->xCord;
	      finalCopy.yCord = final->yCord;

	      executeMove(currentPoint, initialCopy, endKing, STATE_MOVE);
	      executeMove(currentPoint, finalCopy, endRook, STATE_MOVE);
	  }
		else
		{
	    latch(0);
	    moveSquare (currentPoint, initial);
	    latch(1);
	    float ini_move_x_factor = initial->xCord < CENTER? 0.5:-0.5;
	    float ini_move_y_factor = initial->yCord < CENTER? 0.5:-0.5;
	    initial->xCord += ini_move_x_factor;
	    initial->yCord += ini_move_y_factor;
	    moveSquare(currentPoint, initial);
	    float fin_move_x_factor = final->xCord < CENTER? 0.5:-0.5;
	    float fin_move_y_factor = final->yCord < CENTER? 0.5:-0.5;
	    final->xCord += fin_move_x_factor;
	    final->yCord += fin_move_y_factor;
	    moveSquare(currentPoint, final);
	    final->xCord -= fin_move_x_factor;
	    final->yCord -= fin_move_y_factor;
	    moveSquare(currentPoint, final);
	    latch(0);
  	}

  return;
}

void eject(Point_t *currentPoint, Point_t *toEject)
{
	latch(false);
	moveSquare(currentPoint, toEject);
	latch(true);

	Point_t edge;
	edge.xCord = currentPoint->xCord + 0.5;
	edge.yCord = currentPoint->yCord +0.5;

	moveSquare(currentPoint, edge);

	Point_t finalEject;
	finalEject.xCord = -0.6
	finalEject.yCord = currentPoint->yCord;
	moveSquare(currentPoint, finalEject);

	finalEject.xCord = 0;
	moveSquare(currentPoint, finalEject);
	wait10Msec(50);
	latch(false);

	return;
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
    calibrateAxis(true);
    moveDist(-0.6,true);
    calibrateAxis(false);
    moveDist(-3.0,false);
}

void moveDist(float dist_cm, bool yAxis)
{
    const int STEP_CM = yAxis?Y_AXIS_CONV:X_AXIS_CONV;

	nMotorEncoder[motorA]=STOP;
	nMotorEncoder[motorB]=STOP;

	if (dist_cm > 0)
	{
		if(yAxis)
			motor[motorA] = MAX_POW * 0.75;
		else
			motor[motorB] = MAX_POW * 0.75;
	}
	else{
		if(yAxis)
			motor[motorA] = -MAX_POW * 0.75;
		else
			motor[motorB] = -MAX_POW * 0.75;
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

		wait10Msec(10);
    moveDist(diffY, true);
    moveDist(diffX, false);

    currentPoint->xCord = goalPoint->xCord;
    currentPoint->yCord = goalPoint->yCord;

    return;
}
