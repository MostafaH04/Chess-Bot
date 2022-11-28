#ifndef COMMON_CHESS_FILE
#define COMMON_CHESS_FILE

#include "../includes/UW_sensorMux.c"

#define BOARD_SIZE 8

#define BOARD_DIM 30
#define SQUARE_DIM 3.75

#define MAX_POW 100
#define HALF_POW 50
#define STOP 0

const float DIM_Y = 30;
const float DIM_X = 30;


const float Y_AXIS_CONV = 100/1.08;
const float X_AXIS_CONV = 100/1.09;


/**
 * @brief Point type struct:
 *        Stores x and y coordinates of
 *        a point (using coordinate plane
 *        of a chess board [0 to 7]).
 *
 */
typedef struct{

    float xCord;
    float yCord;

} Point_t;

/**
 * @brief Comms_Data struct: Stores retrieved
 *        communication state along with the
 *        start and end points.
 *
 */
typedef struct{

    int comState;

    Point_t pointStart;
    Point_t pointEnd;

} Comms_Data_t;

#endif /* COMMON CHESS FILE */
