#ifndef EXP_H
#define EXP_H
#include<iostream>
#include<fstream>
#include<stdlib.h>
#include<stdio.h>
#include<string>
#include<vector>
#include<map>
#include<iomanip>
#include<sstream>
#include<cstring>
using namespace::std;

class ExpNode;

#define MUST_TRUE 0
#define MUST_FALSE 1
#define TRUE_GUESSED_PHASE_1 2
#define TRUE_GUESSED_PHASE_2 3
#define FALSE_GUESSED_PHASE_1 4
#define FALSE_GUESSED_PHASE_2 5
#define DONT_CARE 6

#define TRUE_NODE 7
#define FALSE_NODE 8
#define PARSE_ROOT 9
#define ATOMIC 10
#define NOT 11
#define OR 12
#define AND 13
#define PLUS 14
#define MINUS 15
#define UNTIL 16
#define WNTIL 17
#define NEXT 18

#define PASS 21
#define FAIL 22
#define UNVISITED 23
#define CONTINUE 24

class ExpNode{
    public:
    int type;
    int index;
    int strategy_index;
    int closure_index;
    string str;
    int owner;
    vector<ExpNode*> ins;
    vector<ExpNode*> outs;
};

void parseExpression(char*);


#endif