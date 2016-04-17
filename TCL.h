#ifndef TCL_H
#define TCL_H
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
#include<utility>
using namespace::std;
extern "C"{
    #include"redcplugins.e"
    #include"redlib.h"
    #include"redlib.e"
}

class ComputationTreeNode;
class TCLFormula;
class TCLGameNode;
class TCLGameEdge;

#define DONT_CARE 0
#define MUST_TRUE 1
#define MUST_FALSE 2
#define PASS_DOWN_MUST_TRUE 3
#define PASS_DOWN_MUST_FALSE 4
#define PASS_DOWN_AND_MUST_TRUE 5
#define PASS_DOWN_AND_MUST_FALSE 6
#define GUESSED_UNTIL_TRUE 7
#define GUESSED_UNTIL_FALSE 8

#define TRUE_NODE 8
#define FALSE_NODE 9
#define PARSE_ROOT 10
#define ATOMIC 11
#define NOT 12
#define OR 13
#define AND 14
#define PLUS 15
#define MINUS 16
#define UNTIL 17
#define WNTIL 18
#define NEXT 19

// no lasso or not done yet
#define CONTINUE 20
#define PASS 21
#define FAIL 22

class ComputationTreeNode{
    public:
        ComputationTreeNode();
        bool CheckTCL();
        void ListAllPossibleGuess();
        void PrintAllPossibleGuess();
        void CheckLocal();
        bool CreatePassDown();
        void GetFirstStrategySelection();
        bool GetNextStrategySelection();
        void PrintStrategySelection();
        bool PassDown();
        int CheckPath();
        bool isDifferent(ComputationTreeNode*);
        int GetMaxNextHeight();
        ComputationTreeNode* parent;
        TCLGameNode* state;
        int current_guess;
        vector< vector<int> > all_possible_guess;
        map<int, int> strategy_selection;
        vector<int> obligation;
        vector<int> pass_down;
};

class TCLFormula{
    public:
        int type;
        int index;
        int strategy_index;
        string str;
        int owner;
        int next_height;
        vector<TCLFormula*> ins;
        vector<TCLFormula*> outs;
};

class TCLGameNode{
    public:
        int index;
        redgram red;
        int owner;
        vector<TCLGameEdge*> outs;
        vector<TCLGameEdge*> ins;
};

class TCLGameEdge{
    public:
        int sxi;
        TCLGameNode* src;
        TCLGameNode* dst;
};
#endif
