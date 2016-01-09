basic: basic.y basic.lex
	bison -d basic.y
	flex -i basic.lex
	cc -o basic basic.tab.c lex.yy.c -ll -std=c89

clean:
	rm basic basic.tab.c basic.tab.h lex.yy.c
