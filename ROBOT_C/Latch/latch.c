#include "latch.h"

void latch(bool latchStatus)
{
    int angleLatch = -72;
    if (latchStatus)
    {
    	angleLatch = -72;
  	}
   	else
   		angleLatch = -20;

    setServoPosition(TETRIX_PORT,
                     SERVO_PORT,
                     angleLatch);
		wait10Msec(10);
    return;
}
