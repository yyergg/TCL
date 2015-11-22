#include <vector>
using namespace::std;

#define PLAYER_UNKNOWN 0
#define PLAYER_FREE 1
#define PLAYER_JAIL 2

#define CHOOSE_NOT_YET 0
#define CHOOSE_CONFESS 1
#define CHOOSE_DENY 2

class GraphNode;

class GraphNode{
public:
    int owner;
    vector<GraphNode*> ins;
    vector<GraphNode*> outs;
    vector<int> playerSelections;
    vector<int> playerStatus;
    GraphNode();
    void determineStatus();
};

void buildGraph(GraphNode*);
void printGraph();
void initGraph();
