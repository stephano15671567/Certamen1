#include "cell.h"
#include <string.h> // For strcmp

// Global connection table
Connection **connection_table = NULL;
int num_connections = 0;

// Global cell table
Cell **cell_table = NULL;
int num_cells = 0;

// Function to create a case with coordinates and a state (string)
Case *create_case(int x, int y, char *state)
{
    Case *new_case = (Case *)malloc(sizeof(Case));
    new_case->x = x;
    new_case->y = y;
    new_case->state = state; // Store the string representing the state
    return new_case;
}

// Function to create a cell with a given height, width, and initial state for all cases
Cell *create_cell(char *name, int height, int width, char *initial_state)
{
    Cell *new_cell = (Cell *)malloc(sizeof(Cell));
    new_cell->name = name;
    new_cell->height = height;
    new_cell->width = width;

    // Allocate memory for the cases
    new_cell->cases = (Case **)malloc(height * width * sizeof(Case *));

    // Initialize each case with the coordinates and the given initial state
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            new_cell->cases[i * width + j] = create_case(i, j, initial_state);
        }
    }

    // Add the cell to the global list of cells
    add_cell(new_cell);

    return new_cell;
}

// Function to add a cell to the global cell list
void add_cell(Cell *new_cell)
{
    num_cells++;
    cell_table = (Cell **)realloc(cell_table, num_cells * sizeof(Cell *));
    cell_table[num_cells - 1] = new_cell;
}

// Function to initialize the connection table
void initialize_connection_table(int initial_size)
{
    connection_table = (Connection **)malloc(initial_size * sizeof(Connection *));
}

// Function to resize the connection table if necessary
void resize_connection_table(int new_size)
{
    connection_table = (Connection **)realloc(connection_table, new_size * sizeof(Connection *));
}

void connect_cells(char *cell1_name, int x1, int y1, char *cell2_name, int x2, int y2)
{
    // Trouve les cellules par leur nom
    Cell *cell1 = find_cell(cell1_name);
    Cell *cell2 = find_cell(cell2_name);

    // Vérifie que les deux cellules ont été trouvées
    if (cell1 == NULL || cell2 == NULL)
    {
        printf("Error: One or both cells not found.\n");
        return;
    }

    // Vérifie que les coordonnées sont dans les limites des cellules
    if (x1 < 0 || x1 >= cell1->height || y1 < 0 || y1 >= cell1->width)
    {
        printf("Error: Coordinates (%d, %d) are out of bounds for cell %s.\n", x1, y1, cell1->name);
        return;
    }
    if (x2 < 0 || x2 >= cell2->height || y2 < 0 || y2 >= cell2->width)
    {
        printf("Error: Coordinates (%d, %d) are out of bounds for cell %s.\n", x2, y2, cell2->name);
        return;
    }

    // Recherche des cases correspondantes dans chaque cellule
    Case *case1 = cell1->cases[x1 * cell1->width + y1];
    Case *case2 = cell2->cases[x2 * cell2->width + y2];

    // Vérifie que les cases existent
    if (case1 == NULL || case2 == NULL)
    {
        printf("Error: One or both cases could not be found.\n");
        return;
    }

    // Créer la connexion
    Connection *new_connection = (Connection *)malloc(sizeof(Connection));
    if (new_connection == NULL)
    {
        printf("Error: Memory allocation failed.\n");
        return;
    }

    new_connection->cell1 = cell1;
    new_connection->case1 = case1;
    new_connection->cell2 = cell2;
    new_connection->case2 = case2;

    // Ajouter la nouvelle connexion dans le tableau
    connection_table[num_connections] = new_connection;
    num_connections++;

    printf("Cases (%d, %d) from %s and (%d, %d) from %s are now connected.\n",
           x1, y1, cell1->name, x2, y2, cell2->name);
}

Cell *find_cell(char *name)
{
    for (int i = 0; i < num_cells; i++)
    {
        if (strcmp(cell_table[i]->name, name) == 0)
        {
            return cell_table[i];
        }
    }
    return NULL; // Return NULL if the cell is not found
}

// Function to print all cells and their cases
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

// Function to print all connections between cases
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