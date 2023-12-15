#include "nakedPair.h"

int findNakedPairInGroup(Cell*** sudokuGrid, Axis* group) {
    int foundCount = 0;

    int bitsRangeA, bitsRangeB;
    
    for (int i = 0; i < 9; i++) {
        if (group -> memberArray[i] -> value != -1) continue;
        bitsRangeA = group -> memberArray[i] -> possibility;

        for (int k = i+1; k < 9; k++) {
            if (group -> memberArray[k] -> value != -1) continue;
            bitsRangeB = bitsRangeA | group -> memberArray[k] -> possibility;
            
            if (countOneBits(bitsRangeB) == 2) {
                if (group -> memberArray[i] -> marked != 1 && group -> memberArray[k] -> marked != 1)
                    foundCount++;
                
                group -> memberArray[i] -> marked = 1;
                group -> memberArray[k] -> marked = 1;

                for (int l = 0; l < 9; l++) {
                    if (l == i || l == k) continue;
                    
                    group -> memberArray[l] -> possibility &= (~bitsRangeA) & BIT_MASK;
                }
            }
        }
    }

    return foundCount;
}

int checkNakedPair(Cell*** sudokuGrid, Axis** boxGrid, Axis** rows, Axis** columns) {
    int checksum = generateBoardChecksum(sudokuGrid);
    int foundCount = 0; // might need to return this

    // We could try and merge all the for loop into one as it feels redundant
    // but a lot of logic need to be changed, prob not worth it
    // continue; too powerful xd

    clearAllMarker(sudokuGrid);

    for (int member = 0; member < 9; member++) {
        foundCount += findNakedPairInGroup(sudokuGrid, rows[member]);
        foundCount += findNakedPairInGroup(sudokuGrid, columns[member]);
        foundCount += findNakedPairInGroup(sudokuGrid, boxGrid[member]);
    }

    if (checksum != generateBoardChecksum(sudokuGrid)) return foundCount;

    return 0;
}