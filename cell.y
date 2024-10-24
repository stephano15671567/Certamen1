%{
    #include <stdio.h>
    #include "cell.h"

    int yylex(void);
    void yyerror(const char *s);
%}

%union {
    char* str;
    double num;
}

%token CREATE_AUTOMATON CREATE_CELL CONNECT_AC PRINT_CELLS PRINT_CONNECTIONS PRINT
%token SET_RATES SIMULATE ALLOW_MOVEMENT DISALLOW_MOVEMENT
%token ALL
%token <str> ID
%token <num> NUM

%%

program:
    statements
    ;

statements:
    statement
    | statements statement
    ;

statement:
    // Crear un nuevo autómata
    CREATE_AUTOMATON ID {
        create_automaton($2);
    }
    // Crear una nueva celda con los parámetros adicionales (D y V)
    | CREATE_CELL ID ID NUM NUM NUM NUM NUM NUM NUM NUM {
        Cell *new_cell = create_cell($3, (int)$4, (int)$5, (int)$6, (int)$7, (int)$8, (int)$9, (int)$10, (int)$11);
        Automaton *automaton = find_automaton($2);
        if (automaton != NULL) {
            add_cell_to_automaton(automaton, new_cell);
        } else {
            printf("Error: Automaton %s not found.\n", $2);
        }
    }
    // Conectar celdas entre autómatas
    | CONNECT_AC ID ID NUM NUM ID ID NUM NUM {
        connect_cells_ac($2, $3, (int)$4, (int)$5, $6, $7, (int)$8, (int)$9);
    }
    // Imprimir todas las celdas
    | PRINT_CELLS {
        print_cells();
    }
    // Imprimir todas las conexiones
    | PRINT_CONNECTIONS {
        print_connections();
    }
    // Imprimir el estado de una celda o autómata específico
    | PRINT ID {
        Cell *cell = find_cell($2);
        if (cell != NULL) {
            print_cell_state($2);
        } else {
            Automaton *automaton = find_automaton($2);
            if (automaton != NULL) {
                print_automaton_state($2);
            } else {
                printf("Error: Cell or Automaton %s not found.\n", $2);
            }
        }
    }
    // Imprimir el estado de todos los autómatas y celdas
    | PRINT ALL {
        print_all_states();
    }
    // Establecer nuevas tasas
    | SET_RATES NUM NUM NUM NUM NUM NUM {
        set_rates($2, $3, $4, $5, $6, $7);
    }
    // Simulación del sistema
    | SIMULATE NUM {
        simulate((int)$2, allow_movement);
    }
    // Permitir el movimiento entre celdas
    | ALLOW_MOVEMENT {
        allow_movement = 1;
        printf("Movement between cells allowed.\n");
    }
    // Restringir el movimiento entre celdas (confinamiento)
    | DISALLOW_MOVEMENT {
        allow_movement = 0;
        printf("Movement between cells disallowed.\n");
    }
    ;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Error: %s\n", s);
}
