/**********************************************************************
Filename: EV3Servo-lib-UW.c
Date: November 3, 2017
File Version: 1.5

A wrapper for the drivers provided by Mindsensor for use with the
NXTServo interface module (which works with NXT or EV3).

History
Ver  Date       Comment
1.5  Nov  3/17  ported to EV3; STAT_COMM_PENDING needed to be defined;
                introduced wait between commands to address communication
                timing issue -> this should be improved for later releases
1.4  Jun  6/15  included code from NXTServo-lib.c (which was updated for
                parameter passing in RobotC v4.30); removed fixed deadband
                correction since it is different for each motor; reduced maximum
                gripper angle by 5 degrees;
1.3  Jan 15/15  changed offset values and scaling
1.2  Jan 14/15  scaled speed setting by 4. previous range from -100 to 100.
                updated range now -400 to 400 with the function call range
                still -100 to 100.
1.1  Jan 12/15  restrict to valid servo numbers 1-7; added SetGripperPos
                to limit gripper servo position;  limit servo position
                to 0 to 180; added constants, valid setting function,
                parameter check function;
                generalized to use any NXT sensor port;
1.0  Dec 18/14  original release

**********************************************************************/
/*
NXTServo-lib.c
This is function library to use with mindsensors.com NXTServo module.
For use with RobotC.
V1.0
2008(c) mindsensors.com
for more info visit www.mindsensors.com

History:
When           Ahthor/Editor          Comments
11/03/17       C. Hulls               ported to EV3; STAT_COMM_PENDING needed to be defined;
                                      introduced wait between commands to address communication
                                      timing issue -> this should be improved for later releases
06/06/15       C. Hulls               Convert I2C functions to pass pointers to message arrays;
                                      Removed extra ; from empty loopsl
05/29/08       Deepak Patil           Initial creation of the file.
03/21/11       support                Added support for Port and i2c address in API parameters
*/

/**********************************************************************
NXTServo-lib.c
**********************************************************************/
/*===========================================================================
Register Summary
===================
Register Read Operation                       Write Operation
0x00    sw version                            control reg

0x01    Servo 1 speed                         Servo 1 speed
0x02    Servo 1 Position low byte in uS	      Servo 1 Position low byte in uS
0x03    Servo 1 Position hi byte in uS	      Servo 1 Position hi byte in uS

0x04    Servo 2 speed                         Servo 2 speed
0x05    Servo 2 Position low byte in uS	      Servo 2 Position low byte in uS
0x06    Servo 2 Position hi byte in uS	      Servo 2 Position hi byte in uS

0x07    Servo 3 speed                         Servo 3 speed
0x08    Servo 3 Position low byte in uS	      Servo 3 Position low byte in uS
0x09    Servo 3 Position hi byte in uS	      Servo 3 Position hi byte in uS

0x0a    Servo 4 speed                         Servo 4 speed
0x0b    Servo 4 Position low byte in uS	      Servo 4 Position low byte in uS
0x0c    Servo 4 Position hi byte in uS	      Servo 4 Position hi byte in uS

0x0d    Servo 5 speed                         Servo 5 speed
0x0e    Servo 5 Position low byte in uS	      Servo 5 Position low byte in uS
0x0f    Servo 5 Position hi byte in uS	      Servo 5 Position hi byte in uS

0x10    Servo 6 speed                         Servo 6 speed
0x11    Servo 6 Position low byte in uS	      Servo 6 Position low byte in uS
0x12    Servo 6 Position hi byte in uS	      Servo 6 Position hi byte in uS

0x13    Servo 7 speed                         Servo 7 speed
0x14    Servo 7 Position low byte in uS	      Servo 7 Position low byte in uS
0x15    Servo 7 Position hi byte in uS	      Servo 7 Position hi byte in uS

0x16    Servo 8 speed                         Servo 8 speed
0x17    Servo 8 Position low byte in uS	      Servo 8 Position low byte in uS
0x18    Servo 8 Position hi byte in uS	      Servo 8 Position hi byte in uS

0x19    battery voltage	None


================
Psudo registers
================

0x1A			servo position 1			nothing
0x1b			servo position 2			nothing
0x1c			servo position 3			nothing
0x1d			servo position 4			nothing
0x1e			servo position 5			nothing
0x1f			servo position 6			nothing
0x20			servo position 7			nothing
0x21			servo position 8			nothing

*/

#pragma SystemFile

const int kSc8CommandReg    = 0x41;
const int kSc8_speed        = 0x52;
const int kSc8_lowbyte      = 0x42;
const int kSc8_hibyte       = 0x43;

const int kSc8_Vbatt        = 0x41;


/**********************************************************************
UW code
**********************************************************************/

// available servo numbers
const int MIN_SERVO_NUM = 1;
const int MAX_SERVO_NUM = 7;  // assumes SV8 used for power

// continous servo
const int MAX_SERVO_SPEED = 100;  // max at +/- 100
const int SERVO_NEUTRAL = 1550;  // 1550 us is neutral position (off)

