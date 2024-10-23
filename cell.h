#ifndef CELL_H
#define CELL_H

#include <stdio.h>
#include <stdlib.h>

// Definition of the structure for a Case
typedef struct
{
    int x, y;    // Coordinates of the case
    char *state; // State of the case (e.g., "susceptible", "infected", "recovered")
} Case;

// Definition of the structure for a Cell, composed of multiple cases
typedef struct
{
    char *name;   // Name of the cell
    Case **cases; // Array of pointers to the cases
    int height;   // Height of the cell (number of rows)
    int width;    // Width of the cell (number of columns)
} Cell;

// Definition of the structure for a Connection between two cases from different cells
typedef struct
{
    Cell *cell1; // Pointer to the first cell
    Case *case1; // Pointer to a case in the first cell
    Cell *cell2; // Pointer to the second cell
    Case *case2; // Pointer to a case in the second cell
} Connection;

// Global connection table
extern Connection **connection_table;
extern int num_connections;

// Global cell table
extern Cell **cell_table;
extern int num_cells;

// Associated functions

// Function to create a cell with a given dimension (height, width)
Cell *create_cell(char *name, int height, int width, char *initial_state);

// Function to initialize a case with coordinates and a state
Case *create_case(int x, int y, char *state);

// Function to connect two cases from different cells
void connect_cells(char *cell1_name, int x1, int y1, char *cell2_name, int x2, int y2);

// Function to initialize the connection table with a given size
void initialize_connection_table(int initial_size);

// Function to resize the connection table if necessary
void resize_connection_table(int new_size);

// Function to add a cell to the global cell list
void add_cell(Cell *new_cell);

// Function to print all cells and their cases
void print_cells();

// Function to print all connections between cases
void print_connections();

Cell *find_cell(char *name);

#endif // CELL_H
