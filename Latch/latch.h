#ifndef SERVO_LATCH
#define SERVO_LATCH

#include "../includes/EV3Servo-lib-UW.c"
#include "../includes/common_chess.h"

#define SERVO_PORT 3
#define TETRIX_PORT S1

/**
 * @brief Recives a bool value to latch
 *        or unlatch the servo magnetic mechanism.
 *
 * @param bool latchStatus
 */
void latch(bool latchStatus);

#endif /* Servo Latch */
