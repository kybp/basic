basic: basic.y basic.lex
	bison -d basic.y
	flex -i basic.lex
	cc -o basic basic.tab.c lex.yy.c -ll -std=c89
