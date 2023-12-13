#include "sudoku.h"

#include "auxilary.h"

#include "hiddenSingle.h"
#include "hiddenPairTriple.h"
#include "nakedPairTriple.h"

#include <stdio.h>
#include <stdlib.h>

const int TWO_POWER_LOOKUP[9] = {
    1, 
    2, 
    4, 
    8, 
    16, 
    32, 
    64, 
    128, 
    256
};

const int BOX_LOOKUP[9][2] = {
    {0, 0},
    {0, 3},
    {0, 6},
    {3, 0},
    {3, 3},
    {3, 6},
    {6, 0},
    {6, 3},
    {6, 6}
};

const int BIT_MASK = 0x1ff;

Cell* createCell() {
    Cell* cell = malloc(sizeof(Cell));

    cell -> value = -1;
    cell -> possibility = BIT_MASK;

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

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("%s", "Usage: main <sudoku>\n");
        return 1;
    }

    Cell*** sudokuGrid = createSudokuGrid();
    Box** boxGrid = createBoxGrid(sudokuGrid);
    Axis** rows = createAxis(sudokuGrid, 0);
    Axis** columns = createAxis(sudokuGrid, 1);

    int temp;
    char* inputSudoku = argv[1];
    int solvedCell = 0;

    for (int i = 0; i < 9; i++) {
        for (int k = 0; k < 9; k++) {
            
            temp = inputSudoku[9*i + k] - 49;

            if (temp > -1) {
                solvedCell++;
                boxGrid[(i/3)*3 + (k/3)] -> impossibleValue += TWO_POWER_LOOKUP[temp];
                rows[i] -> impossibleValue += TWO_POWER_LOOKUP[temp];
                columns[k] -> impossibleValue += TWO_POWER_LOOKUP[temp];
                sudokuGrid[i][k] -> possibility = 0;
            }

            sudokuGrid[i][k] -> value = temp;
        }
    }
    
    while (solvedCell < 81) {
        possibilityCleanup(sudokuGrid, boxGrid, rows, columns);

        temp = checkSolvedCells(sudokuGrid, boxGrid, rows, columns);
        if (temp != 0) {
            solvedCell += temp;
            continue;
        }

        temp = checkHiddenSingles(sudokuGrid, boxGrid, rows, columns);
        if (temp != 0) {
            solvedCell += temp;
            continue;
        }

        temp = checkNakedPairTriple(sudokuGrid, boxGrid, rows, columns);
        if (temp != 0) {
            continue;
        }

        temp = checkHiddenPairTriple(sudokuGrid, boxGrid, rows, columns);
        if (temp != 0) {
            continue;
        }

        printf("Grid cannot be solved with current toolset!\nCurrently stuck in the following position:\n");
        solvedCell = 81;
    }

    printSudokuGrid(sudokuGrid);
    cleanUp(sudokuGrid, boxGrid, rows, columns);
}