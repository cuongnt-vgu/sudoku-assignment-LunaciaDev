#pragma once

#include "sudoku.h"
#include <stdio.h>
#include <stdlib.h>

void printSudokuGrid(Cell*** sudokuGrid);
void cleanUp(Cell*** sudokuGrid, Box** boxGrid, Axis** row, Axis** column);
void possibilityCleanup(Cell*** sudokuGrid, Box** boxGrid, Axis** rows, Axis** columns);
int generateBoardChecksum(Cell *** sudokuGrid);
int countOneBits(int bitStream);
int checkSingularBit(Cell*** sudokuGrid, Box** boxGrid, Axis** rows, Axis** columns, int bits, int i, int k);
int checkSolvedCells(Cell*** sudokuGrid, Box** boxGrid, Axis** rows, Axis** columns);