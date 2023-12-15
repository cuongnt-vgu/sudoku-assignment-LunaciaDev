#pragma once

//Create 2 lookup tables
//This lookup table hold the value of 2^x
//This enable setting the bit at position x to 0 or 1 rapidly
extern const int TWO_POWER_LOOKUP[9];

//This lookup table hold the starting position of n-th box
extern const int BOX_LOOKUP[9][2];

//A constant for 9 bit of 1
extern const int BIT_MASK;

extern const int BOARD_SIZE;

typedef struct Cell {
    int value;
    int possibility;
    int fixed;
    int marked;
} Cell;

typedef struct Box {
    Cell** memberArray;
    int impossibleValue;
} Box;

typedef struct Axis {
    Cell** memberArray;
    int impossibleValue;
} Axis;

typedef struct clearQueue {
    int mask;
    Cell** groupToApply;
    int* excludeMembers;
    struct clearQueue* next;
} clearQueue;