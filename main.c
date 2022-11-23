#include "/includes/common_chess.h"
#include "/gantry/gantry.c"
#include "/comms/comms.c"
#include "/latch/latch.c"

void configure()
{
	BOARD_PARAMS.DIM_X = 37;
	BOARD_PARAMS.DIM_Y = 40;
	BOARD_PARAMS.xAxisClicks = 0;
	BOARD_PARAMS.yAxisClicks = 0;

	BOARD_PARAMS.yAxisConv = 100/1.06;
	BOARD_PARAMS.xAxisConv = 100/1.09;

	while(!configureSwitchs())
	{}
}

task main()
{
	configure();
	calibrate();
	moveDist(-3.3,false);
	moveDist(-0.3,true);
	Point_t currentPos;
	currentPos.xCord = 7;
	currentPos.yCord = 7;

	Point_t dest;
	dest.xCord = 7;
	dest.yCord = 7;

	Comms_Data_t commsInfo;
	Comms_Data_t emptyComms;
	emptyComms.comState = STATE_STANDBY;
	emptyComms.pointStart.xCord = 0;
	emptyComms.pointEnd.xCord = 0;
	emptyComms.pointStart.yCord = 0;
	emptyComms.pointEnd.yCord = 0;
	while (true)
	{
		comms_update(commsInfo);
		if (commsInfo.comState == STATE_MOVE)
		{
			moveSquare(currentPos, commsInfo.pointStart);
			latch(true);
			moveSquare(currentPos, commsInfo.pointEnd);
			latch(false);
		}
		else if (commsInfo.comState == STATE_STANDBY)
		{
			latch(false);
		}
		commsInfo = emptyComms;
	}

}
