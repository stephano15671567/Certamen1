%{
    #include "cell.tab.h"
%}

%%
"CREATE_AUTOMATON"      { return CREATE_AUTOMATON; }
"CREATE_CELL"           { return CREATE_CELL; }
"CONNECT_AC"            { return CONNECT_AC; }
"PRINT_CELLS"           { return PRINT_CELLS; }
"PRINT_CONNECTIONS"     { return PRINT_CONNECTIONS; }
"PRINT"                 { return PRINT; }
"SET_RATES"             { return SET_RATES; }
"SIMULATE"              { return SIMULATE; }
"ALLOW_MOVEMENT"        { return ALLOW_MOVEMENT; }
"DISALLOW_MOVEMENT"     { return DISALLOW_MOVEMENT; }

"ALL"                   { return ALL; }
[0-9]+"."[0-9]+         { yylval.num = atof(yytext); return NUM; } // Para números decimales
[0-9]+                  { yylval.num = atof(yytext); return NUM; }
[a-zA-Z_][a-zA-Z0-9_]*  { yylval.str = strdup(yytext); return ID; }

[ \t\n]+                { /* Ignorar espacios en blanco y saltos de línea */ }
.                       { printf("Unrecognized character: %s\n", yytext); }

%%

int yywrap() {
    return 1;
}

