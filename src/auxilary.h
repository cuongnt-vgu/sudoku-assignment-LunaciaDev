#pragma once

#include "sudoku.h"

#include <stdlib.h>
#include <stdio.h>

void printSudokuGrid(Cell*** sudokuGrid);

void cleanUp(Cell*** sudokuGrid, Axis** boxGrid, Axis** row, Axis** column);

void possibilityCleanup(Cell*** sudokuGrid, Axis** boxGrid, Axis** rows, Axis** columns);

void clearAllMarker(Cell*** sudokuGrid);

int generateBoardChecksum(Cell *** sudokuGrid);

int countOneBits(int bitStream);

int checkSingularBit(Cell*** sudokuGrid, Axis** boxGrid, Axis** rows, Axis** columns, int bits, int i, int k);

int checkSolvedCells(Cell*** sudokuGrid, Axis** boxGrid, Axis** rows, Axis** columns);

Cell* createCell();

Cell*** createSudokuGrid();

Axis** createBoxGrid(Cell *** sudokuGrid);

Axis** createAxis(Cell*** sudokuGrid, int direction);