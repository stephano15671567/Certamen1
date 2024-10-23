#include "cell.h"
#include <string.h> 
#include <stdlib.h> 

#define INITIAL_CONNECTION_SIZE 10

// Tablas globales
Connection **connection_table = NULL;
int num_connections = 0;
Cell **cell_table = NULL;
int num_cells = 0;
Automaton **automaton_table = NULL;
int num_automata = 0;

// Crear un caso con coordenadas y estado
Case *create_case(int x, int y, char *state)
{
    Case *new_case = (Case *)malloc(sizeof(Case));
    new_case->x = x;
    new_case->y = y;
    new_case->state = strdup(state); // Duplicar el string del estado
    return new_case;
}

// Crear una celda con dimensiones y estado inicial para todos los casos
Cell *create_cell(char *name, int height, int width, char *initial_state)
{
    Cell *new_cell = (Cell *)malloc(sizeof(Cell));
    new_cell->name = strdup(name);  
    new_cell->height = height;
    new_cell->width = width;

    // Asignar memoria para los casos
    new_cell->cases = (Case **)malloc(height * width * sizeof(Case *));

    // Inicializar cada caso con las coordenadas y el estado inicial
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            new_cell->cases[i * width + j] = create_case(i, j, initial_state);
        }
    }

    // Agregar la celda a la tabla global de celdas
    add_cell(new_cell);

    return new_cell;
}

// Agregar una celda a la tabla global
void add_cell(Cell *new_cell)
{
    num_cells++;
    cell_table = (Cell **)realloc(cell_table, num_cells * sizeof(Cell *));
    cell_table[num_cells - 1] = new_cell;
}

// Crear un autómata
Automaton *create_automaton(char *name)
{
    Automaton *new_automaton = (Automaton *)malloc(sizeof(Automaton));
    new_automaton->name = strdup(name);  
    new_automaton->cells = NULL;
    new_automaton->num_cells = 0;

    // Agregar el autómata a la tabla global
    add_automaton(new_automaton);

    return new_automaton;
}

// Agregar una celda a un autómata
void add_cell_to_automaton(Automaton *automaton, Cell *new_cell)
{
    automaton->num_cells++;
    automaton->cells = (Cell **)realloc(automaton->cells, automaton->num_cells * sizeof(Cell *));
    automaton->cells[automaton->num_cells - 1] = new_cell;
}

// Agregar un autómata a la tabla global
void add_automaton(Automaton *new_automaton)
{
    num_automata++;
    automaton_table = (Automaton **)realloc(automaton_table, num_automata * sizeof(Automaton *));
    automaton_table[num_automata - 1] = new_automaton;
}

// Inicializar la tabla de conexiones
void initialize_connection_table()
{
    if (connection_table == NULL)
    {
        connection_table = (Connection **)malloc(INITIAL_CONNECTION_SIZE * sizeof(Connection *));
        num_connections = 0;
    }
}

// Conectar celdas entre autómatas
void connect_cells_ac(char *automaton1_name, char *cell1_name, int x1, int y1,
                      char *automaton2_name, char *cell2_name, int x2, int y2)
{
    Automaton *automaton1 = find_automaton(automaton1_name);
    Automaton *automaton2 = find_automaton(automaton2_name);

    if (automaton1 == NULL || automaton2 == NULL)
    {
        printf("Error: One or both automata not found.\n");
        return;
    }

    Cell *cell1 = find_cell_in_automaton(automaton1, cell1_name);
    Cell *cell2 = find_cell_in_automaton(automaton2, cell2_name);

    if (cell1 == NULL || cell2 == NULL)
    {
        printf("Error: One or both cells not found.\n");
        return;
    }

    connect_cells(cell1->name, x1, y1, cell2->name, x2, y2);
}

// Conectar dos celdas
void connect_cells(char *cell1_name, int x1, int y1, char *cell2_name, int x2, int y2)
{
    initialize_connection_table();

    Cell *cell1 = find_cell(cell1_name);
    Cell *cell2 = find_cell(cell2_name);

    if (cell1 == NULL || cell2 == NULL)
    {
        printf("Error: One or both cells not found.\n");
        return;
    }

    Case *case1 = cell1->cases[x1 * cell1->width + y1];
    Case *case2 = cell2->cases[x2 * cell2->width + y2];

    if (case1 == NULL || case2 == NULL)
    {
        printf("Error: One or both cases could not be found.\n");
        return;
    }

    Connection *new_connection = (Connection *)malloc(sizeof(Connection));
    new_connection->cell1 = cell1;
    new_connection->case1 = case1;
    new_connection->cell2 = cell2;
    new_connection->case2 = case2;

    connection_table[num_connections] = new_connection;
    num_connections++;

    printf("Cases (%d, %d) from %s and (%d, %d) from %s are now connected.\n",
           x1, y1, cell1->name, x2, y2, cell2->name);
}

// Encontrar una celda por nombre
Cell *find_cell(char *name)
{
    for (int i = 0; i < num_cells; i++)
    {
        if (strcmp(cell_table[i]->name, name) == 0)
        {
            return cell_table[i];
        }
    }
    return NULL; 
}

// Encontrar un autómata por nombre
Automaton *find_automaton(char *name)
{
    for (int i = 0; i < num_automata; i++)
    {
        if (strcmp(automaton_table[i]->name, name) == 0)
        {
            return automaton_table[i];
        }
    }
    return NULL;
}

// Encontrar una celda dentro de un autómata
Cell *find_cell_in_automaton(Automaton *automaton, char *cell_name)
{
    for (int i = 0; i < automaton->num_cells; i++)
    {
        if (strcmp(automaton->cells[i]->name, cell_name) == 0)
        {
            return automaton->cells[i];
        }
    }
    return NULL; 
}

// Imprimir todas las celdas y sus casos
void print_cells()
{
    printf("List of cells:\n");
    for (int i = 0; i < num_cells; i++)
    {
        printf("Cell %s: %d x %d\n", cell_table[i]->name, cell_table[i]->height, cell_table[i]->width);
        for (int j = 0; j < cell_table[i]->height; j++)
        {
            for (int k = 0; k < cell_table[i]->width; k++)
            {
                printf("  Case (%d, %d) -> State: %s\n", j, k, cell_table[i]->cases[j * cell_table[i]->width + k]->state);
            }
        }
    }
}

// Imprimir todas las conexiones
void print_connections()
{
    printf("List of connections:\n");
    for (int i = 0; i < num_connections; i++)
    {
        Connection *conn = connection_table[i];
        printf("  Connection between Cell %s (%d, %d) and Cell %s (%d, %d)\n",
               conn->cell1->name, conn->case1->x, conn->case1->y,
               conn->cell2->name, conn->case2->x, conn->case2->y);
    }
}
