#include <iostream>
#include <fstream>
#include <algorithm>
#include "Graph.h"
#include "Expression.h"
#include "TCL.h"

using namespace::std;

extern GraphNode* rootGraph;

int main(int argc, char** argv){
    initGraph();
    buildGraph(rootGraph);
    // printGraph();
    fstream fileExp;
    fileExp.open(argv[1], ios::in);
    parseExpression(argv[1]);
}