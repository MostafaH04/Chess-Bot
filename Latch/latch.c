#include "latch.h"

void latch(bool latchStatus)
{
    int angleLatch = -65;
    if (latchStatus)
    	angleLatch = -65;
   	else
   		angleLatch = -20;

    setServoPosition(TETRIX_PORT,
                     SERVO_PORT,
                     angleLatch);

    return;
}
