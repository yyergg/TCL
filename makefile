TCL: lex.yy.c TCL.tab.c TCL.tab.h TCL.cpp
	g++ TCL.cpp TCL.tab.c lex.yy.c debug.cpp redlib.a  -lfl -o TCL

TCL.tab.c TCL.tab.h:TCL.y
	bison -d TCL.y

lex.yy.c:TCL.l
	flex TCL.l

clean:
	find . -name '*.ir' -type f -exec rm -f {} \;
	find . -name '*.o' -type f -exec rm -f {} \;
	find . -name '*.redtab' -type f -exec rm -f {} \;
	find . -name '*~' -type f -exec rm -f {} \;
	rm TCL



	
