#ifndef COMMON_CHESS_FILE
#define COMMON_CHESS_FILE

#include "../includes/UW_sensorMux.c"

#define BOARD_SIZE 8

#define BOARD_DIM 30
#define SQUARE_DIM 3.75

#define MAX_POW 100
#define HALF_POW 50
#define STOP 0

/**
 * @brief Board type struct:
 *        Stores info regarding board calibration
 *        and board size.
 *
 */
typedef struct{

    float DIM_X;
    float DIM_Y;

    int xAxisClicks;
    int yAxisClicks;

    int xAxisConv;
    int yAxisConv;

} Board_t;

Board_t BOARD_PARAMS;


/**
 * @brief Point type struct:
 *        Stores x and y coordinates of
 *        a point (using coordinate plane
 *        of a chess board [0 to 7]).
 *
 */
typedef struct{

    int xCord;
    int yCord;

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
