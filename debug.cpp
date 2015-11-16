#include "iostream"
#include "TCL.h"
#include "debug.h"
using namespace::std;
extern map<int,string> type_map;
extern int closureCount;
extern vector<TCL_Formula*> Closure;
extern int strategyCount;
extern vector<int> strategy2owner;
extern int ** Matrix;
extern vector<TCL_Game_Node*> Nodes;


void print_parse_tree(TCL_Formula* F, int depth){
    int i;
    for(i=0;i<depth;i++){
        cout<<"  ";
    }
    cout<<F->index<<" "<<type_map[F->type]<<" ";
    if(F->type==ATOMIC){cout<<F->str;}
    else if(F->type==PARSE_ROOT || F->type==PLUS){cout<<F->owner;}
    cout<<endl;
    for(i=0;i<F->outs.size();i++){
        print_parse_tree(F->outs[i],depth+1);
    }
}


void print_matrix(){
    int i,j;
    cout<<setw(7)<<"*******";
    for(i=0;i<closureCount;i++){
        cout<<setw(5)<<Closure[i]->index;
    }
    cout<<endl;
    for(i=0;i<strategyCount;i++){
        string temp_string;
        stringstream ss(temp_string);
        ss<<i<<"("<<strategy2owner[i]<<")";
        cout<<setw(7)<<ss.str();
        for(j=0;j<closureCount;j++){
            cout<<setw(5)<<Matrix[i][j];
        }
        cout<<endl;
    }
    cout<<endl;
}


void print_strategy2owner(){
    int i;
    cout<<"strategy2owner:"<<endl;
    for(i=0;i<strategy2owner.size();i++){
        cout<<strategy2owner[i]<<" ";
    }
    cout<<endl;
}


void printGameGraph(){
    int i,j,k;
    for(i=0;i<Nodes.size();i++){
        cout<<i<<": "<<red_diagram_string(Nodes[i]->red)<<endl;
        for(j=0;j<Nodes[i]->outs.size();j++){
            cout<<"[";
            for(k=0;k<Nodes[i]->outs[j]->selectionArray.size();k++){
                cout<<Nodes[i]->outs[j]->selectionArray[k];
                if(k<Nodes[i]->outs[j]->selectionArray.size()-1){cout<<",";}
            }
            cout<<"]-->"<<red_diagram_string(Nodes[Nodes[i]->outs[j]->dst->index]->red)<<endl;
        }
    }
}