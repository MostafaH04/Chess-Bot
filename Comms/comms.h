#ifndef EV3_COMMS
#define EV3_COMMS

#include "JoystickDriver.c";
#include "../includes/common_chess.h"

#define STATE_STANDBY -1
#define STATE_ERROR 8
#define STATE_TAKE 9
#define STATE_CASTLE 10
#define STATE_CHECK 11
#define STATE_MOVE 1


// private

/**
 * @brief Retrieves state type from comms data.
 *
 * @return int
 */
int _comms_getState();

/**
 * @brief Retrieves the starting point from
 *        the comms data.
 *
 * @param Point startPoint
 */
void _comms_startPoint(Point_t *startPoint);

/**
 * @brief Reduces value of joystick axis readings
 *        from -100 to 100 to -1 to 1 (int)
 *
 * @param int axis
 * @return int
 */
int _comms_normalizeAxis(int axis);

/**
 * @brief Reads the point movement based on the
 *        selected joystick axis's position.
 *
 * @param bool firstJoy
 * @return int
 */
int _comms_readAxis(bool firstJoy);

/**
 * @brief Retrieves the end point from the
 *        comms data.
 *
 * @param Point endPoint
 */
void _comms_endPoint(Point_t *endPoint);


// public
/**
 * @brief Updates the comms status,
 *        retrieving any moves if needed.
 *
 * @param Comms_Data commsOut
 */
void comms_update(Comms_Data_t *commsOut);

#endif /* EV3_COMMS */
