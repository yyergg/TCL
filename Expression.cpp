#include "Expression.h"
using namespace::std;

extern "C" int yylex();
extern "C" int yyparse();
extern "C" FILE *yyin;

ExpNode* rootExp;


void parseExpression(char* fileName){
    FILE *infile = fopen(fileName, "r");
    yyin=infile;
    do {
        yyparse();
    } while (!feof(yyin));
}