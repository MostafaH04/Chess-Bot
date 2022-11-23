#include "/includes/EV3Servo-lib-UW.c"
#include "JoystickDriver.c"
#include "UW_sensorMux.c"

const float BOARD_LEN = 30;
const int BOARD_W = 8;
const int BOARD_L = 8;

const float DIST_Y_LIMIT = 5;
const float DIST_X_LIMIT = 5;

const int SERVO_PORT3 = 3;

const tSensors TOUCH_PORT1 = S3;
const tSensors TOUCH_PORT2 = S4;

bool config();

// functions for limit switchs
bool limitInteruptY(); // returns if limit is hit
void limitYStart(); // start up

bool limitInteruptX();
void limitXStart();

void latch(bool latch);
void moveSquare(int *current, int goal, bool xAxis);
void moveDist(float dist, bool xAxis);

void startUp();

int comms(int *toMoveX, int *toMoveY);

// to make:
//void eject();
//void chessTimer();

task main()
{
	int currentX = 1, currentY = 0;
	int move_Val = 0;
	int toMoveX = 0, toMoveY = 0;

	config();
	while(!getButtonPress(buttonEnter))
	{
		do
		{
			move_Val = comms(&toMoveX, &toMoveY);
		} while(move_Val == -1);
		if (move_Val == 9)
		{
			latch(1);
		}
		else if (move_Val == 10)
		{
			latch(0);
		}
		else{
			displayString(5,"test: %d", toMoveX);
			displayString(7,"test: %d", toMoveY);
			moveSquare(&currentY, toMoveY, false);
			moveSquare(&currentX, toMoveX, true);
			currentX = toMoveX;
			currentY = toMoveY;
		}
		move_Val = -1;
	}

}


bool config()
{
	SensorType[S1] = sensorI2CCustom9V;
	/*
	SensorType[S1] = sensorEV3_GenericI2C;
	wait1Msec(100);

	if(!initSensorMux(msensor_S4_1, touchStateBump))
		return false;
	if(!initSensorMux(msensor_S4_2, touchStateBump))
		return false;
	if(!initSensorMux(msensor_S4_3, touchStateBump))
		return false;
	*/
	SensorType[S3] = sensorEV3_Touch;

	return true;
}


bool limitInteruptY()
{
	bool output = readMuxSensor(msensor_S4_2) || readMuxSensor(msensor_S4_1);
	if (output)
	{
		motor[motorA] = 0;
		moveDist(-DIST_Y_LIMIT, false);
		motor[motorA] = 0;
	}
	return output;
}

bool limitInteruptX()
{
	bool output = readMuxSensor(msensor_S4_2) || readMuxSensor(msensor_S4_1);
	if (output)
	{
		motor[motorA] = 0;
		moveDist(-DIST_Y_LIMIT, true);
		motor[motorA] = 0;
	}
	return output;
}


void moveDist(float dist, bool xAxis)
{
	const int STEP_CM = 100/1.11;
	eraseDisplay();
	displayString(5,"Test: %d", dist);
	nMotorEncoder[motorA]=0;
	nMotorEncoder[motorB] = 0;
	displayString(5, "speed: %d", dist);
	if (dist > 0)
	{
		if(!xAxis)
			motor[motorA] = -100;
		else
			motor[motorB] = 100;
	}
	else{
		if(!xAxis)
			motor[motorA] = 100;
		else
			motor[motorB] = -100;
	}
	displayString(5,"TESTING THIS %d", abs(dist * STEP_CM));
	if(!xAxis)
	{
		while(abs(nMotorEncoder[motorA]) < abs(dist * STEP_CM))
		{}
	}
	else
	{
		while(abs(nMotorEncoder[motorB]) < abs(dist * STEP_CM))
		{}
	}

	motor[motorA] = 0;
	motor[motorB] = 0;

	return;
}

void latch(bool latch)
{
	int val = latch?-70:0;
	wait1Msec(50);
	setServoPosition(S1,SERVO_PORT3,val);
	wait1Msec(50);
	displayString(5, "servo angle %d", val);
	return;
}

void moveSquare(int *current, int goal, bool xAxis)
{

	const float SQUARE = BOARD_LEN/BOARD_L;

	moveDist((goal-*current) * SQUARE, xAxis);
	*current = goal;

	return;
}

/*
void limitYStart()
{

	motor[motorA] = 100;
	while (!limitInteruptY())
	{}
	motor[motorA] = 0;

	return;
}
*/

void startUp()
{
	//go to bottom left till touch sensor interrupt for X and Y
	//limitYStart();



	return;
}

int comms(int *toMoveX, int *toMoveY)
{
	getJoystickSettings(joystick);

	int output = -1;

	if (joy1Btn(09) == 1)
	{
		return 9;
	}
	else if (joy1Btn(10) == 1)
	{
		return 10;
	}
	*toMoveX = 0;
	*toMoveY = 0;
	if(joy1Btn(01))
	{
		output = 1;
		*toMoveY+=1;
	}
	if(joy1Btn(02))
	{
		output = 1;
		*toMoveY+=2;
	}
	if(joy1Btn(03))
	{
		output = 1;
		*toMoveY+=4;
	}
	if(joy1Btn(04))
	{
		output = 1;
		*toMoveY+=8;
	}
	displayString(8, "cry: %d", joy1_TopHat);
	for(int count = 0; count <= 7; count ++)
	{
		if (joystick.joy1_TopHat == count)
		{
			*toMoveX = count+1;
			output = 1;
		}
	}

	displayString(1,"X move:%d", *toMoveX);
	displayString(3,"Y move:%d", *toMoveY);

	return output;
}

void comms()
{
	return;
}
