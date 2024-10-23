#ifndef CELL_H
#define CELL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>  // Para manejar strcmp y strdup

// Estructura para un "Case"
typedef struct
{
    int x, y;    // Coordenadas
    char *state; // Estado (p.ej. "susceptible", "infected", etc.)
} Case;

// Estructura para una "Cell"
typedef struct
{
    char *name;   // Nombre de la celda
    Case **cases; // Array de casos
    int height;   // Altura (número de filas)
    int width;    // Anchura (número de columnas)
} Cell;

// Estructura para una "Connection" entre dos celdas
typedef struct
{
    Cell *cell1;  // Primer celda
    Case *case1;  // Primer caso
    Cell *cell2;  // Segunda celda
    Case *case2;  // Segundo caso
} Connection;

// Estructura para un "Automaton"
typedef struct
{
    char *name;   // Nombre del autómata
    Cell **cells; // Array de celdas
    int num_cells; // Número de celdas
} Automaton;

// Tablas globales
extern Connection **connection_table;
extern int num_connections;
extern Cell **cell_table;
extern int num_cells;
extern Automaton **automaton_table;
extern int num_automata;

// Funciones declaradas
Cell *create_cell(char *name, int height, int width, char *initial_state);
Case *create_case(int x, int y, char *state);
void connect_cells(char *cell1_name, int x1, int y1, char *cell2_name, int x2, int y2);
void connect_cells_ac(char *automaton1_name, char *cell1_name, int x1, int y1,
                      char *automaton2_name, char *cell2_name, int x2, int y2);
void add_cell(Cell *new_cell);
void add_cell_to_automaton(Automaton *automaton, Cell *new_cell);
Automaton *create_automaton(char *name);
void add_automaton(Automaton *new_automaton);
void print_cells();
void print_connections();
Cell *find_cell(char *name);
Automaton *find_automaton(char *name);
Cell *find_cell_in_automaton(Automaton *automaton, char *cell_name);
void initialize_connection_table();

#endif // CELL_H
