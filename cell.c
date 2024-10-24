#include "cell.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>  // Para usar la función round()

#define INITIAL_CONNECTION_SIZE 10

// Tablas globales
Connection **connection_table = NULL;
int num_connections = 0;
Cell **cell_table = NULL;
int num_cells = 0;
Automaton **automaton_table = NULL;
int num_automata = 0;

// Variables globales para las tasas
double beta = 0.3;   // Tasa de infección (S->E)
double sigma = 0.1;  // Tasa de exposición (E->I)
double gamma_ = 0.05; // Tasa de recuperación (I->R)
double mu = 0.02;     // Tasa de mortalidad (I->Removidos)
int allow_movement = 1; // Por defecto, se permite el movimiento

// Crear un caso con coordenadas y estados iniciales
Case *create_case(int x, int y, int initial_S, int initial_E, int initial_I, int initial_R)
{
    Case *new_case = (Case *)malloc(sizeof(Case));
    new_case->x = x;
    new_case->y = y;
    new_case->S = initial_S;
    new_case->E = initial_E;
    new_case->I = initial_I;
    new_case->R = initial_R;
    new_case->neighbors = NULL;
    new_case->num_neighbors = 0;
    return new_case;
}

// Crear una celda con dimensiones y estados iniciales para los casos
Cell *create_cell(char *name, int height, int width, int initial_S, int initial_E, int initial_I, int initial_R)
{
    Cell *new_cell = (Cell *)malloc(sizeof(Cell));
    new_cell->name = strdup(name);
    new_cell->height = height;
    new_cell->width = width;

    // Asignar memoria para los casos
    new_cell->cases = (Case **)malloc(height * width * sizeof(Case *));

    // Inicializar cada caso con las coordenadas y los estados iniciales
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            new_cell->cases[i * width + j] = create_case(i, j, initial_S, initial_E, initial_I, initial_R);
        }
    }

    // Asignar vecinos a los casos
    assign_neighbors(new_cell);

    // Agregar la celda a la tabla global de celdas
    add_cell(new_cell);

    return new_cell;
}

