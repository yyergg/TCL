TCL: lex.yy.c TCL.tab.c TCL.tab.h TCL.cpp
	g++ TCL.cpp TCL.tab.c lex.yy.c redlib.a -o TCL -m32 -static

TCL.tab.c TCL.tab.h:TCL.y
	bison -d TCL.y

lex.yy.c:TCL.l
	flex TCL.l

clean:
	rm *.o ./Models/*.redtab ./Models/*.ir TCL