%{
    #include <stdio.h>
    #include "cell.h"

    int yylex(void);  
    void yyerror(const char *s);  
%}

%union {
    char* str;  
    int num;    
}

%token CREATE_AUTOMATON CREATE_CELL CONNECT_AC PRINT_CELLS PRINT_CONNECTIONS
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
    | CREATE_CELL ID ID NUM NUM ID {
        Cell *new_cell = create_cell($3, $4, $5, $6);
        Automaton *automaton = find_automaton($2);
        if (automaton != NULL) {
            add_cell_to_automaton(automaton, new_cell); 
        } else {
            printf("Error: Automaton %s not found.\n", $2);
        }
    }
    | CONNECT_AC ID ID NUM NUM ID ID NUM NUM {
        connect_cells_ac($2, $3, $4, $5, $6, $7, $8, $9);
    }
    | PRINT_CELLS {
        print_cells(); 
    }
    | PRINT_CONNECTIONS {
        print_connections(); 
    }
    ;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Error: %s\n", s);
}
