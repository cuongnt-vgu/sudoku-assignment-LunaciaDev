#include "hiddenPair.h"

int findHiddenPairInGroup(Axis* group, clearQueue** head) {
    int foundCount = 0;

    int bitsRangeA;
    int bitsRangeB;
    int temp;

    /*
    We first OR all possibility that's not from the 2 cells selected
    NOT that to get what is not within the combination.

    Compare that with the possibility of the selected cell to check if they are hidden!
    */

    for (int i = 0; i < 9; i++) {
        if (group -> memberArray[i] -> value != -1) continue;
        bitsRangeA = group -> memberArray[i] -> possibility;

        for (int k = i+1; k < 9; k++) {
            if (group -> memberArray[k] -> value != -1) continue;
            bitsRangeB = group -> memberArray[k] -> possibility;

            temp = 0;

            for (int l = 0; l < 9; l++) {
                if (l == i || l == k) continue;

                temp |= group -> memberArray[l] -> possibility;
            }

            temp = ~temp & BIT_MASK;

            temp = (bitsRangeA & temp) | (bitsRangeB & temp);

            if (temp == bitsRangeA && temp == bitsRangeB) continue;

            if (countOneBits(temp) == 2) {
                if (group -> memberArray[i] -> marked != 1 && group -> memberArray[k] -> marked != 1)
                    foundCount++;
                
                group -> memberArray[i] -> marked = 1;
                group -> memberArray[k] -> marked = 1;

                int* excludeMembers = malloc(sizeof(int) * 9);

                for (int m = 0; m < 9; m++) {
                    if (m == i || m == k) continue;

                    excludeMembers[m] = 1;
                }

                addMask(head, temp, group -> memberArray, excludeMembers);
            }
        }
    }

    return foundCount;
}

int checkHiddenPair(Cell*** sudokuGrid, Axis** boxGrid, Axis** rows, Axis** columns) {
    int foundCount = 0;
    clearQueue* head = NULL;

    clearAllMarker(sudokuGrid);

    for (int member = 0; member < 9; member++) {
        foundCount += findHiddenPairInGroup(rows[member], &head);
        foundCount += findHiddenPairInGroup(columns[member], &head);
        foundCount += findHiddenPairInGroup(boxGrid[member], &head);

    }

    applyMask(&head);

    return foundCount;
}