all: slytherin.o

slytherin.o: main.cpp node.cpp parser.y flex.l parser.tab.c parser.tab.h lex.yy.c
	g++ parser.tab.c lex.yy.c node.cpp main.cpp -o slytherin.o 

parser.tab.h: parser.y node.cpp
	bison -d parser.y -Wno-all

parser.tab.c: parser.y node.cpp
	bison -d parser.y -Wno-all

lex.yy.c: flex.l parser.y node.cpp
	flex flex.l

clean:
	rm -f lex.yy.c parser.tab.h parser.tab.c *.dot *.o *.pdf *.csv *.txt *.c *.s