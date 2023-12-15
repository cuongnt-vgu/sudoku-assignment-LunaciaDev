#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wchar.h>

#include "../src/auxilary.h"
#include "../src/sudoku.h"
#include "../src/hiddenSingle.h"
#include "../src/hiddenPair.h"
#include "../src/hiddenTriple.h"
#include "../src/nakedPair.h"
#include "../src/nakedTriple.h"

// Verbosity of test runner. Overridden via compilation flag
#ifdef VERBOSE
#define VERBOSE 1
#endif
#ifndef VERBOSE
#define VERBOSE 0
#endif

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

const int BOARD_SIZE = 9;

char mapping[32] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v'};

int get_index(char *array, int len, char input)
{
    for (int i = 0; i < len; i++)
    {
        if (array[i] == input)
            return i;
    }
    return -1;
}

void toBinary(int n, int *bin_array)
{
    int id = 0;
    while (n)
    {
        bin_array[id++] = n % 2;
        n = n / 2;
    }
}

int toInteger(int *bin_array, int len)
{
    int num = 0;
    for (int i = 0; i < len; i++)
    {
        num += bin_array[i] * (1 << i);
    }
    return num;
}

void load_cell_candidates(Cell *p_cell, char *textData)
{
    int left_num = get_index(mapping, 32, textData[0]);
    int right_num = get_index(mapping, 32, textData[1]);
    int bin_candidates[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    int candidate = 0;

    toBinary(right_num, &(bin_candidates[0]));
    toBinary(left_num, &(bin_candidates[5]));

    for (int i = 1; i < 10; i++) {
        candidate += bin_candidates[i] == 1 ? TWO_POWER_LOOKUP[i-1] : 0;
    }

    p_cell -> possibility = candidate;
    if (bin_candidates[0] == 1) p_cell -> fixed = 1; 
}

void load_sudoku_with_candidates(Cell*** sudokuBoard, char *textData)
{
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int k = 0; k < BOARD_SIZE; k++) {
            load_cell_candidates(sudokuBoard[i][k], textData);
            textData += 2;
        }
    }
}

void print_string_candidates(Cell *p_cell, char *textData)
{
    int candidates = p_cell -> possibility;

    int bin_candidates[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    if (p_cell->fixed)
        bin_candidates[0] = 1;

    if (p_cell -> value == -1) {
        for (int i = 1; i < 10; i++) {
            bin_candidates[i] = candidates % 2;
            candidates = candidates >> 1;
        }
    }
    else {
        bin_candidates[p_cell -> value + 1] = 1;
    }

    int left_index = toInteger(&(bin_candidates[5]), 5);
    int right_index = toInteger(&(bin_candidates[0]), 5);
    sprintf(textData, "%c%c", mapping[left_index], mapping[right_index]);
}

void print_sudoku_with_candidates(Cell*** p_board, char *textData)
{
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int k = 0; k < BOARD_SIZE; k++) {
            print_string_candidates(p_board[i][k], textData);
            textData += 2;
        }
    }
}

typedef int (*method)(Cell*** sudokuGrid, Axis** boxGrid, Axis** rows, Axis** columns);

method get_method(char *method_name)
{
    if (strcmp(method_name, "hidden_singles") == 0)
        return checkHiddenSingles;
    else if (strcmp(method_name, "naked_pairs") == 0)
        return checkNakedPair;
    else if (strcmp(method_name, "hidden_pairs") == 0)
        return checkHiddenPair;
    else if (strcmp(method_name, "naked_triples") == 0)
        return checkNakedTriple;
    else if (strcmp(method_name, "hidden_triples") == 0)
        return checkHiddenTriple;
    else
        return NULL;
}

int main(int argc, char **argv)
{
    if (argc < 4)
    {
        printf("Usage: %s <sudoku string> <method> <pipe>\n\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    Cell*** sudokuGrid = createSudokuGrid();
    Axis** boxGrid = createBoxGrid(sudokuGrid);
    Axis** rows = createAxis(sudokuGrid, 0);
    Axis** columns = createAxis(sudokuGrid, 1);
    
    load_sudoku_with_candidates(sudokuGrid, argv[1]);

    checkSolvedCells(sudokuGrid, boxGrid, rows, columns);
    possibilityCleanup(sudokuGrid, boxGrid, rows, columns);

    FILE *pipe = fdopen(atoi(argv[3]), "w");

    int num_detected = get_method(argv[2])(sudokuGrid, boxGrid, rows, columns);

    char *outText = malloc(BOARD_SIZE * BOARD_SIZE * 2 + 1);
    outText[0] = '\0';

    print_sudoku_with_candidates(sudokuGrid, outText);

    fprintf(pipe,
            "{\n"
            "    \"num_detects\": %d,\n"
            "    \"boards\": \"%s\"\n"
            "}\n",
            num_detected, outText);

    cleanUp(sudokuGrid, boxGrid, rows, columns);

    fclose(pipe);
    exit(EXIT_SUCCESS);
}