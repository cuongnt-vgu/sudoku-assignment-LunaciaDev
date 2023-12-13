#include "hiddenTriple.h"

int checkHiddenTriple(Cell*** sudokuGrid, Axis** boxGrid, Axis** rows, Axis** columns) {
    int foundCount = 0;

    int* bitsRangeA;
    int* bitsRangeB;
    int* bitsRangeC;
    int temp;

    for (int member = 0; member < 9; member++) {

        //check the corresponding row
        for (int i = 0; i < 9; i++) {
            if (rows[member] -> memberArray[i] -> value != -1) continue;
            bitsRangeA = &rows[member] -> memberArray[i] -> possibility;

            for (int k = i+1; k < 9; k++) {
                if (rows[member] -> memberArray[k] -> value != -1) continue;
                bitsRangeB = &rows[member] -> memberArray[k] -> possibility;

                for (int l = k+1; l < 9; l++) {
                    if (rows[member] -> memberArray[l] -> value != -1) continue;
                    bitsRangeC = &rows[member] -> memberArray[l] -> possibility;

                    temp = 0;

                    for (int m = 0; m < 9; m++) {
                        if (m == i || m == k || m == l) continue;

                        temp |= rows[member] -> memberArray[m] -> possibility;
                    }

                    temp = ~temp & BIT_MASK;

                    if (countOneBits(*bitsRangeA & temp) < 2) continue;
                    if (countOneBits(*bitsRangeB & temp) < 2) continue;
                    if (countOneBits(*bitsRangeC & temp) < 2) continue;

                    temp = (*bitsRangeA & temp) | (*bitsRangeB & temp) | (*bitsRangeC & temp);

                    if (countOneBits(temp) == 3) {
                        foundCount++;
                        *bitsRangeA &= temp;
                        *bitsRangeB &= temp;
                        *bitsRangeC &= temp;
                    }
                }
            }
        }

        //check the corresponding column
        for (int i = 0; i < 9; i++) {
            if (columns[member] -> memberArray[i] -> value != -1) continue;
            bitsRangeA = &columns[member] -> memberArray[i] -> possibility;

            for (int k = i+1; k < 9; k++) {
                if (columns[member] -> memberArray[k] -> value != -1) continue;
                bitsRangeB = &columns[member] -> memberArray[k] -> possibility;
                
                for (int l = k+1; l < 9; l++) {
                    if (columns[member] -> memberArray[l] -> value != -1) continue;
                    bitsRangeC = &columns[member] -> memberArray[l] -> possibility;

                    temp = 0;

                    for (int m = 0; m < 9; m++) {
                        if (m == i || m == k || m == l) continue;

                        temp |= columns[member] -> memberArray[m] -> possibility;
                    }

                    temp = ~temp & BIT_MASK;

                    if (countOneBits(*bitsRangeA & temp) < 2) continue;
                    if (countOneBits(*bitsRangeB & temp) < 2) continue;
                    if (countOneBits(*bitsRangeC & temp) < 2) continue;

                    temp = (*bitsRangeA & temp) | (*bitsRangeB & temp) | (*bitsRangeC & temp);

                    if (countOneBits(temp) == 3) {
                        foundCount++;
                        *bitsRangeA &= temp;
                        *bitsRangeB &= temp;
                        *bitsRangeC &= temp;
                    }
                }
            }
        }

        //check the corresponding box
        for (int i = 0; i < 9; i++) {
            if (boxGrid[member] -> memberArray[i] -> value != -1) continue;
            bitsRangeA = &boxGrid[member] -> memberArray[i] -> possibility;

            for (int k = i+1; k < 9; k++) {
                if (boxGrid[member] -> memberArray[k] -> value != -1) continue;
                bitsRangeB = &boxGrid[member] -> memberArray[k] -> possibility;

                for (int l = k+1; l < 9; l++) {
                    if (boxGrid[member] -> memberArray[l] -> value != -1) continue;
                    bitsRangeC = &boxGrid[member] -> memberArray[l] -> possibility;

                    temp = 0;

                    for (int m = 0; m < 9; m++) {
                        if (m == i || m == k || m == l) continue;

                        temp |= boxGrid[member] -> memberArray[m] -> possibility;
                    }

                    temp = ~temp & BIT_MASK;

                    if (countOneBits(*bitsRangeA & temp) < 2) continue;
                    if (countOneBits(*bitsRangeB & temp) < 2) continue;
                    if (countOneBits(*bitsRangeC & temp) < 2) continue;

                    temp = (*bitsRangeA & temp) | (*bitsRangeB & temp) | (*bitsRangeC & temp);

                    if (countOneBits(temp) == 3) {
                        foundCount++;
                        *bitsRangeA &= temp;
                        *bitsRangeB &= temp;
                        *bitsRangeC &= temp;
                    }

                }
            }
        }
    }

    return foundCount;
}