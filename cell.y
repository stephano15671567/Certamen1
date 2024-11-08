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

%token CREAR_AUTOMATA CREAR_CELDA CONECTAR_AC IMPRIMIR_CELDAS IMPRIMIR_CONEXIONES IMPRIMIR
%token ESTABLECER_TASAS SIMULAR PERMITIR_MOVIMIENTO RESTRINGIR_MOVIMIENTO
%token TODOS
%token <str> ID
%token <num> NUM

%%

programa:
    instrucciones
    ;

instrucciones:
    instruccion
    | instrucciones instruccion
    ;

instruccion:
    // Crear un nuevo autómata
    CREAR_AUTOMATA ID {
        crear_automata($2);
    }
    // Crear una nueva celda con los parámetros adicionales (D y V)
    | CREAR_CELDA ID ID NUM NUM NUM NUM NUM NUM NUM NUM {
        Celda *nueva_celda = crear_celda($3, (int)$4, (int)$5, (int)$6, (int)$7, (int)$8, (int)$9, (int)$10, (int)$11);
        Automata *automata = encontrar_automata($2);
        if (automata != NULL) {
            agregar_celda_a_automata(automata, nueva_celda);
        } else {
            printf("Error: Autómata %s no encontrado.\n", $2);
        }
    }
    // Conectar celdas entre autómatas
    | CONECTAR_AC ID ID NUM NUM ID ID NUM NUM {
        conectar_celdas_ac($2, $3, (int)$4, (int)$5, $6, $7, (int)$8, (int)$9);
    }
    // Imprimir todas las celdas
    | IMPRIMIR_CELDAS {
        imprimir_celdas();
    }
    // Imprimir todas las conexiones
    | IMPRIMIR_CONEXIONES {
        imprimir_conexiones();
    }
    // Imprimir el estado de una celda o autómata específico
    | IMPRIMIR ID {
        Celda *celda = encontrar_celda($2);
        if (celda != NULL) {
            imprimir_estado_celda($2);
        } else {
            Automata *automata = encontrar_automata($2);
            if (automata != NULL) {
                imprimir_estado_automata($2);
            } else {
                printf("Error: Celda o Autómata %s no encontrado.\n", $2);
            }
        }
    }
    // Imprimir el estado de todos los autómatas y celdas
    | IMPRIMIR TODOS {
        imprimir_todos_estados();
    }
    // Establecer nuevas tasas
    | ESTABLECER_TASAS NUM NUM NUM NUM NUM NUM {
        establecer_tasas($2, $3, $4, $5, $6, $7);
    }
    // Simulación del sistema
    | SIMULAR NUM {
        simular((int)$2, permitir_movimiento);
    }
    // Permitir el movimiento entre celdas
    | PERMITIR_MOVIMIENTO {
        permitir_movimiento = 1;
        printf("Movimiento entre celdas permitido.\n");
    }
    // Restringir el movimiento entre celdas (confinamiento)
    | RESTRINGIR_MOVIMIENTO {
        permitir_movimiento = 0;
        printf("Movimiento entre celdas restringido.\n");
    }
    ;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Error: %s\n", s);
}
