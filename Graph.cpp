#include <iostream>
#include "Graph.h"
using namespace::std;

GraphNode* rootGraph;
int playerNumber = 3;
vector<GraphNode*> allNodes;

void GraphNode::determineStatus(){
    int betrayerCounter;
    int betrayerIndex;
    betrayerCounter = 0;
    betrayerIndex = 0;

    int i;
    i = 0;
    for (i=0;i<this->playerSelections.size();i++){
        if(this->playerSelections[i] == CHOOSE_CONFESS){
            betrayerCounter++;
            betrayerIndex = i;
        }
    }
    if(betrayerCounter == 0){
        fill(this->playerStatus.begin(),this->playerStatus.end(),PLAYER_FREE);
    } else if(betrayerCounter == 1){
        fill(this->playerStatus.begin(),this->playerStatus.end(),PLAYER_JAIL);
        this->playerStatus[betrayerIndex] = PLAYER_FREE;
    } else {
        fill(this->playerStatus.begin(),this->playerStatus.end(),PLAYER_JAIL);
    }
}

GraphNode::GraphNode(){
    allNodes.push_back(this);
}

void printGraph(){
    int i,j;
    for(i = 0;i < allNodes.size();i++){
        for(j = 0;j < allNodes[i]->playerSelections.size();j++){
            cout<<allNodes[i]->playerSelections[j]<<" ";
        }
        cout<<endl;
        for(j = 0;j < allNodes[i]->playerStatus.size();j++){
            cout<<allNodes[i]->playerStatus[j]<<" ";
        }
        cout<<endl;
    }
}

void initGraph(){
    rootGraph = new GraphNode();
    rootGraph->owner = 0;
    rootGraph->ins = vector<GraphNode*>();
    rootGraph->outs = vector<GraphNode*>();
    rootGraph->playerSelections = vector<int>(playerNumber, CHOOSE_NOT_YET);
    rootGraph->playerStatus = vector<int>(playerNumber, PLAYER_FREE);
}

void buildGraph(GraphNode* node){
    if(node->owner == playerNumber){
        node->determineStatus();
        node->outs.push_back(rootGraph);
        rootGraph->ins.push_back(node);
        return;
    }

    // create child 1
    GraphNode* child1 = new GraphNode();
    child1->owner = node->owner + 1;
    child1->ins = vector<GraphNode*>();
    child1->outs = vector<GraphNode*>();
    child1->playerSelections = node->playerSelections;
    child1->playerStatus = vector<int>(playerNumber, PLAYER_UNKNOWN);
    // init value
    child1->ins.push_back(node);
    child1->playerSelections[node->owner] = CHOOSE_CONFESS;

    // create child 2
    GraphNode* child2 = new GraphNode();
    child2->owner = node->owner + 1;
    child2->ins = vector<GraphNode*>();
    child2->outs = vector<GraphNode*>();
    child2->playerSelections = node->playerSelections;
    child2->playerStatus = vector<int>(playerNumber, PLAYER_UNKNOWN);
    // init value
    child2->ins.push_back(node);
    child2->playerSelections[node->owner] = CHOOSE_DENY;

    buildGraph(child1);
    buildGraph(child2);
    node->outs.push_back(child1);
    node->outs.push_back(child2);
}