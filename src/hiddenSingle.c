#include "hiddenSingle.h"

int checkHiddenSingles(Cell*** sudokuGrid, Axis** boxGrid, Axis** rows, Axis** columns) {
    int solvedCellCount = 0;

    for (int i = 0; i < 9; i++) {
        for (int k = 0; k < 9; k++) {
            if (sudokuGrid[i][k] -> value != -1) continue;
            
            /*
                For each cell, we OR all possible values of other cell on same row/col/box
                NOT the result, then AND with the possible values of current cell
                That will filter out all possibility the cell and the row/col/box have in commn
                Check if there's only one bit, and voila!
            */
            int row = 0, col = 0, box = 0;
            int temp = (i/3)*3 + (k/3);

            for (int l = 0; l < 9; l++) {
                row |= l == k ? 0 : rows[i] -> memberArray[l] -> possibility;
                col |= l == i ? 0 : columns[k] -> memberArray[l] -> possibility;
                box |= l == (i - BOX_LOOKUP[temp][0])*3 + k - BOX_LOOKUP[temp][1] ? 0 : boxGrid[temp] -> memberArray[l] -> possibility;
            }

            row = ~row & BIT_MASK; // keeping only last 9 bits, zero out the rest
            col = ~col & BIT_MASK;
            box = ~box & BIT_MASK;

            temp = col & sudokuGrid[i][k] -> possibility;

            if (checkSingularBit(sudokuGrid, boxGrid, rows, columns, temp, i, k)) {
                solvedCellCount++;
                continue;
            }

            temp = row & sudokuGrid[i][k] -> possibility;
            
            if (checkSingularBit(sudokuGrid, boxGrid, rows, columns, temp, i, k)) {
                solvedCellCount++;
                continue;
            }

            temp = box & sudokuGrid[i][k] -> possibility;
            
            if (checkSingularBit(sudokuGrid, boxGrid, rows, columns, temp, i, k)) {
                solvedCellCount++;
                continue;
            }
        }
    }
    
    return solvedCellCount;
}