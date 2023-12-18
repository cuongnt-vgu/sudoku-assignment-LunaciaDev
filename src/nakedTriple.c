#include "nakedTriple.h"

int findNakedTripleInGroup(Cell*** sudokuGrid, Axis* group, clearQueue** head) {
    int foundCount = 0;
    
    int bitsRangeA, bitsRangeB, bitsRangeC;

    /*
    We simply find 3 cells where OR their possibility yield 3x 1 bit.
    */

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

                    int* excludeMembers = malloc(sizeof(int) * 9);
                    excludeMembers[i] = 1;
                    excludeMembers[k] = 1;
                    excludeMembers[l] = 1;

                    addMask(head, (~bitsRangeC) & BIT_MASK, group -> memberArray, excludeMembers);
                }
            }
        }
    }

    return foundCount;
}

int checkNakedTriple(Cell*** sudokuGrid, Axis** boxGrid, Axis** rows, Axis** columns) {
    int foundCount = 0; // might need to return this
    clearQueue* head = NULL;

    // We could try and merge all the for loop into one as it feels redundant
    // but a lot of logic need to be changed, prob not worth it
    // continue; too powerful xd

    clearAllMarker(sudokuGrid);

    for (int member = 0; member < 9; member++) {
        foundCount += findNakedTripleInGroup(sudokuGrid, rows[member], &head);
        foundCount += findNakedTripleInGroup(sudokuGrid, columns[member], &head);
        foundCount += findNakedTripleInGroup(sudokuGrid, boxGrid[member], &head);
    }

    applyMask(&head);

    return foundCount;
}