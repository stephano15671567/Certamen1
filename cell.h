#ifndef CELL_H
#define CELL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>  // Para manejar strcmp y strdup

// Estructura para un "Case"
typedef struct Case
{
    int x, y;    // Coordenadas
    double S;    // Susceptibles
    double E;    // Expuestos
    double I;    // Infectados
    double R;    // Recuperados
    struct Case **neighbors; // Vecinos
    int num_neighbors;       // Número de vecinos
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
    Cell *cell1;  // Primera celda
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

// Variables globales para las tasas
extern double beta;   // Tasa de infección (S->E)
extern double sigma;  // Tasa de exposición (E->I)
extern double gamma_; // Tasa de recuperación (I->R)
extern double mu;     // Tasa de mortalidad (I->Removidos)
extern int allow_movement; // Control de movimiento

// Funciones declaradas
Cell *create_cell(char *name, int height, int width, int initial_S, int initial_E, int initial_I, int initial_R);
Case *create_case(int x, int y, int initial_S, int initial_E, int initial_I, int initial_R);
void assign_neighbors(Cell *cell);
void connect_cells(char *cell1_name, int x1, int y1, char *cell2_name, int x2, int y2);
void connect_cells_ac(char *automaton1_name, char *cell1_name, int x1, int y1,
                      char *automaton2_name, char *cell2_name, int x2, int y2);
void add_cell(Cell *new_cell);
void add_cell_to_automaton(Automaton *automaton, Cell *new_cell);
Automaton *create_automaton(char *name);
void add_automaton(Automaton *new_automaton);
void print_cells();
void print_connections();
void print_cell_state(char *cell_name);
void print_automaton_state(char *automaton_name);
void print_all_states();
Cell *find_cell(char *name);
Automaton *find_automaton(char *name);
Cell *find_cell_in_automaton(Automaton *automaton, char *cell_name);
void initialize_connection_table();
void set_rates(double new_beta, double new_sigma, double new_gamma, double new_mu);
void simulate(int steps, int allow_movement);

#endif // CELL_H
