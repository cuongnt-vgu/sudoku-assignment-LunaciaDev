#include "hiddenPair.h"

int findHiddenPairInGroup(Cell*** sudokuGrid, Axis* group) {
    int foundCount = 0;

    int* bitsRangeA;
    int* bitsRangeB;
    int temp;

    for (int i = 0; i < 9; i++) {
        if (group -> memberArray[i] -> value != -1) continue;
        bitsRangeA = &group -> memberArray[i] -> possibility;

        for (int k = i+1; k < 9; k++) {
            if (group -> memberArray[k] -> value != -1) continue;
            bitsRangeB = &group -> memberArray[k] -> possibility;

            temp = 0;

            for (int l = 0; l < 9; l++) {
                if (l == i || l == k) continue;

                temp |= group -> memberArray[l] -> possibility;
            }

            temp = ~temp & BIT_MASK;

            temp = (*bitsRangeA & temp) | (*bitsRangeB & temp);

            if (countOneBits(temp) == 2) {
                if (group -> memberArray[i] -> marked != 1 && group -> memberArray[k] -> marked != 1)
                    foundCount++;
                
                group -> memberArray[i] -> marked = 1;
                group -> memberArray[k] -> marked = 1;

                *bitsRangeA &= temp;
                *bitsRangeB &= temp;
            }
        }
    }

    return foundCount;
}

int checkHiddenPair(Cell*** sudokuGrid, Axis** boxGrid, Axis** rows, Axis** columns) {
    int foundCount = 0;

    clearAllMarker(sudokuGrid);

    for (int member = 0; member < 9; member++) {
        
        foundCount += findHiddenPairInGroup(sudokuGrid, rows[member]);
        foundCount += findHiddenPairInGroup(sudokuGrid, columns[member]);
        foundCount += findHiddenPairInGroup(sudokuGrid, boxGrid[member]);

    }

    return foundCount;
}