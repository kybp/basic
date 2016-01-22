CFLAGS = -g -O0 -Wall -Wextra -std=c89

basic: expr.o symtab.o stack.o listing.o basic.tab.h lex.yy.c
	cc $(CFLAGS) -o basic basic.tab.c lex.yy.c \
		stack.o listing.o symtab.o expr.o -ll -lm

lex.yy.c: basic.lex basic.tab.h
	flex -i basic.lex

basic.tab.h: basic.y
	bison -d basic.y

stack.o: stack.c stack.h
	cc $(CFLAGS) -c stack.c

listing.o: listing.c listing.h basic.tab.h
	cc $(CFLAGS) -c listing.c

symtab.o: symtab.c symtab.h
	cc $(CFLAGS) -c symtab.c

expr.o: expr.c expr.h basic.tab.h
	cc $(CFLAGS) -c expr.c

clean:
	rm basic basic.tab.c basic.tab.h lex.yy.c stack.o listing.o expr.o