// Asignar vecinos a cada caso dentro de una celda (vecindad de Moore)
void assign_neighbors(Cell *cell)
{
    for (int i = 0; i < cell->height; i++)
    {
        for (int j = 0; j < cell->width; j++)
        {
            Case *current_case = cell->cases[i * cell->width + j];
            current_case->neighbors = (Case **)malloc(8 * sizeof(Case *));
            current_case->num_neighbors = 0;

            // Vecindad de Moore
            for (int dx = -1; dx <= 1; dx++)
            {
                for (int dy = -1; dy <= 1; dy++)
                {
                    if (dx == 0 && dy == 0)
                        continue; // Saltar la celda actual

                    int nx = i + dx;
                    int ny = j + dy;

                    // Verificar si las coordenadas están dentro de la celda
                    if (nx >= 0 && nx < cell->height && ny >= 0 && ny < cell->width)
                    {
                        Case *neighbor_case = cell->cases[nx * cell->width + ny];
                        current_case->neighbors[current_case->num_neighbors++] = neighbor_case;
                    }
                }
            }
        }
    }
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

// Conectar dos celdas y agregar los casos como vecinos mutuos
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

    // Agregar los casos como vecinos mutuos
    case1->neighbors = (Case **)realloc(case1->neighbors, (case1->num_neighbors + 1) * sizeof(Case *));
    case1->neighbors[case1->num_neighbors++] = case2;

    case2->neighbors = (Case **)realloc(case2->neighbors, (case2->num_neighbors + 1) * sizeof(Case *));
    case2->neighbors[case2->num_neighbors++] = case1;

    // Registrar la conexión en la tabla de conexiones
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
                Case *current_case = cell_table[i]->cases[j * cell_table[i]->width + k];
                printf("  Case (%d, %d) -> S=%.2f, E=%.2f, I=%.2f, R=%.2f\n",
                       current_case->x, current_case->y,
                       current_case->S, current_case->E, current_case->I, current_case->R);
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

// Imprimir el estado de una celda específica
void print_cell_state(char *cell_name)
{
    Cell *cell = find_cell(cell_name);
    if (cell == NULL)
    {
        printf("Cell %s not found.\n", cell_name);
        return;
    }

    printf("State of Cell %s:\n", cell->name);
    for (int j = 0; j < cell->height; j++)
    {
        for (int k = 0; k < cell->width; k++)
        {
            Case *current_case = cell->cases[j * cell->width + k];
            printf("  Case (%d, %d) -> S=%.2f, E=%.2f, I=%.2f, R=%.2f\n",
                   current_case->x, current_case->y,
                   current_case->S, current_case->E, current_case->I, current_case->R);
        }
    }
}

// Imprimir el estado de un autómata específico
void print_automaton_state(char *automaton_name)
{
    Automaton *automaton = find_automaton(automaton_name);
    if (automaton == NULL)
    {
        printf("Automaton %s not found.\n", automaton_name);
        return;
    }

    printf("State of Automaton %s:\n", automaton->name);
    for (int i = 0; i < automaton->num_cells; i++)
    {
        print_cell_state(automaton->cells[i]->name);
    }
}

// Imprimir el estado de todas las celdas y autómatas
void print_all_states()
{
    printf("State of all Automata and Cells:\n");
    for (int i = 0; i < num_automata; i++)
    {
        print_automaton_state(automaton_table[i]->name);
    }
}

// Establecer nuevas tasas
void set_rates(double new_beta, double new_sigma, double new_gamma, double new_mu)
{
    beta = new_beta;
    sigma = new_sigma;
    gamma_ = new_gamma;
    mu = new_mu;
}

// Simulación del modelo SEIR
void simulate(int steps, int allow_movement)
{
    for (int step = 0; step < steps; step++)
    {
        printf("Step %d:\n", step + 1);

        // Crear copias de los estados actuales para actualizar simultáneamente
        for (int i = 0; i < num_cells; i++)
        {
            Cell *cell = cell_table[i];
            int num_cases = cell->height * cell->width;

            // Mapeo de casos antiguos a nuevos
            Case **new_cases = (Case **)malloc(num_cases * sizeof(Case *));
            Case **old_to_new_case_map = (Case **)malloc(num_cases * sizeof(Case *));

            // Crear los nuevos casos y mapearlos
            for (int idx = 0; idx < num_cases; idx++)
            {
                Case *current_case = cell->cases[idx];
                Case *new_case = (Case *)malloc(sizeof(Case));
                new_case->x = current_case->x;
                new_case->y = current_case->y;
                new_case->S = current_case->S;
                new_case->E = current_case->E;
                new_case->I = current_case->I;
                new_case->R = current_case->R;
                new_case->num_neighbors = current_case->num_neighbors;
                new_case->neighbors = NULL; // Lo actualizaremos después
                new_cases[idx] = new_case;
                old_to_new_case_map[idx] = new_case;
            }

            // Actualizar los punteros a vecinos en los nuevos casos
            for (int idx = 0; idx < num_cases; idx++)
            {
                Case *current_case = cell->cases[idx];
                Case *new_case = new_cases[idx];
                new_case->neighbors = (Case **)malloc(new_case->num_neighbors * sizeof(Case *));
                for (int n = 0; n < current_case->num_neighbors; n++)
                {
                    Case *neighbor_old_case = current_case->neighbors[n];

                    // Encontrar el índice del vecino
                    int neighbor_index = -1;
                    for (int search_idx = 0; search_idx < num_cases; search_idx++)
                    {
                        if (cell->cases[search_idx] == neighbor_old_case)
                        {
                            neighbor_index = search_idx;
                            break;
                        }
                    }
                    if (neighbor_index != -1)
                    {
                        new_case->neighbors[n] = old_to_new_case_map[neighbor_index];
                    }
                    else
                    {
                        // Si no se encuentra, puede ser un vecino externo (otra celda)
                        // Aquí asumimos que las conexiones externas permanecen válidas
                        new_case->neighbors[n] = neighbor_old_case;
                    }
                }
            }

            // Actualizar los estados
            for (int idx = 0; idx < num_cases; idx++)
            {
                Case *current_case = cell->cases[idx];
                Case *new_case = new_cases[idx];

                // Cálculo de la población total
                double N = current_case->S + current_case->E + current_case->I + current_case->R;
                if (N == 0) N = 1; // Evitar división por cero

                // Transiciones internas en la celda
                double new_exposed = beta * current_case->S * current_case->I / N;
                double new_infected = sigma * current_case->E;
                double new_recovered = gamma_ * current_case->I;
                double new_deaths = mu * current_case->I;

                // Actualizar estados con precisión decimal
                new_case->S -= new_exposed;
                new_case->E += new_exposed - new_infected;
                new_case->I += new_infected - new_recovered - new_deaths;
                new_case->R += new_recovered + new_deaths;

                // Asegurar que los valores no sean negativos
                if (new_case->S < 0) new_case->S = 0;
                if (new_case->E < 0) new_case->E = 0;
                if (new_case->I < 0) new_case->I = 0;
                if (new_case->R < 0) new_case->R = 0;

                // Movimiento entre celdas
                if (allow_movement)
                {
                    // Implementar lógica de movimiento (ejemplo simple)
                    double moving_infected = new_case->I * 0.01; // 1% se mueve
                    new_case->I -= moving_infected;

                    // Distribuir entre vecinos
                    int num_neighbors = new_case->num_neighbors;
                    if (num_neighbors > 0)
                    {
                        double infected_per_neighbor = moving_infected / num_neighbors;
                        for (int n = 0; n < num_neighbors; n++)
                        {
                            Case *neighbor_case = new_case->neighbors[n];
                            neighbor_case->I += infected_per_neighbor;

                            printf("Moved %.2f infected individuals from Cell %s (%d,%d) to neighbor (%d,%d)\n",
                                   infected_per_neighbor, cell->name, new_case->x, new_case->y,
                                   neighbor_case->x, neighbor_case->y);
                        }
                    }
                }

                // Redondear los valores finales para mostrar
                int S_int = (int)round(new_case->S);
                int E_int = (int)round(new_case->E);
                int I_int = (int)round(new_case->I);
                int R_int = (int)round(new_case->R);

                // Actualizar los valores de new_case con los valores enteros
                new_case->S = S_int;
                new_case->E = E_int;
                new_case->I = I_int;
                new_case->R = R_int;

                // Imprimir el estado actual de la celda
                printf("Cell %s (%d,%d): S=%d, E=%d, I=%d, R=%d\n",
                       cell->name, new_case->x, new_case->y,
                       S_int, E_int, I_int, R_int);
            }

            // Reemplazar los casos antiguos con los nuevos
            for (int idx = 0; idx < num_cases; idx++)
            {
                free(cell->cases[idx]);
                cell->cases[idx] = new_cases[idx];
            }
            free(new_cases);
            free(old_to_new_case_map);
        }
    }
}
