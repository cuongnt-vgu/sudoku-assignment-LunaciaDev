#include "nakedPair.h"

int checkNakedPair(Cell*** sudokuGrid, Axis** boxGrid, Axis** rows, Axis** columns) {
    int checksum = generateBoardChecksum(sudokuGrid);
    int foundCount = 0; // might need to return this
    int bitsRangeA, bitsRangeB;

    // We could try and merge all the for loop into one as it feels redundant
    // but a lot of logic need to be changed, prob not worth it
    // continue; too powerful xd

    for (int member = 0; member < 9; member++) {

        //check the corresponding row
        for (int i = 0; i < 9; i++) {
            if (rows[member] -> memberArray[i] -> value != -1) continue;
            bitsRangeA = rows[member] -> memberArray[i] -> possibility;

            for (int k = i+1; k < 9; k++) {
                if (rows[member] -> memberArray[k] -> value != -1) continue;
                bitsRangeB = bitsRangeA | rows[member] -> memberArray[k] -> possibility;
                
                if (countOneBits(bitsRangeB) == 2) {
                    foundCount++;

                    for (int l = 0; l < 9; l++) {
                        if (l == i || l == k) continue;
                        
                        rows[member] -> memberArray[l] -> possibility &= (~bitsRangeA) & BIT_MASK;
                    }
                }
            }
        }

        //check the corresponding column
        for (int i = 0; i < 9; i++) {
            if (columns[member] -> memberArray[i] -> value != -1) continue;
            bitsRangeA = columns[member] -> memberArray[i] -> possibility;

            for (int k = i+1; k < 9; k++) {
                if (columns[member] -> memberArray[k] -> value != -1) continue;
                bitsRangeB = bitsRangeA | columns[member] -> memberArray[k] -> possibility;
                
                if (countOneBits(bitsRangeB) == 2) {
                    foundCount++;

                    for (int l = 0; l < 9; l++) {
                        if (l == i || l == k) continue;
                        
                        columns[member] -> memberArray[l] -> possibility &= (~bitsRangeA) & BIT_MASK;
                    }

                }
            }
        }

        //check the corresponding box
        for (int i = 0; i < 9; i++) {
            if (boxGrid[member] -> memberArray[i] -> value != -1) continue;
            bitsRangeA = boxGrid[member] -> memberArray[i] -> possibility;

            for (int k = i+1; k < 9; k++) {
                if (boxGrid[member] -> memberArray[k] -> value != -1) continue;
                bitsRangeB = bitsRangeA | boxGrid[member] -> memberArray[k] -> possibility;
                
                if (countOneBits(bitsRangeB) == 2) {
                    foundCount++;

                    for (int l = 0; l < 9; l++) {
                        if (l == i || l == k) continue;
                        
                        boxGrid[member] -> memberArray[l] -> possibility &= (~bitsRangeA) & BIT_MASK;
                    }

                }
            }
        }
    }

    if (checksum != generateBoardChecksum(sudokuGrid)) return foundCount;

    return 0;
}