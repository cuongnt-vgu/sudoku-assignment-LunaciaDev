#include "auxilary.h"

void printSudokuGrid(Cell*** sudokuGrid) {
    char printGrid[9][9][9];

    for (int i = 0; i < 9; i++) {
        for (int k = 0; k < 9; k++) {
            for (int l = 0; l < 9; l++) {
                printGrid[i][k][l] = 32;
            }
        }
    }

    int temp;

    for (int i = 0; i < 9; i++) {
        for (int k = 0; k < 9; k++) {
            if (sudokuGrid[i][k] -> value != -1) {
                printGrid[i][k][4] = (char) sudokuGrid[i][k] -> value + 49;
                continue;
            }

            temp = sudokuGrid[i][k] -> possibility;

            for (int l = 0; l < 9; l++) {
                printGrid[i][k][l] = (char) (temp % 2 ? l + 49 : 32);
                temp /= 2;
            }
        }
    }

    // Do I hard code this pretty printer
    // Maybe
    // What have I done again

    for (int i = 0; i < 9; i++) {
        for (int k = 0; k < 9; k++) {
            for (int l = 0; l < 3; l++) {
                printf("%c", printGrid[i][k][l]);
            }

            printf(" ");

            if (k % 3 == 2) printf("|");
        }

        printf("\n");

        for (int k = 0; k < 9; k++) {
            for (int l = 3; l < 6; l++) {
                printf("%c", printGrid[i][k][l]);
            }

            printf(" ");

            if (k % 3 == 2) printf("|");
        }

        printf("\n");

        for (int k = 0; k < 9; k++) {
            for (int l = 6; l < 9; l++) {
                printf("%c", printGrid[i][k][l]);
            }

            printf(" ");

            if (k % 3 == 2) printf("|");
        }

        printf("\n");
        printf("---------------------------------------\n");
    }

    printf("\n");
}

void cleanUp(Cell*** sudokuGrid, Box** boxGrid, Axis** row, Axis** column) {
    for (int i = 0; i < 9; i++) {
        for (int k = 0; k < 9; k++) {
            free(sudokuGrid[i][k]);
        }

        free(sudokuGrid[i]);
    }

    free(sudokuGrid);

    for (int i = 0; i < 9; i++) {
        free(boxGrid[i] -> memberArray);
        free(boxGrid[i]);
        free(row[i] -> memberArray);
        free(row[i]);
        free(column[i] -> memberArray);
        free(column[i]);
    }

    free(boxGrid);
    free(row);
    free(column);
}

void possibilityCleanup(Cell*** sudokuGrid, Box** boxGrid, Axis** rows, Axis** columns) {
    for (int i = 0; i < 9; i++) {
        for (int k = 0; k < 9; k++) {
            if (sudokuGrid[i][k] -> value > -1) {
                sudokuGrid[i][k] -> possibility = 0;
                continue;
            }

            /*
                We have 9 bits representing the remaining possible value for a cell
                And 9 bits representing the already claimed value in a row/col/box
                Therefore:
                0 - 0 -> 0
                1 - 0 -> 1
                0 - 1 -> 0
                1 - 1 -> 0
                This operation is A and not B (A & (~B))
            */
            sudokuGrid[i][k] -> possibility &= ~(boxGrid[(i/3)*3 + (k/3)] -> impossibleValue);
            sudokuGrid[i][k] -> possibility &= ~(rows[i] -> impossibleValue);
            sudokuGrid[i][k] -> possibility &= ~(columns[k] -> impossibleValue);
        }
    }
}

int generateBoardChecksum(Cell *** sudokuGrid) {
    int checksum = 0;

    for (int i = 0; i < 9; i++) {
        for (int k = 0; k < 9; k++) {
            checksum += sudokuGrid[i][k] -> possibility;
        }
    }

    return checksum;
}

int countOneBits(int bitStream) {
    int bitcount = 0;

    while (bitStream != 0) {
        bitcount += bitStream % 2;
        bitStream /= 2;
    }

    return bitcount;
}

