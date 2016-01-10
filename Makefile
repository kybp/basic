CFLAGS = -g -Wall -Wextra -std=c89

basic: stack.o listing.o basic.tab.h lex.yy.c
	cc $(CFLAGS) -o basic basic.tab.c lex.yy.c stack.o listing.o -ll

lex.yy.c: basic.lex basic.tab.h
	flex -i basic.lex

basic.tab.h: basic.y
	bison -d basic.y

stack.o: stack.c stack.h
	cc $(CFLAGS) -c stack.c

listing.o: listing.c listing.h basic.tab.h
	cc $(CFLAGS) -c listing.c

clean:
	rm basic basic.tab.c basic.tab.h lex.yy.c stack.o listing.o
