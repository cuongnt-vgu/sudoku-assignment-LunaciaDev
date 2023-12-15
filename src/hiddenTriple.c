#include "hiddenTriple.h"
#include "auxilary.h"

int findHiddenTripleInGroup(Cell*** sudokuGrid, Axis* group) {
    int foundCount = 0;

    int temp;
    int* bitsRangeA;
    int* bitsRangeB;
    int* bitsRangeC;

    for (int i = 0; i < 9; i++) {
        if (group -> memberArray[i] -> value != -1) continue;
        bitsRangeA = &group -> memberArray[i] -> possibility;

        for (int k = i+1; k < 9; k++) {
            if (group -> memberArray[k] -> value != -1) continue;
            bitsRangeB = &group -> memberArray[k] -> possibility;

            for (int l = k+1; l < 9; l++) {
                if (group -> memberArray[l] -> value != -1) continue;
                bitsRangeC = &group -> memberArray[l] -> possibility;

                temp = 0;

                for (int m = 0; m < 9; m++) {
                    if (m == i || m == k || m == l) continue;

                    temp |= group -> memberArray[m] -> possibility;
                }

                temp = ~temp & BIT_MASK;

                if (countOneBits(*bitsRangeA & temp) < 2) continue;
                if (countOneBits(*bitsRangeB & temp) < 2) continue;
                if (countOneBits(*bitsRangeC & temp) < 2) continue;

                temp = (*bitsRangeA & temp) | (*bitsRangeB & temp) | (*bitsRangeC & temp);

                if (countOneBits(temp) == 3) {
                    if (group -> memberArray[i] -> marked != 1 && group -> memberArray[k] -> marked != 1 && group -> memberArray[l] -> marked != 1)
                        foundCount++;
                    
                    group -> memberArray[i] -> marked = 1;
                    group -> memberArray[k] -> marked = 1;
                    group -> memberArray[l] -> marked = 1;

                    *bitsRangeA &= temp;
                    *bitsRangeB &= temp;
                    *bitsRangeC &= temp;
                }
            }
        }
    }

    return foundCount;
}

int checkHiddenTriple(Cell*** sudokuGrid, Axis** boxGrid, Axis** rows, Axis** columns) {
    int foundCount = 0;

    clearAllMarker(sudokuGrid);

    for (int member = 0; member < 9; member++) {
        foundCount += findHiddenTripleInGroup(sudokuGrid, rows[member]);
        foundCount += findHiddenTripleInGroup(sudokuGrid, columns[member]);
        foundCount += findHiddenTripleInGroup(sudokuGrid, boxGrid[member]);
    }

    return foundCount;
}