#include "hiddenPair.h"

int checkHiddenPair(Cell*** sudokuGrid, Box** boxGrid, Axis** rows, Axis** columns) {
    int foundCount = 0;

    int* bitsRangeA;
    int* bitsRangeB;
    int temp;

    for (int member = 0; member < 9; member++) {

        //check the corresponding row
        for (int i = 0; i < 9; i++) {
            if (rows[member] -> memberArray[i] -> value != -1) continue;
            bitsRangeA = &rows[member] -> memberArray[i] -> possibility;

            for (int k = i+1; k < 9; k++) {
                if (rows[member] -> memberArray[k] -> value != -1) continue;
                bitsRangeB = &rows[member] -> memberArray[k] -> possibility;

                temp = 0;

                for (int l = 0; l < 9; l++) {
                    if (l == i || l == k) continue;

                    temp |= rows[member] -> memberArray[l] -> possibility;
                }

                temp = ~temp & BIT_MASK;

                temp = (*bitsRangeA & temp) | (*bitsRangeB & temp);

                if (countOneBits(temp) == 2) {
                    foundCount++;
                    *bitsRangeA &= temp;
                    *bitsRangeB &= temp;
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

                temp = 0;

                for (int l = 0; l < 9; l++) {
                    if (l == i || l == k) continue;

                    temp |= columns[member] -> memberArray[l] -> possibility;
                }

                temp = ~temp & BIT_MASK;

                temp = (*bitsRangeA & temp) | (*bitsRangeB & temp);

                if (countOneBits(temp) == 2) {
                    foundCount++;
                    *bitsRangeA &= temp;
                    *bitsRangeB &= temp;
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

                temp = 0;

                for (int l = 0; l < 9; l++) {
                    if (l == i || l == k) continue;

                    temp |= boxGrid[member] -> memberArray[l] -> possibility;
                }

                temp = ~temp & BIT_MASK;

                temp = (*bitsRangeA & temp) | (*bitsRangeB & temp);

                if (countOneBits(temp) == 2) {
                    foundCount++;
                    *bitsRangeA &= temp;
                    *bitsRangeB &= temp;
                }
            }
        }
    }

    return foundCount;
}