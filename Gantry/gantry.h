#ifndef GANTRY_MOVEMENT
#define GANTRY_MOVEMENT

#include "../includes/common_chess.h"

//#define TOUCH_Y1 msensor_S4_1
//const int TOUCH_Y2 = S3

//#define TOUCH_X1 msensor_S4_2
//#define TOUCH_X2 msensor_S4_3

/**
 * @brief Configures limit swithes.
 *
 */
bool configSwitchs();

/**
 * @brief Reads the given switch, and returns
 *        its value. Acts as a way to normalize
 *        calls to reading limit switchs.
 *
 * @param int touchPort
 * @return bool
 */
//bool readSwitch(int touchSensor);

/**
 * @brief Limit switch interupt for
 *        y axis (switch 1).
 *
 * @return bool
 */
bool limitY1();
/**
 * @brief Limit switch interupt for
 *        y axis (switch 2).
 *
 * @return bool
 */
bool limitY2();

/**
 * @brief Limit switch interupt for
 *        x axis (switch 1).
 *
 * @return bool
 */
bool limitX1();
/**
 * @brief Limit switch interupt for
 *        x axis (switch 2).
 *
 * @return bool
 */
bool limitX2();

/**
 * @brief Calibrates the selected axis, and returns
 *        the encoder clicks for the entire axis.
 *
 * @param bool xAxis
 * @return int
 */
int calibrateAxis(bool yAxis);
/**
 * @brief Calibrates the gantry. Called at the begining,
 *        or whenever the gantry needs recalibration.
 *
 */
void calibrate();

/**
 * @brief Moves the gantry a certain distance in any
 *        selected axis.
 *
 * @param float dist_cm
 * @param float yAxis
 */
void moveDist(float dist_cm, bool yAxis);

void moveSquare(Point_t *currentPoint, Point_t *goalPoint);

#endif /* GANTRY_MOVEMENT */
