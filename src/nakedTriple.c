#include "nakedTriple.h"

int findNakedTripleInGroup(Cell*** sudokuGrid, Axis* group) {
    int foundCount = 0;
    
    int bitsRangeA, bitsRangeB, bitsRangeC;

    for (int i = 0; i < 9; i++) {
        if (group -> memberArray[i] -> value != -1) continue;
        bitsRangeA = group -> memberArray[i] -> possibility;

        for (int k = i+1; k < 9; k++) {
            if (group -> memberArray[k] -> value != -1) continue;
            bitsRangeB = bitsRangeA | group -> memberArray[k] -> possibility;

            for (int l = k+1; l < 9; l++) {
                if (group -> memberArray[l] -> value != -1) continue;
                bitsRangeC = bitsRangeA | bitsRangeB | group -> memberArray[l] -> possibility;

                if (countOneBits(bitsRangeC) == 3) {
                    if (group -> memberArray[i] -> marked != 1 && group -> memberArray[k] -> marked != 1 && group -> memberArray[l] -> marked != 1)
                        foundCount++;
                    
                    group -> memberArray[i] -> marked = 1;
                    group -> memberArray[k] -> marked = 1;
                    group -> memberArray[l] -> marked = 1;

                    for (int m = 0; m < 9; m++) {
                        if (m == i || m == k || m == l) continue;

                        group -> memberArray[m] -> possibility &= (~bitsRangeC) & BIT_MASK;
                    }

                }
            }
        }
    }

    return foundCount;
}

int checkNakedTriple(Cell*** sudokuGrid, Axis** boxGrid, Axis** rows, Axis** columns) {
    int checksum = generateBoardChecksum(sudokuGrid);
    int foundCount = 0; // might need to return this

    // We could try and merge all the for loop into one as it feels redundant
    // but a lot of logic need to be changed, prob not worth it
    // continue; too powerful xd

    clearAllMarker(sudokuGrid);

    for (int member = 0; member < 9; member++) {
        foundCount += findNakedTripleInGroup(sudokuGrid, rows[member]);
        foundCount += findNakedTripleInGroup(sudokuGrid, columns[member]);
        foundCount += findNakedTripleInGroup(sudokuGrid, boxGrid[member]);
    }

    if (checksum != generateBoardChecksum(sudokuGrid)) return foundCount;

    return 0;
}