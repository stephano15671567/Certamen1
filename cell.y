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
    CREATE_AUTOMATON ID {
        create_automaton($2);
    }
    | CREATE_CELL ID ID NUM NUM NUM NUM NUM NUM {
        Cell *new_cell = create_cell($3, (int)$4, (int)$5, (int)$6, (int)$7, (int)$8, (int)$9);
        Automaton *automaton = find_automaton($2);
        if (automaton != NULL) {
            add_cell_to_automaton(automaton, new_cell);
        } else {
            printf("Error: Automaton %s not found.\n", $2);
        }
    }
    | CONNECT_AC ID ID NUM NUM ID ID NUM NUM {
        connect_cells_ac($2, $3, (int)$4, (int)$5, $6, $7, (int)$8, (int)$9);
    }
    | PRINT_CELLS {
        print_cells();
    }
    | PRINT_CONNECTIONS {
        print_connections();
    }
    | PRINT ID {
        // Imprimir estado de una celda o autómata
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
    | PRINT ALL {
        print_all_states();
    }
    | SET_RATES NUM NUM NUM NUM {
        set_rates($2, $3, $4, $5);
        printf("Rates updated.\n");
    }
    | SIMULATE NUM {
        simulate((int)$2, allow_movement);
    }
    | ALLOW_MOVEMENT {
        allow_movement = 1;
        printf("Movement between cells allowed.\n");
    }
    | DISALLOW_MOVEMENT {
        allow_movement = 0;
        printf("Movement between cells disallowed.\n");
    }
    ;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Error: %s\n", s);
}
