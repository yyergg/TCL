TCL: TCL.cpp lex.yy.c Expression.tab.c Expression.tab.h Graph Expression
	g++ TCL.cpp Expression.tab.c lex.yy.c Graph.o Expression.o -g -o TCL

Graph: Graph.cpp Graph.h
	g++ -c Graph.cpp

Expression: lex.yy.c Expression.tab.c Expression.tab.h Expression.cpp Expression.h
	g++ -c Expression.cpp

Expression.tab.c Expression.tab.h:Expression.y
	bison -d Expression.y

lex.yy.c:Expression.l
	flex Expression.l

clean:
	rm TCL
	rm *.o
	rm *.tab.c
	rm *.tab.h
	rm lex.yy.c