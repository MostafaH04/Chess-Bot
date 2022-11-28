#include "/includes/common_chess.h"
#include "/gantry/gantry.c"
#include "/comms/comms.c"
#include "/latch/latch.c"

bool waitButton(int buttonType)
{
	while (!getButtonPress(buttonType) || !getButtonPress(buttonAny))
	{}

	if (getButtonPress(buttonType))
	{
		while(getButtonPress(buttonAny))
		{}
		return true
	}

	return false;
}

task main()
{
	configureSwitchs();
	latch(false);
	calibrate();
	Point_t currentPos;
	currentPos.xCord = 7;
	currentPos.yCord = 7;
	Point_t dest;
	dest.xCord = 4;
	dest.yCord = 4;

	moveSquare(currentPos, dest);


	Comms_Data_t commsInfo;

	Comms_Data_t emptyComms;
	emptyComms.comState = STATE_STANDBY;
	emptyComms.pointStart.xCord = 0;
	emptyComms.pointEnd.xCord = 0;
	emptyComms.pointStart.yCord = 0;
	emptyComms.pointEnd.yCord = 0;

	int countMoves = 0;
	int countMax[4] = {4, 2, 2, 1};
	int countMaxCounter = 0;

	double totalTime = 0;
	int totalPlays = 0;

	bool gameRunning = true;
	while (gameRunning)
	{
		comms_update(commsInfo);
		if (commsInfo.comState == STATE_MOVE || commsInfo.comState == STATE_CASTLE)
		{
			countMoves ++;
			time1[T1] = 0;
			executeMove(currentPos, commsInfo.pointStart, commsInfo.pointEnd, commsInfo.comState);
			eraseDisplay()
			displayString(5, "It took the bot %d s", time1[T1]/1000);
			totalTime += time1[T1]/1000;
			displayString(6, "to respond to your move.");
			wait10Msec(100);
		}
		else if (commsInfo.comState == STATE_TAKE)
		{
			countMoves ++;
			eject(currentPos, commsInfo.pointEnd);
			executeMove(currentPos, commsInfo.pointStart, commsInfo.pointEnd, commsInfo.comState);
		}
		else if (commsInfo.comState == STATE_CHECK)
		{
			eraseDisplay();
			displayString(5, "CHECK MATE");
			displayString(6, "GAME OVER");
			displayString(8, "Average time: %f s", (totalTime/totalPlays));
			waitButton(buttonAny);

			gameRunning = false;

		}
		else if (commsInfo.comState == STATE_ERROR)
		{
			eraseDisplay();
			displayString(3, "There has been an error!");
			displayString(4, "Refer to python if needed.");
			displayString(5, "Press any button when good.");
			displayString(6, "Then update screen on python.");
			waitButton(buttonAny);
		}
		else if (commsInfo.comSTate == STATE_STANDBY)
		{
			time1[T1] = 0;
			eraseDisplay();
			displayString(5, "Make a move when good");
			displayString(6, "press any button.");
			displayString(7, "Update python.");
			waitButton(buttonAny);
			eraseDisplay();
			displayString(4, "You took %d s", time1[T1]/1000);
			totalTime += time1[T1]/1000;
			totalPlays ++;
			displaySTring(5, "To make a move");
		}
		commsInfo = emptyComms;

		if (countMoves > countMax[countMaxCounter])
		{
			eraseDisplay();
			displayString(5, "PLEASE WAIT!");
			displayString(6, "Currently recalibrating for");
			displayString(7, "accuracy.");
			latch(false);
			countMoves = 0;
			if (countMaxCounter != 3)
			{
				countMaxCounter ++;
			}
			calibrate();
			currentPos.xCord = 7;
			currentPos.yCord = 7;
			moveSquare(currentPos, dest);
		}
	}
	eraseDisplay();
	displayString(5, "Calibrating and exiting!");

	calibrate();

}