// standard servo position limits
const int MIN_SERVO_ANGLE = 0;  // numbers are from datasheets, verified w/ servo
const int MAX_SERVO_ANGLE = 180;
const int SERVO_OFFSET = 90;  // make zero to be the neutral position
const int SERVO_ZERO = 600;  // 600 us is 0 angle for servo
const int MIN_GRIP_ANGLE = 50;  // numbers are based on tests with gripper
const int MAX_GRIP_ANGLE = 120;

// default I2C address (factory setting)
const int I2C_ADDR = 0xb0;

// control register
const ubyte CONTROL_REG_ADDR = 0x41;

// use NXT definition of STAT_COMM_PENDING which had to be added for EV3
#define STAT_COMM_PENDING 32


/**********************************************************************
NXTServo-lib.c
**********************************************************************/

/*===================================
**
** Read the battery voltage data from
** NXTServo module (in mili-volts)
**
===================================*/
int  NXTServo_Get_Batt_V(tSensors kSc8Port, int kSc8ID)
{
	ubyte sc8Msg[5];
	const int kMsgSize       = 0;
	const int kSc8Address    = 1;
	const int kReadAddress   = 2;
	ubyte replyMsg[2];

	// Build the I2C message
	sc8Msg[kMsgSize]         = 2;
	sc8Msg[kSc8Address]      = kSc8ID ;
	sc8Msg[kReadAddress]     = kSc8_Vbatt ;

	while (nI2CStatus[kSc8Port] == STAT_COMM_PENDING)  // CH removed ;
	{
		// Wait for I2C bus to be ready
	}
	// when the I2C bus is ready, send the message you built
	sendI2CMsg(kSc8Port, &sc8Msg[0], 1);  // CH added &

	while (nI2CStatus[kSc8Port] == STAT_COMM_PENDING)  // CH removed ;
	{
		// Wait for I2C bus to be ready
	}
	// when the I2C bus is ready, send the message you built
	readI2CReply(kSc8Port, &replyMsg[0], 1);  // CH added &

	return(37*(0x00FF & replyMsg[0]));  // 37 is calculated from

	//supply from NXT =4700 mv /128
}

/*===================================
**
** Quick setup for servo.
** Valid values are from 50 to 250
** uses pseudo registers from 0x1A to 0x21
**
===================================*/
void NXTServo_Quick_Servo_Setup(tSensors kSc8Port, int kSc8ID, int servoNumber, int position)
{

	ubyte sc8Msg[5];
	const int kMsgSize          = 0;
	const int kSc8Address       = 1;
	const int kSc8Servo         = 2;
	const int kData             = 3;

	// Build the I2C message
	sc8Msg[kMsgSize]            = 3;
	sc8Msg[kSc8Address]         = kSc8ID;
	sc8Msg[kSc8Servo]           = 0x59+servoNumber ;
	sc8Msg[kData]               = (ubyte)position ;

	while (nI2CStatus[kSc8Port] == STAT_COMM_PENDING)  // CH removed ;
	{
		// Wait for I2C bus to be ready
	}
	// when the I2C bus is ready, send the message you built
	sendI2CMsg(kSc8Port, &sc8Msg[0], 0);  // CH added &

	// introduce a wait to allow communication channel to idle
	wait1Msec(20);
}

/*===================================
**
** Set the speed of a servo to
** given value.
** highest speed -> 0
** lowest speed -> 255
**
===================================*/

void NXTServo_SetSpeed(tSensors kSc8Port, int kSc8ID, int servoNumber, int speed)
{

	ubyte sc8Msg[5];
	const int kMsgSize        = 0;
	const int kSc8Address     = 1;
	const int kSc8Servo       = 2;
	const int kData       = 3;

	// Build the I2C message
	sc8Msg[kMsgSize]          = 4;
	sc8Msg[kSc8Address]       = kSc8ID;
	sc8Msg[kSc8Servo]         = kSc8_speed+servoNumber-1 ;
	sc8Msg[kData]             = speed ;

	while (nI2CStatus[kSc8Port] == STAT_COMM_PENDING)  // CH removed ;
	{
		// Wait for I2C bus to be ready
	}
	// when the I2C bus is ready, send the message you built
	sendI2CMsg(kSc8Port, &sc8Msg[0], 0);  // CH added &

}

/*===================================
**
** Set the position of a servo to
** given value.
**
===================================*/

void NXTServo_SetPosition(tSensors kSc8Port, int kSc8ID, int servoNumber, int position)
{
	NXTServo_Quick_Servo_Setup(kSc8Port, kSc8ID, servoNumber, position/10);
}

/*
*
*
*
*/

