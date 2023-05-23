#pragma once
#define EMPTYCELL '_'
#define OUTOFMAP '\0'
#define BLACKPAWN 'B'
#define WHITEPAWN 'W'
#define BORDER '+'

#define WHITETURN 1
#define BLACKTURN 2

#define PAWNAMOUNT 15

// Commands
#define LOAD_GAME_BOARD 1
#define PRINT_GAME_BOARD 2
#define DO_MOVE 3
#define PRINT_GAME_STATE 4
#define GEN_ALL_POS_MOV 5
#define GEN_ALL_POS_MOV_EXT 6
#define GEN_ALL_POS_MOV_NUM 7
#define GEN_ALL_POS_MOV_EXT_NUM 8
#define WINNING_SEQUENCE_EXIST 9
#define SOLVE_GAME_STATE 10
#define INVALID_COMMAND -1

// Directions
#define LEFT -1
#define RIGHT 1
#define UP -1
#define DOWN 1

// Comparisons
#define LEFTSMALLER -1
#define EQUAL 0
#define RIGHTSMALLER 1

// Move validity
#define NOERRORS -1
#define MOVE_COMMITTED 0
#define BAD_MOVE_FIRST_IS_WRONG_INDEX 1
#define BAD_MOVE_SECOND_IS_WRONG_INDEX 2
#define UNKNOWN_MOVE_DIRECTION 3

