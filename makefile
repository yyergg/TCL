PSIL: lex.yy.c PSIL.tab.c PSIL.tab.h PSIL.cpp
	g++ PSIL.cpp PSIL.tab.c lex.yy.c redlib.a  -lfl -o PSIL

PSIL.tab.c PSIL.tab.h:PSIL.y
	bison -d PSIL.y

lex.yy.c:PSIL.l
	flex PSIL.l

clean:
	find . -name '*.ir' -o -name '*.redtab' -o -name '*.o' -type f -exec rm -f {} \;
	rm PSIL



	
