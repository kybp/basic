%{
#include <stdlib.h>
#include <string.h>
#include "basic.tab.h"
%}

ID [-_0-9a-z]+

%%

\"[^\"]*\"      { yylval.string = (char *)malloc(strlen(yytext) + 1);
                  strcpy(yylval.string, yytext);
                  return STRING; }
[0-9]+          { yylval.integer = atoi(yytext); return INTEGER; }
[0-9]+"."[0-9]+ { yylval.real    = atof(yytext); return REAL; }

"+"             { return ADD;  }
"-"             { return SUB;  }
"*"             { return MUL;  }
"/"             { return DIV;  }
"^"             { return EXPT; }

"gosub"         { return GOSUB;  }
"goto"          { return GOTO;   }
"if"            { return IF;     }
"let"           { return LET;    }
"?"             { return PRINT;  }
"print"         { return PRINT;  }
"rem"[^\n]*     { /* ignore comments */ }
"return"        { return RETURN; }

"<"             { return LT; }
"<="            { return LE; }
"="             { return EQ; }
">="            { return GE; }
">"             { return GT; }
"<>"            { return NE; }
","             { return COMMA; }
";"             { return SEMI; }

"("             { return LPAREN; }
")"             { return RPAREN; }

"\n"            { return EOL; }
[ \t]           { /* ignore whitespace */ }

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
