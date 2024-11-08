%{
    #include "cell.tab.h"
%}

%%
"CREAR_AUTOMATA"        { return CREAR_AUTOMATA; }
"CREAR_CELDA"           { return CREAR_CELDA; }
"CONECTAR_AC"           { return CONECTAR_AC; }
"IMPRIMIR_CELDAS"       { return IMPRIMIR_CELDAS; }
"IMPRIMIR_CONEXIONES"   { return IMPRIMIR_CONEXIONES; }
"IMPRIMIR"              { return IMPRIMIR; }
"ESTABLECER_TASAS"      { return ESTABLECER_TASAS; }
"SIMULAR"               { return SIMULAR; }
"PERMITIR_MOVIMIENTO"   { return PERMITIR_MOVIMIENTO; }
"RESTRINGIR_MOVIMIENTO" { return RESTRINGIR_MOVIMIENTO; }

"TODOS"                 { return TODOS; }
[0-9]+"."[0-9]+         { yylval.num = atof(yytext); return NUM; } // Para números decimales
[0-9]+                  { yylval.num = atof(yytext); return NUM; }
[a-zA-Z_][a-zA-Z0-9_]*  { yylval.str = strdup(yytext); return ID; }

[ \t\n]+                { /* Ignorar espacios en blanco y saltos de línea */ }
.                       { printf("Carácter no reconocido: %s\n", yytext); }

%%

int yywrap() {
    return 1;
}


