%{
    #include <stdio.h>
    #include "cell.h"

    int yylex(void);  // Declaration de yylex pour Flex
    void yyerror(const char *s);  // Déclaration de la fonction de gestion des erreurs
%}

%union {
    char* str;  // Pour stocker les noms de cellules (chaînes de caractères)
    int num;    // Pour stocker les valeurs numériques (coordonnées, dimensions)
}

%token CREATE_CELL CONNECT PRINT_CELLS PRINT_CONNECTIONS
%token <str> ID
%token <num> NUM

%%

// Définition des statements acceptés dans le programme
program:
    statements
    ;

statements:
    statement
    | statements statement
    ;

statement:
    CREATE_CELL ID NUM NUM ID            { create_cell($2, $3, $4, $5); }  // Créer une cellule avec un nom, des dimensions et un état initial
    | CONNECT ID NUM NUM ID NUM NUM      { connect_cells($2, $3, $4, $5, $6, $7); }  // Connecter deux cases entre deux cellules
    | PRINT_CELLS                       { print_cells(); }  // Afficher toutes les cellules créées
    | PRINT_CONNECTIONS                 { print_connections(); }  // Afficher toutes les connexions entre les cellules
    ;

%%

// Gestion des erreurs
void yyerror(const char *s) {
    fprintf(stderr, "Error: %s\n", s);
}