int checkSingularBit(Cell*** sudokuGrid, Box** boxGrid, Axis** rows, Axis** columns, int bits, int i, int k) {
    /*
        Given a binary value, is it possible to check if it only have a single bit being 1?
        0b001000000
        not give 0b110111111
        +1 give 0b111000000
        and the first and this give 0b001000000 which is the same
        so (x == (x & (~x + 1)))?

        BUG: this yield true for 0b000000000, but to have all 0 for possible value
        means something has gone REALLY WRONG before this

        Then count trailing 0 to get which bit is 1
        shift left one by one until it is 0!
    */

    if (bits == 0) return 0;

    if (bits == (bits & (~bits + 1))) {
        int val = -1;
        while (bits != 0) {
            val++;
            bits >>= 1;
        }

        sudokuGrid[i][k] -> value = val;
        boxGrid[(i/3)*3 + (k/3)] -> impossibleValue += TWO_POWER_LOOKUP[val];
        rows[i] -> impossibleValue += TWO_POWER_LOOKUP[val];
        columns[k] -> impossibleValue += TWO_POWER_LOOKUP[val];
        return 1;
    }

    return 0;
}

int checkSolvedCells(Cell*** sudokuGrid, Box** boxGrid, Axis** rows, Axis** columns) {
    int solvedCellCount = 0;

    for (int i = 0; i < 9; i++) {
        for (int k = 0; k < 9; k++) {
            if (sudokuGrid[i][k] -> value > -1) continue;
            
            int temp = sudokuGrid[i][k] -> possibility;

            if (checkSingularBit(sudokuGrid, boxGrid, rows, columns, temp, i, k)) {
                solvedCellCount++;
                continue;
            }
        }
    }
    
    return solvedCellCount;
}

Cell* createCell() {
    Cell* cell = malloc(sizeof(Cell));

    cell -> value = -1;
    cell -> possibility = BIT_MASK;
    cell -> fixed = 0;

    return cell;
}

Cell*** createSudokuGrid() {
    Cell*** sudokuGrid = malloc(sizeof(Cell**)*9);

    for (int i = 0; i < 9; i++) {
        sudokuGrid[i] = malloc(sizeof(Cell*)*9);
        
        for (int k = 0; k < 9; k++) {
            sudokuGrid[i][k] = createCell();
        }
    }

    return sudokuGrid;
}

Box** createBoxGrid(Cell *** sudokuGrid) {
    Box** boxGrid = malloc(sizeof(Box*) * 9);

    for (int i = 0; i < 9; i++) {
        boxGrid[i] = malloc(sizeof(Box));
        boxGrid[i] -> memberArray = malloc(sizeof(Cell*) * 9);
        
        for (int k = 0; k < 3; k++) {
            for (int l = 0; l < 3; l++) {
                // ffs
                boxGrid[i] -> memberArray[k*3 + l] = sudokuGrid[BOX_LOOKUP[i][0]+k][BOX_LOOKUP[i][1] + l];
            }
        }

        boxGrid[i] -> impossibleValue = 0;
    }

    return boxGrid;
}

Axis** createAxis(Cell*** sudokuGrid, int direction) {
    Axis** axis = malloc(sizeof(Axis*) * 9);

    if (direction == 0) {
        for (int i = 0; i < 9; i++) {
            axis[i] = malloc(sizeof(Axis));
            axis[i] -> memberArray = malloc(sizeof(Cell*) * 9);

            for (int k = 0; k < 9; k++) {
                axis[i] -> memberArray[k] = sudokuGrid[i][k];
            }
            
            axis[i] -> impossibleValue = 0;
        }
    }
    else {
        for (int i = 0; i < 9; i++) {
            axis[i] = malloc(sizeof(Axis));
            axis[i] -> memberArray = malloc(sizeof(Cell*) * 9);

            for (int k = 0; k < 9; k++) {
                axis[i] -> memberArray[k] = sudokuGrid[k][i];
            }
            
            axis[i] -> impossibleValue = 0;
        }
    }

    return axis;
}