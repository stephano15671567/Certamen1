#include <stdio.h>
#include "cell.h"

// Prototipo para yyparse (generado por Bison)
int yyparse();

int main()
{
    printf("Simulación de Autómata Celular\n");
    printf("Ingrese sus comandos:\n");
    yyparse(); // Llamada al parser generado por Bison
    return 0;
}
