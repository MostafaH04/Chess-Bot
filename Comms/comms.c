#include "../Comms/comms.h"

int _comms_getState()
{
	/* States */
	int state = STATE_STANDBY;

	if (joy1Btn(07))
	{
		return STATE_ERROR;
	}
	else if (joy1Btn(09))
	{
		state = STATE_TAKE;
	}
	else if (joy1Btn(10))
	{
		state =  STATE_CASTLE;
	}
	else if (joy1Btn(11))
	{
		state = STATE_CHECK;
	}
	else if (joy1Btn(06))
	{
		state = STATE_MOVE;
	}

	return state;
}

void _comms_startPoint(Point_t *startPoint)
{
	/* Start Point */
    // ensure initial values are zeroed
	startPoint->xCord = 0;
	startPoint->yCord = 0;

	if(joy1Btn(01))
	{
		startPoint->yCord+=1;
	}
	if(joy1Btn(02))
	{
		startPoint->yCord+=2;
	}
	if(joy1Btn(03))
	{
		startPoint->yCord+=4;
	}
	if(joy1Btn(04))
	{
		startPoint->yCord+=8;
	}


	for(int count = 0; count <= 7; count ++)
	{
		if (joystick.joy1_TopHat == count)
		{
			startPoint->xCord = count;
		}
	}

	/* Start Point End */
}

int _comms_normalizeAxis(int axis)
{
	const int ZONE_DIV = 33;
	float reading = axis + 128;
	reading = reading/228.00;
	return (int)round(7*reading);
}

int _comms_readAxis(bool firstJoy)
{
	int outCord = _comms_normalizeAxis(firstJoy?joystick.joy1_x1:-joystick.joy1_y1);
	return outCord;
}

void _comms_endPoint(Point_t *endPoint)
{
	const int ZONE_MAX = -128;
	const int ZONE_DIV = 95;

	endPoint->xCord = _comms_readAxis(true);
	endPoint->yCord = _comms_readAxis(false);
}

void comms_update(Comms_Data_t *commsOut)
{
	// read joystick settings
  getJoystickSettings(joystick);

	commsOut->comState = _comms_getState();
	_comms_startPoint(commsOut->pointStart);
	_comms_endPoint(commsOut->pointEnd);

	return;
}
