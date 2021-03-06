%{
#include <stdlib.h>
#include <string.h>
#include "basic.tab.h"

int fileno(FILE *f);
void yyerror(char const *s);

%}

ID [_0-9a-z]+

%%

\"[^\"]*\"       { yylval.string = (char *)malloc(strlen(yytext) - 1);
                   strncpy(yylval.string, yytext + 1, strlen(yytext) - 2);
                   return STRING; }
[0-9]+           { yylval.integer = atoi(yytext); return INTEGER; }
[0-9]+"."[0-9]+  { yylval.real    = atof(yytext); return REAL; }

"+"              { return ADD;  }
"-"              { return SUB;  }
"*"              { return MUL;  }
"/"              { return DIV;  }
"**"             { return EXPT; }

"ceil"           { return CEIL;      }
"floor"          { return FLOOR;     }
"real"           { return REAL_CAST; }
"round"          { return ROUND;     }

"rand"           { return RAND; }
"sqrt"           { return SQRT; }

"gosub"          { return GOSUB;  }
"goto"           { return GOTO;   }
"if"             { return IF;     }
"input"          { return INPUT;  }
"let"            { return LET;    }
"?"              { return PRINT;  }
"print"          { return PRINT;  }
"rem"[^\n]*      { /* ignore comments */ }
"return"         { return RETURN; }

"list"           { return LIST; }
"load"           { return LOAD; }
"new"            { return NEW;  }
"run"            { return RUN;  }
"save"           { return SAVE; }

"<"              { return LT; }
"<="             { return LE; }
"="              { return EQ; }
">="             { return GE; }
">"              { return GT; }
"<>"             { return NE; }
","              { return COMMA; }
";"              { return SEMI; }

"("              { return LPAREN; }
")"              { return RPAREN; }

"\n"             { return EOL; }
[ \t]            { /* ignore whitespace */ }

{ID}"$" { yylval.string = (char *)malloc(strlen(yytext) + 1);
          strcpy(yylval.string, yytext);
          return STR_VAR; }
{ID}"%" { yylval.string = (char *)malloc(strlen(yytext) + 1);
          strcpy(yylval.string, yytext);
          return INT_VAR; }
{ID}    { yylval.string = (char *)malloc(strlen(yytext) + 1);
          strcpy(yylval.string, yytext);
          return REAL_VAR; }
. { yyerror("unrecognised character"); }

%%
