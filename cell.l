%{
    #include "cell.tab.h"
%}

%%

"CREATE_CELL"      { return CREATE_CELL; }
"CONNECT"          { return CONNECT; }
"PRINT_CELLS"      { return PRINT_CELLS; }
"PRINT_CONNECTIONS" { return PRINT_CONNECTIONS; }

[0-9]+             { yylval.num = atoi(yytext); return NUM; }
[a-zA-Z_][a-zA-Z0-9_]* { yylval.str = strdup(yytext); return ID; }

"("                { return '('; }
")"                { return ')'; }
","                { return ','; }

[ \t\n]+           { /* Ignore whitespace and new lines */ }
.                  { printf("Unrecognized character: %s\n", yytext); }

%%

int yywrap() {
    return 1;
}
