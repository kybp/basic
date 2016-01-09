basic: stack.o listing.o basic.tab.h lex.yy.c
	cc -o basic basic.tab.c lex.yy.c stack.o listing.o -ll -std=c89

lex.yy.c: basic.lex basic.tab.h
	flex -i basic.lex

basic.tab.h: basic.y
	bison -d basic.y

stack.o: stack.c stack.h
	cc -c stack.c

listing.o: listing.c listing.h
	cc -c listing.c

clean:
	rm basic basic.tab.c basic.tab.h lex.yy.c stack.o listing.o
