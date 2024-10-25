all: main

main: lex.yy.c cell.tab.c cell.o main.o
	gcc -o main lex.yy.c cell.tab.c cell.o main.o -lfl -lm


cell.o: cell.c cell.h
	gcc -c cell.c

main.o: main.c cell.h
	gcc -c main.c

lex.yy.c: cell.l
	flex cell.l

cell.tab.c: cell.y
	bison -d cell.y

clean:
	rm -f lex.yy.c cell.tab.c cell.tab.h cell.o main.o main