/*
void NXTServo_SetPosition(tSensors kSc8Port, int kSc8ID, int servoNumber, int position)
{

byte sc8Msg[5];
const int kMsgSize        = 0;
const int kSc8Address     = 1;
const int kSc8Servo       = 2;
const int kData_low       = 3;
const int kData_hi        = 4;

// Build the I2C message
sc8Msg[kMsgSize]          = 4;
sc8Msg[kSc8Address]       = kSc8ID;
sc8Msg[kSc8Servo]         = kSc8_lowbyte+2*servoNumber-2 ;
sc8Msg[kData_low]         = (ubyte)position ;
sc8Msg[kData_hi]          = position/0x100;

while (nI2CStatus[kSc8Port] == STAT_COMM_PENDING)  // CH removed ;
{
// Wait for I2C bus to be ready
}
// when the I2C bus is ready, send the message you built
sendI2CMsg(kSc8Port, &sc8Msg[0], 0);  // CH added &

}

*/


/**********************************************************************
UW code
**********************************************************************/

void setValidSetting(int & setting, int minSetting, int maxSetting)
{
	if (setting > maxSetting)
		setting = maxSetting;
	else if (setting < minSetting)
		setting = minSetting;
}

bool paramIsValid(tSensors nxtPort, int servoNumber)
{
	return (nxtPort == S1 || nxtPort == S2 || nxtPort == S3 || nxtPort == S4)
	&& servoNumber >= MIN_SERVO_NUM && servoNumber <= MAX_SERVO_NUM;
}

void setServoSpeed(tSensors nxtPort, int servo_number, int speed_setting, int neg_offset = 0, int pos_offset = 0)
{
	if (paramIsValid(nxtPort,servo_number))
	{
		setValidSetting(speed_setting, -MAX_SERVO_SPEED, MAX_SERVO_SPEED);

		if (speed_setting == 0)
			NXTServo_SetPosition(nxtPort, I2C_ADDR, servo_number, SERVO_NEUTRAL);
		else if (speed_setting > 0)
			NXTServo_SetPosition(nxtPort, I2C_ADDR, servo_number, SERVO_NEUTRAL + (speed_setting + pos_offset) * 4);
		else
			NXTServo_SetPosition(nxtPort, I2C_ADDR, servo_number, SERVO_NEUTRAL + (speed_setting + neg_offset ) * 4);
	}
}

void setServoPosition(tSensors nxtPort, int servo_number, int position)
{
	if (paramIsValid(nxtPort,servo_number))
	{
		// zero is neutral position
		position = position + SERVO_OFFSET;

		setValidSetting(position, MIN_SERVO_ANGLE, MAX_SERVO_ANGLE);

		//now convert to servo signals and send.  The servo takes a signal from 600uS (-90 degrees) to 2400uS (90 degrees)
		NXTServo_SetPosition(nxtPort, I2C_ADDR, servo_number, SERVO_ZERO + (position)*10);
	}
}

/*
Set gripper position based on angle - normed to allow 0 to 70 as valid values
*/
void setGripperPosition(tSensors nxtPort, int servo_number, int position)
{
	if (paramIsValid(nxtPort,servo_number))
	{
		// zero commanded position is minimum angle
		position = position + MIN_GRIP_ANGLE;

		setValidSetting(position, MIN_GRIP_ANGLE, MAX_GRIP_ANGLE);

		//now convert to servo signals and send.  The servo takes a signal from 600uS (-90 degrees) to 2400uS (90 degrees)
		NXTServo_SetPosition(nxtPort, I2C_ADDR, servo_number, SERVO_ZERO + position*10);
	}
}

void resetGripper(tSensors nxtPort, int servoNumber)
{
	setGripperPosition(nxtPort, servoNumber, 90 - MIN_GRIP_ANGLE);
}

/*
The two functions below have not been tested.  They should reset the interface to its
factory settings.  They can be tested after the first problems with an interface,
otherwise at the moment it is best to leave things as is.
// I2C message send is copied from the code in NXTServo-lib.c
void NXTServo_SendCommand(tSensors port, ubyte cmd)
{
	ubyte sc8Msg[5];
	const int kMsgSize          = 0;
	const int kSc8Address       = 1;
	const int kSc8Register      = 2;
	const int kData             = 3;

	// Build the I2C message
	sc8Msg[kMsgSize]            = 3;
	sc8Msg[kSc8Address]         = I2C_ADDR;
	sc8Msg[kSc8Register]        = CONTROL_REG_ADDR;
	sc8Msg[kData]               = cmd;

	while (nI2CStatus[port] == STAT_COMM_PENDING)
	{
		// Wait for I2C bus to be ready
	}
	// when the I2C bus is ready, send the message you built
	sendI2CMsg(port, &sc8Msg[0], 0);
}

void resetServoInterface(tSensors port)
{
	NXTServo_SendCommand(port, (ubyte)'S');
}
*/
/**********************************************************************
Copyright(c) 2014-2017 D. Lau, M. Stachowsky, and C.C.W. Hulls, P.Eng.
Students, staff, and faculty members at the University of Waterloo
are granted a non-exclusive right to copy, modify, or use this
software for non-commercial teaching, learning, and research purposes
provided the author(s) are acknowledged except for as noted below.
**********************************************************************/
