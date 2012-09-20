#ifndef PSIL_H
#define PSIL_H
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
extern "C"{
	#include"redcplugins.e"
	#include"redlib.h"
	#include"redlib.e"
}




class Computation_Tree_Node;
class PSIL_Formula;
class PSIL_Game_Node;
class PSIL_Game_Edge;




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


class Computation_Tree_Node{
	public:
		Computation_Tree_Node();
		~Computation_Tree_Node();
		Computation_Tree_Node* ins;
		vector<Computation_Tree_Node*> outs;
		int until_token_old;
		int until_token;
		int* G;
		int* obligation;
		int* pass_down;
		bool* passed;
		PSIL_Game_Node* state;
};





class PSIL_Formula{
	public:
	int type;
	int index;
	int strategy_index;
	int closure_index;
	string str;
	int owner;
	vector<PSIL_Formula*> ins;
	vector<PSIL_Formula*> outs;
};

class PSIL_Game_Node{
	public:
	int index;
	redgram red;
	int owner;
	vector<PSIL_Game_Edge*> outs;
	vector<PSIL_Game_Edge*> ins;
};

class PSIL_Game_Edge{
	public:
	int sxi;
	PSIL_Game_Node* src;
	PSIL_Game_Node* dst;
};




#endif
