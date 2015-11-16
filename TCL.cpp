#include "TCL.h"
#include "debug.h"
/*extern function from parser*/
extern "C" int yylex();
extern "C" int yyparse();
extern "C" FILE *yyin;

//Global variables
bool debugCheckTCL=false;

int sxiCount;
int nodeCount;
int strategyCount;
int closureCount;
int parseCount;

redgram path;
TCL_Game_Edge* temp_edge;
TCL_Game_Node* temp_node;
string temp_string;
char temp_char[1000];
map<string,int> node_map;
map<int,string> type_map;
vector<TCL_Game_Node*> Nodes;
vector<TCL_Formula*> Parse_Tree;
vector<TCL_Formula*> Closure;
vector<int> strategy2owner;

TCL_Formula* ROOT_ptr;

int ** Matrix;
int* strategy_stack;
//End of Global variables

int    cplugin_proc(int module_index,int    proc_index) {
  switch(module_index) {
  case 1:
    break;
  case 2:
    break;
  case 3:
    switch (proc_index) {
    case 1:
      break;
    case 2:
      break;
    case 3:

      break;
    default:
      fprintf(RED_OUT, "\nCPLUG-INs module %1d: Illegal proc index %1d.\n",
        module_index, proc_index
      );
      fflush(RED_OUT);
      exit(0);
    }
    break;
  default:
    fprintf(RED_OUT,
      "\nCPLUG-INs: Illegal module index %1d.\n", module_index
    );
    fflush(RED_OUT);
    exit(0);
  }
}
//redundant function to meet RED requirement

Computation_Tree_Node::Computation_Tree_Node(){
    int i;
    guessedSolution=new int[closureCount];
    until_token=0;
    until_token_old=0;
    obligation=new int[closureCount];
    passDown=new int[closureCount];
    passed=new bool[closureCount];
    for(i=0;i<closureCount;i++){
        guessedSolution[i]=DONT_CARE;
        obligation[i]=0;
        passDown[i]=0;
        passed[i]=false;
    }
}


Computation_Tree_Node::~Computation_Tree_Node(){
    delete guessedSolution;
    delete obligation;
    delete passDown;
    delete passed;
}


void Setup_TCL_Formula(TCL_Formula* F){
    int i;
    Parse_Tree.push_back(F);
    if(F->type==PARSE_ROOT || F->type==PLUS){
        strategy2owner.push_back(F->owner);
        F->index=parseCount;
        parseCount++;
        F->strategy_index=strategyCount;
        strategyCount++;
        F->closure_index=0;
        for(i=0;i<F->outs.size();i++){
            Setup_TCL_Formula(F->outs[i]);
        }
    }
    else if(F->type==MINUS){
        F->index=parseCount;
        parseCount++;
        F->strategy_index=-1;
        F->closure_index=0;
        for(i=0;i<F->outs.size();i++){
            Setup_TCL_Formula(F->outs[i]);
        }
    }
    else if(F->type==ATOMIC){
        F->index=parseCount;
        parseCount++;
        F->closure_index=closureCount;
        Closure.push_back(F);
        closureCount++;
        F->strategy_index=0;
        for(i=0;i<F->outs.size();i++){
            Setup_TCL_Formula(F->outs[i]);
        }
    }
    else{
        F->index=parseCount;
        parseCount++;
        F->closure_index=closureCount;
        Closure.push_back(F);
        closureCount++;
        F->strategy_index=0;
        for(i=0;i<F->outs.size();i++){
            Setup_TCL_Formula(F->outs[i]);
        }
    }
}


void fill_in_matrix(TCL_Formula* F){
    int i;
    switch(F->type){
        case TRUE_NODE:
            for(i=0;i<strategyCount;i++){
                Matrix[i][F->closure_index]=strategy_stack[i];
            }
            break;
        case FALSE_NODE:
            for(i=0;i<strategyCount;i++){
                Matrix[i][F->closure_index]=strategy_stack[i];
            }
            break;
        case PARSE_ROOT:
//            cout<<"PARSE_ROOT:"<<endl;
            for(i=0;i<strategyCount;i++){
                if(strategy2owner[i]==F->owner){
                    strategy_stack[i]=0;
                }
            } //clean the previous strategy of the same player
            strategy_stack[F->strategy_index]= 1;
            fill_in_matrix(F->outs[0]);
            strategy_stack[F->strategy_index]= 0;
//            cout<<"END"<<endl;
            break;
        case ATOMIC:
            for(i=0;i<strategyCount;i++){
                Matrix[i][F->closure_index]=strategy_stack[i];
            }
            break;
    case NOT:
            for(i=0;i<strategyCount;i++){
                Matrix[i][F->closure_index]=strategy_stack[i];
            }
        fill_in_matrix(F->outs[0]);
        break;
        case OR:
            for(i=0;i<strategyCount;i++){
                Matrix[i][F->closure_index]=strategy_stack[i];
            }
            fill_in_matrix(F->outs[0]);
            fill_in_matrix(F->outs[1]);
            break;
        case AND:
            for(i=0;i<strategyCount;i++){
                Matrix[i][F->closure_index]=strategy_stack[i];
            }
            fill_in_matrix(F->outs[0]);
            fill_in_matrix(F->outs[1]);
            break;
        case PLUS:
            for(i=0;i<strategyCount;i++){
                if(strategy2owner[i]==F->owner){
                    strategy_stack[i]=0;
                }
            } //clean the previous strategy of the same player
            strategy_stack[F->strategy_index]= 1;
            fill_in_matrix(F->outs[0]);
            strategy_stack[F->strategy_index]= 0;
            break;
        case MINUS:
            for(i=0;i<strategyCount;i++){
                if(strategy2owner[i]==F->owner){
                    strategy_stack[i]=0;
                }
            } //clean the previous strategy of the same player
            fill_in_matrix(F->outs[0]);
            break;
        case UNTIL:
//            cout<<"UNTIL"<<endl;
            for(i=0;i<strategyCount;i++){
                Matrix[i][F->closure_index]=strategy_stack[i];
            }
            fill_in_matrix(F->outs[0]);
            fill_in_matrix(F->outs[1]);
            break;
        case WNTIL:
            for(i=0;i<strategyCount;i++){
                Matrix[i][F->closure_index]=strategy_stack[i];
            }
            fill_in_matrix(F->outs[0]);
            fill_in_matrix(F->outs[1]);
            break;
        case NEXT:
//            cout<<"NEXT"<<endl;
            for(i=0;i<strategyCount;i++){
                Matrix[i][F->closure_index]=strategy_stack[i];
            }
        fill_in_matrix(F->outs[0]);
            break;
    }
}


int findNextClosure(int x){
    if(Parse_Tree[x]->type==PARSE_ROOT || Parse_Tree[x]->type==PLUS || Parse_Tree[x]->type==MINUS){
        return findNextClosure(Parse_Tree[x]->outs[0]->index);
    }
    else {return Parse_Tree[x]->closure_index;}
}


void setupMatrix(){
    int i,j;
    Matrix=new int*[strategyCount];
    for(i=0;i<strategyCount;i++){
        Matrix[i]=new int[closureCount];
    }
    for(i=0;i<strategyCount;i++){
        for(j=0;j<closureCount;j++){
            Matrix[i][j]=0;
        }
    }
    //setup strategy_stack, conveniently
    strategy_stack=new int[strategyCount];
    for(i=0;i<strategyCount;i++){
        strategy_stack[i]=0;
    }
}


void extractModelFromFile(TCL_Game_Node* root){
//    cout<<root->index<<": "<<red_diagram_string(root->red)<<endl;
    int i;
    for(i=1;i<sxiCount;i++){
        redgram temp_red=red_sync_xtion_fwd(
            root->red,
            path,
            RED_USE_DECLARED_SYNC_XTION,
            i,
            RED_GAME_MODL | RED_GAME_SPEC | RED_GAME_ENVR,
            RED_TIME_PROGRESS,
            RED_NORM_ZONE_CLOSURE,
            RED_NO_ACTION_APPROX,
            RED_REDUCTION_INACTIVE,
            RED_NOAPPROX_MODL_GAME | RED_NOAPPROX_SPEC_GAME
            | RED_NOAPPROX_ENVR_GAME | RED_NOAPPROX_GLOBAL_GAME,
            RED_NO_SYMMETRY,
            0);
        if(red_and(temp_red,red_query_diagram_enhanced_global_invariance())!=red_false()){
            if(node_map[temp_string.assign(red_diagram_string(temp_red))]==0){
                temp_edge=new TCL_Game_Edge;
                temp_node=new TCL_Game_Node;
                temp_edge->src=root;
                temp_edge->dst=temp_node;
                root->outs.push_back(temp_edge);
                temp_node->index=nodeCount; nodeCount++;
                temp_node->red=temp_red;
                temp_node->ins.push_back(temp_edge);
                Nodes.push_back(temp_node);
                node_map[temp_string.assign(red_diagram_string(temp_red))]=temp_node->index;
//                cout<<root->index<<"--"<<i<<"-->"<<temp_node->index<<endl;
                extractModelFromFile(temp_node);
            }
            else{
                temp_node=Nodes[node_map[temp_string.assign(red_diagram_string(temp_red))]];
                temp_edge=new TCL_Game_Edge;
                temp_edge->src=root;
                temp_edge->dst=temp_node;
                root->outs.push_back(temp_edge);
                temp_node->ins.push_back(temp_edge);
//                cout<<root->index<<"--"<<i<<"-->"<<temp_node->index<<endl;
            }
        }
    }
}


void labelPlayerSelectionOnEdge(TCL_Game_Node* root){
    int lb,hb;
    vector<string> selectionName;
    selectionName.push_back("p1_choose");
    selectionName.push_back("p2_choose");
    char temp[20];
    int i,j,k;
    for(i=0;i<nodeCount;i++){
        for(j=0;j<Nodes[i]->ins.size();j++){
            for(k=0;k<selectionName.size();k++){
                strcpy(temp,selectionName[k].c_str());
                red_get_cube_discrete_value(Nodes[i]->red,temp,&lb,&hb);
                Nodes[i]->ins[j]->selectionArray.push_back(lb);            
            }
        }
    }
}


bool check_parent(int a,int b){
    if(a==b){return true;}
    else{
        if(a>b){
            while(a!=0){
                a=Parse_Tree[a]->ins[0]->index;
                if(a==b){return false;}
            }
        }
        else if(a<b){
            while(b!=0){
                b=Parse_Tree[b]->ins[0]->index;
                if(a==b){return false;}
            }
        }
    }
    return true;
}


int checkVisited(Computation_Tree_Node* R){
    cout<<"check visited"<<endl;
    int i;
    if(R->until_token_old==0){
        R->until_token=(R->until_token_old+1)%(closureCount+1);
    }
    else if(Closure[R->until_token_old-1]->type==UNTIL && R->guessedSolution[R->until_token_old-1]==TRUE_GUESSED_PHASE_2){
        R->until_token=R->until_token_old;
    }
    else if(Closure[R->until_token_old-1]->type==WNTIL && R->guessedSolution[R->until_token_old-1]==FALSE_GUESSED_PHASE_2){
        R->until_token=R->until_token_old;
    }    
    else{
        R->until_token=(R->until_token_old+1)%(closureCount+1);
        while(R->until_token!=0 && (Closure[R->until_token-1]->type!=UNTIL && Closure[R->until_token-1]->type!=WNTIL)){
            R->until_token=(R->until_token+1)%(closureCount+1);
        }
    }

    cout<<"checkVisited(state,token):("<<R->state->index<<","<<R->until_token<<")"<<endl;
    
    
    Computation_Tree_Node* ancestor;
    ancestor=R;
    int local_index;
    local_index=R->state->index;
    bool token_changed;
    token_changed=false;
    while(ancestor->ins!=NULL){
        ancestor=ancestor->ins;
        if(ancestor->until_token!=R->until_token){token_changed=true;}
        else{
            if(ancestor->state->index==local_index){
                bool diff_G;
                diff_G=false;
                for(i=0;i<closureCount;i++){
                    if(R->guessedSolution[i]!=ancestor->guessedSolution[i]){diff_G=true;}
                }
                if(diff_G==false){
                    if(token_changed && R->until_token==0){
                        cout<<"visited and pass"<<endl;
                        return PASS;
                    }
                    else{
                        cout<<"visited and fail"<<endl;
                        return FAIL;
                    }
                }
            }
        }
    }
    cout<<"unvisited"<<endl;
    return UNVISITED;
}


void guessFirstPossibleSolution(Computation_Tree_Node* R){
    int i,j;
    for(i=0;i<closureCount;i++){
        R->guessedSolution[i]=R->obligation[i];
    }
    for(i=0;i<closureCount;i++){
        if(R->guessedSolution[i]==MUST_TRUE){
            switch(Closure[i]->type){
                int left,right;
                case OR:
                    left=findNextClosure(Closure[i]->outs[0]->index);
                    right=findNextClosure(Closure[i]->outs[1]->index);
                    R->guessedSolution[i]=TRUE_GUESSED_PHASE_1;
                    R->guessedSolution[left]=MUST_TRUE;
                    break;
                case AND:
                    left=findNextClosure(Closure[i]->outs[0]->index);
                    right=findNextClosure(Closure[i]->outs[1]->index);
                    R->guessedSolution[i]=MUST_TRUE;
                    R->guessedSolution[left]=MUST_TRUE;
                    R->guessedSolution[right]=MUST_TRUE;
                    break;
                case UNTIL:
                    //not sure
                    left=findNextClosure(Closure[i]->outs[0]->index);
                    right=findNextClosure(Closure[i]->outs[1]->index);
                    R->guessedSolution[i]=TRUE_GUESSED_PHASE_1;
                    R->guessedSolution[right]=MUST_TRUE;
                    break;
                case WNTIL:
                    //not sure
                    left=findNextClosure(Closure[i]->outs[0]->index);
                    right=findNextClosure(Closure[i]->outs[1]->index);
                    R->guessedSolution[i]=TRUE_GUESSED_PHASE_1;
                    R->guessedSolution[right]=MUST_TRUE;
                    break;
                case NOT:
                    R->guessedSolution[i]=DONT_CARE;
                    R->guessedSolution[findNextClosure(Closure[i]->outs[0]->index)]=MUST_FALSE;
                    break;
                default:
                    R->guessedSolution[i]=MUST_TRUE;
            }
        }
        else if(R->guessedSolution[i]==MUST_FALSE){
            switch(Closure[i]->type){
                int left,right;
                case OR:
                    left=findNextClosure(Closure[i]->outs[0]->index);
                    right=findNextClosure(Closure[i]->outs[1]->index);
                    R->guessedSolution[i]=DONT_CARE;
                    R->guessedSolution[left]=MUST_FALSE;
                    R->guessedSolution[right]=MUST_FALSE;
                    break;
                case AND:
                    left=findNextClosure(Closure[i]->outs[0]->index);
                    right=findNextClosure(Closure[i]->outs[1]->index);
                    R->guessedSolution[i]=FALSE_GUESSED_PHASE_1;
                    R->guessedSolution[left]=MUST_FALSE;
                    break;
                case UNTIL:
                    //Not sure
                    left=findNextClosure(Closure[i]->outs[0]->index);
                    right=findNextClosure(Closure[i]->outs[1]->index);
                    R->guessedSolution[i]=FALSE_GUESSED_PHASE_1;
                    R->guessedSolution[left]=MUST_FALSE;
                    R->guessedSolution[right]=MUST_FALSE;
                    break;
                case WNTIL:
                    //not sure
                    left=findNextClosure(Closure[i]->outs[0]->index);
                    right=findNextClosure(Closure[i]->outs[1]->index);
                    R->guessedSolution[i]=FALSE_GUESSED_PHASE_1;
                    R->guessedSolution[left]=MUST_FALSE;
                    R->guessedSolution[right]=MUST_FALSE;
                    break;
                case NOT:
                    R->guessedSolution[i]=DONT_CARE;
                    R->guessedSolution[findNextClosure(Closure[i]->outs[0]->index)]=MUST_TRUE;
                    break;
                default:
                    R->guessedSolution[i]=MUST_FALSE;
            }
        }
    }
    cout<<"After first guess:";
    for(i=0;i<closureCount;i++){
        cout<<" "<<R->guessedSolution[i];
    }
    cout<<endl;
}


bool Guess(Computation_Tree_Node* R){
    int i,j;
    cout<<"Previous possible solution before guess:";
    for(i=0;i<closureCount;i++){
        cout<<" "<<R->guessedSolution[i];
    }
    cout<<endl;
    bool carry;
    carry=true;
    for(i=closureCount-1;i>=0;i--){
        if(carry){
            if(R->guessedSolution[i]==TRUE_GUESSED_PHASE_1){
                R->guessedSolution[i]=TRUE_GUESSED_PHASE_2;
                carry=false;
            }
            else if(R->guessedSolution[i]==TRUE_GUESSED_PHASE_2){
                R->guessedSolution[i]=TRUE_GUESSED_PHASE_1;
                carry=true;
            }
            else if(R->guessedSolution[i]==FALSE_GUESSED_PHASE_1){
                R->guessedSolution[i]=FALSE_GUESSED_PHASE_2;
                carry=false;
            }
            else if(R->guessedSolution[i]==FALSE_GUESSED_PHASE_2){
                R->guessedSolution[i]=FALSE_GUESSED_PHASE_1;
                carry=true;
            }
        }
    }
    if(carry){
        cout<<"No other possibility"<<endl;
        return false;
    }
    cout<<"After guess:";
    for(i=0;i<closureCount;i++){
        cout<<R->guessedSolution[i]<<" ";
    }
    cout<<endl;

    for(i=0;i<closureCount;i++){
        cout<<"Rebuild a new guess tree"<<i<<": ";
        for(j=0;j<closureCount;j++){
            cout<<R->guessedSolution[j]<<" ";
        }
        cout<<endl;
        if(R->guessedSolution[i]==MUST_TRUE){
            switch(Closure[i]->type){
                int left,right;
                case OR:
                    left=findNextClosure(Closure[i]->outs[0]->index);
                    right=findNextClosure(Closure[i]->outs[1]->index);
                    R->guessedSolution[i]=TRUE_GUESSED_PHASE_1;
                    if(R->obligation[left]==DONT_CARE){
                        R->guessedSolution[left]=MUST_TRUE;
                    }
                    if(R->obligation[right]==DONT_CARE){
                        R->guessedSolution[right]=DONT_CARE;
                    }
                    break;
                case AND:
                    left=findNextClosure(Closure[i]->outs[0]->index);
                    right=findNextClosure(Closure[i]->outs[1]->index);
                    R->guessedSolution[i]=MUST_TRUE;
                    if(R->guessedSolution[left]==DONT_CARE){
                        R->guessedSolution[left]=MUST_TRUE;
                    }
                    if(R->guessedSolution[right]==DONT_CARE){
                        R->guessedSolution[right]=MUST_TRUE;
                    }
                    break;
                case UNTIL:
                    //not sure
                    left=findNextClosure(Closure[i]->outs[0]->index);
                    right=findNextClosure(Closure[i]->outs[1]->index);
                    R->guessedSolution[i]=TRUE_GUESSED_PHASE_1;
                    if(R->obligation[left]==DONT_CARE){
                        R->guessedSolution[left]=DONT_CARE;
                    }
                    if(R->obligation[right]==DONT_CARE){
                        R->guessedSolution[right]=MUST_TRUE;
                    }
                    break;
                case WNTIL:
                    left=findNextClosure(Closure[i]->outs[0]->index);
                    right=findNextClosure(Closure[i]->outs[1]->index);
                    R->guessedSolution[i]=TRUE_GUESSED_PHASE_1;
                    if(R->obligation[left]==DONT_CARE){
                        R->guessedSolution[left]=DONT_CARE;
                    }
                    if(R->obligation[right]==DONT_CARE){
                        R->guessedSolution[right]=MUST_TRUE;
                    }
                    break;
                case NEXT:
                    if(R->obligation[findNextClosure(Closure[i]->outs[0]->index)]==DONT_CARE){
                        R->guessedSolution[findNextClosure(Closure[i]->outs[0]->index)]=DONT_CARE;
                    }
                    break;
                case NOT:
                    R->guessedSolution[i]=DONT_CARE;
                    if(R->obligation[findNextClosure(Closure[i]->outs[0]->index)]==DONT_CARE){
                        R->guessedSolution[findNextClosure(Closure[i]->outs[0]->index)]=MUST_FALSE;
                    }
                    break;
                default:
                    R->guessedSolution[i]=MUST_TRUE;
            }
        }
        else if(R->guessedSolution[i]==MUST_FALSE){
            switch(Closure[i]->type){
                int left,right;
                case OR:
                    left=findNextClosure(Closure[i]->outs[0]->index);
                    right=findNextClosure(Closure[i]->outs[1]->index);
                    R->guessedSolution[i]=MUST_FALSE;
                    if(R->guessedSolution[left]==DONT_CARE){
                        R->guessedSolution[left]=MUST_FALSE;
                    }
                    if(R->guessedSolution[right]==DONT_CARE){
                        R->guessedSolution[right]=MUST_FALSE;
                    }
                    break;
                case AND:
                    left=findNextClosure(Closure[i]->outs[0]->index);
                    right=findNextClosure(Closure[i]->outs[1]->index);
                    R->guessedSolution[i]=FALSE_GUESSED_PHASE_1;
                    if(R->obligation[left]==DONT_CARE){
                        R->guessedSolution[left]=MUST_FALSE;
                    }
                    if(R->obligation[right]==DONT_CARE){
                        R->guessedSolution[right]=DONT_CARE;
                    }
                    break;
                case UNTIL:
                    left=findNextClosure(Closure[i]->outs[0]->index);
                    right=findNextClosure(Closure[i]->outs[1]->index);
                    R->guessedSolution[i]=FALSE_GUESSED_PHASE_1;
                    if(R->obligation[left]==DONT_CARE){
                        R->guessedSolution[left]=MUST_FALSE;
                    }
                    if(R->obligation[right]==DONT_CARE){
                        R->guessedSolution[right]=MUST_FALSE;
                    }
                    break;
                case WNTIL:
                    left=findNextClosure(Closure[i]->outs[0]->index);
                    right=findNextClosure(Closure[i]->outs[1]->index);
                    R->guessedSolution[i]=FALSE_GUESSED_PHASE_1;
                    if(R->obligation[left]==DONT_CARE){
                        R->guessedSolution[left]=MUST_FALSE;
                    }
                    if(R->obligation[right]==DONT_CARE){
                        R->guessedSolution[right]=MUST_FALSE;
                    }
                    break;
                case NEXT:
                    if(R->obligation[findNextClosure(Closure[i]->outs[0]->index)]==DONT_CARE){
                        R->guessedSolution[findNextClosure(Closure[i]->outs[0]->index)]=DONT_CARE;
                    }
                    break;
                case NOT:
                    R->guessedSolution[i]=DONT_CARE;
                    R->guessedSolution[findNextClosure(Closure[i]->outs[0]->index)]=MUST_TRUE;
                    break;
                default:
                    R->guessedSolution[i]=MUST_FALSE;
            }
        }
        else if(R->guessedSolution[i]==TRUE_GUESSED_PHASE_1){
            switch(Closure[i]->type){
                int left,right;
                case OR:
                    left=findNextClosure(Closure[i]->outs[0]->index);
                    right=findNextClosure(Closure[i]->outs[1]->index);
                    R->guessedSolution[i]=TRUE_GUESSED_PHASE_1;
                    if(R->guessedSolution[left]==TRUE_GUESSED_PHASE_1 || R->guessedSolution[left]==TRUE_GUESSED_PHASE_2
                    ||R->guessedSolution[left]==FALSE_GUESSED_PHASE_1 || R->guessedSolution[left]==FALSE_GUESSED_PHASE_2){
                        R->guessedSolution[left]=R->guessedSolution[left];
                    }
                    else{R->guessedSolution[left]=MUST_TRUE;}
                    if(R->obligation[right]==DONT_CARE){
                        R->guessedSolution[right]=DONT_CARE;
                    }
                    break;
                case UNTIL:
                    left=findNextClosure(Closure[i]->outs[0]->index);
                    right=findNextClosure(Closure[i]->outs[1]->index);
                    R->guessedSolution[i]=TRUE_GUESSED_PHASE_1;
                    if(R->obligation[left]==DONT_CARE){
                        R->guessedSolution[left]=DONT_CARE;
                    }
                    if(R->guessedSolution[right]==TRUE_GUESSED_PHASE_1 || R->guessedSolution[right]==TRUE_GUESSED_PHASE_2
                    ||R->guessedSolution[right]==FALSE_GUESSED_PHASE_1 || R->guessedSolution[right]==FALSE_GUESSED_PHASE_2){
                        R->guessedSolution[right]=R->guessedSolution[right];
                    }
                    else{R->guessedSolution[right]=MUST_TRUE;}
                    break;
                case WNTIL:
                    left=findNextClosure(Closure[i]->outs[0]->index);
                    right=findNextClosure(Closure[i]->outs[1]->index);
                    R->guessedSolution[i]=TRUE_GUESSED_PHASE_1;
                    if(R->obligation[left]==DONT_CARE){
                        R->guessedSolution[left]=DONT_CARE;
                    }
                    if(R->guessedSolution[right]==TRUE_GUESSED_PHASE_1 || R->guessedSolution[right]==TRUE_GUESSED_PHASE_2
                    ||R->guessedSolution[right]==FALSE_GUESSED_PHASE_1 || R->guessedSolution[right]==FALSE_GUESSED_PHASE_2){
                        R->guessedSolution[right]=R->guessedSolution[right];
                    }
                    else{R->guessedSolution[right]=MUST_TRUE;}
                    break;
            }
        }
        else if(R->guessedSolution[i]==TRUE_GUESSED_PHASE_2){
            switch(Closure[i]->type){
                int left,right;
                case OR:
                    left=findNextClosure(Closure[i]->outs[0]->index);
                    right=findNextClosure(Closure[i]->outs[1]->index);
                    R->guessedSolution[i]=TRUE_GUESSED_PHASE_2;
                    if(R->obligation[left]==DONT_CARE){
                        R->guessedSolution[left]=DONT_CARE;
                    }
                    if(R->guessedSolution[right]==TRUE_GUESSED_PHASE_1 || R->guessedSolution[right]==TRUE_GUESSED_PHASE_2
                    ||R->guessedSolution[right]==FALSE_GUESSED_PHASE_1 || R->guessedSolution[right]==FALSE_GUESSED_PHASE_2){
                        R->guessedSolution[right]=R->guessedSolution[right];
                    }
                    else{R->guessedSolution[right]=MUST_TRUE;}
                    break;
                case UNTIL:
                    left=findNextClosure(Closure[i]->outs[0]->index);
                    right=findNextClosure(Closure[i]->outs[1]->index);
                    R->guessedSolution[i]=TRUE_GUESSED_PHASE_2;
                    if(R->guessedSolution[left]==TRUE_GUESSED_PHASE_1 || R->guessedSolution[left]==TRUE_GUESSED_PHASE_2
                    ||R->guessedSolution[left]==FALSE_GUESSED_PHASE_1 || R->guessedSolution[left]==FALSE_GUESSED_PHASE_2){
                        R->guessedSolution[left]=R->guessedSolution[left];
                    }
                    else{R->guessedSolution[left]=MUST_TRUE;}
                    if(R->obligation[right]==DONT_CARE){
                        R->guessedSolution[right]=DONT_CARE;
                    }
                    break;
                case WNTIL:
                    left=findNextClosure(Closure[i]->outs[0]->index);
                    right=findNextClosure(Closure[i]->outs[1]->index);
                    R->guessedSolution[i]=TRUE_GUESSED_PHASE_2;
                    if(R->guessedSolution[left]==TRUE_GUESSED_PHASE_1 || R->guessedSolution[left]==TRUE_GUESSED_PHASE_2
                    ||R->guessedSolution[left]==FALSE_GUESSED_PHASE_1 || R->guessedSolution[left]==FALSE_GUESSED_PHASE_2){
                        R->guessedSolution[left]=R->guessedSolution[left];
                    }
                    else{R->guessedSolution[left]=MUST_TRUE;}
                    if(R->obligation[right]==DONT_CARE){
                        R->guessedSolution[right]=DONT_CARE;
                    }
                    break;
            }
        }
        else if(R->guessedSolution[i]==FALSE_GUESSED_PHASE_1){
            switch(Closure[i]->type){
                int left,right;
                case AND:
                    left=findNextClosure(Closure[i]->outs[0]->index);
                    right=findNextClosure(Closure[i]->outs[1]->index);
                    R->guessedSolution[i]=FALSE_GUESSED_PHASE_1;
                    if(R->guessedSolution[left]==TRUE_GUESSED_PHASE_1 || R->guessedSolution[left]==TRUE_GUESSED_PHASE_2
                    ||R->guessedSolution[left]==FALSE_GUESSED_PHASE_1 || R->guessedSolution[left]==FALSE_GUESSED_PHASE_2){
                        R->guessedSolution[left]=R->guessedSolution[left];
                    }
                    else{R->guessedSolution[left]=MUST_FALSE;}
                    if(R->obligation[right]==DONT_CARE){
                        R->guessedSolution[right]=DONT_CARE;
                    }
                    break;
                case UNTIL:
                    left=findNextClosure(Closure[i]->outs[0]->index);
                    right=findNextClosure(Closure[i]->outs[1]->index);
                    R->guessedSolution[i]=FALSE_GUESSED_PHASE_1;
                    if(R->guessedSolution[left]==TRUE_GUESSED_PHASE_1 || R->guessedSolution[left]==TRUE_GUESSED_PHASE_2
                    ||R->guessedSolution[left]==FALSE_GUESSED_PHASE_1 || R->guessedSolution[left]==FALSE_GUESSED_PHASE_2){
                        R->guessedSolution[left]=R->guessedSolution[left];
                    }
                    else{R->guessedSolution[left]=MUST_FALSE;}
                    if(R->guessedSolution[right]==TRUE_GUESSED_PHASE_1 || R->guessedSolution[right]==TRUE_GUESSED_PHASE_2
                    ||R->guessedSolution[right]==FALSE_GUESSED_PHASE_1 || R->guessedSolution[right]==FALSE_GUESSED_PHASE_2){
                        R->guessedSolution[right]=R->guessedSolution[right];
                    }
                    else{R->guessedSolution[right]=MUST_FALSE;}
                    break;
                case WNTIL:
                    left=findNextClosure(Closure[i]->outs[0]->index);
                    right=findNextClosure(Closure[i]->outs[1]->index);
                    R->guessedSolution[i]=FALSE_GUESSED_PHASE_1;
                    if(R->guessedSolution[left]==TRUE_GUESSED_PHASE_1 || R->guessedSolution[left]==TRUE_GUESSED_PHASE_2
                    ||R->guessedSolution[left]==FALSE_GUESSED_PHASE_1 || R->guessedSolution[left]==FALSE_GUESSED_PHASE_2){
                        R->guessedSolution[left]=R->guessedSolution[left];
                    }
                    else{R->guessedSolution[left]=MUST_FALSE;}
                    if(R->guessedSolution[right]==TRUE_GUESSED_PHASE_1 || R->guessedSolution[right]==TRUE_GUESSED_PHASE_2
                    ||R->guessedSolution[right]==FALSE_GUESSED_PHASE_1 || R->guessedSolution[right]==FALSE_GUESSED_PHASE_2){
                        R->guessedSolution[right]=R->guessedSolution[right];
                    }
                    else{R->guessedSolution[right]=MUST_FALSE;}                                        
                    break;
            }
        }
        else if(R->guessedSolution[i]==FALSE_GUESSED_PHASE_2){
            switch(Closure[i]->type){
                int left,right;
                case AND:
                    left=findNextClosure(Closure[i]->outs[0]->index);
                    right=findNextClosure(Closure[i]->outs[1]->index);
                    R->guessedSolution[i]=FALSE_GUESSED_PHASE_2;
                    if(R->obligation[left]==DONT_CARE){
                        R->guessedSolution[left]=DONT_CARE;
                    }
                    if(R->guessedSolution[right]==TRUE_GUESSED_PHASE_1 || R->guessedSolution[right]==TRUE_GUESSED_PHASE_2
                    ||R->guessedSolution[right]==FALSE_GUESSED_PHASE_1 || R->guessedSolution[right]==FALSE_GUESSED_PHASE_2){
                        R->guessedSolution[right]=R->guessedSolution[right];
                    }
                    else{R->guessedSolution[right]=MUST_FALSE;}
                    break;
                case UNTIL:
                    left=findNextClosure(Closure[i]->outs[0]->index);
                    right=findNextClosure(Closure[i]->outs[1]->index);
                    R->guessedSolution[i]=FALSE_GUESSED_PHASE_2;
                    if(R->guessedSolution[left]==TRUE_GUESSED_PHASE_1 || R->guessedSolution[left]==TRUE_GUESSED_PHASE_2
                    ||R->guessedSolution[left]==FALSE_GUESSED_PHASE_1 || R->guessedSolution[left]==FALSE_GUESSED_PHASE_2){
                        R->guessedSolution[left]=R->guessedSolution[left];
                    }
                    else{R->guessedSolution[left]=MUST_TRUE;}
                    if(R->guessedSolution[right]==TRUE_GUESSED_PHASE_1 || R->guessedSolution[right]==TRUE_GUESSED_PHASE_2
                    ||R->guessedSolution[right]==FALSE_GUESSED_PHASE_1 || R->guessedSolution[right]==FALSE_GUESSED_PHASE_2){
                        R->guessedSolution[right]=R->guessedSolution[right];
                    }
                    else{R->guessedSolution[right]=MUST_FALSE;}
                    break;
                case WNTIL:
                    left=findNextClosure(Closure[i]->outs[0]->index);
                    right=findNextClosure(Closure[i]->outs[1]->index);
                    R->guessedSolution[i]=FALSE_GUESSED_PHASE_2;
                    if(R->guessedSolution[left]==TRUE_GUESSED_PHASE_1 || R->guessedSolution[left]==TRUE_GUESSED_PHASE_2
                    ||R->guessedSolution[left]==FALSE_GUESSED_PHASE_1 || R->guessedSolution[left]==FALSE_GUESSED_PHASE_2){
                        R->guessedSolution[left]=R->guessedSolution[left];
                    }
                    else{R->guessedSolution[left]=MUST_TRUE;}
                    if(R->guessedSolution[right]==TRUE_GUESSED_PHASE_1 || R->guessedSolution[right]==TRUE_GUESSED_PHASE_2
                    ||R->guessedSolution[right]==FALSE_GUESSED_PHASE_1 || R->guessedSolution[right]==FALSE_GUESSED_PHASE_2){
                        R->guessedSolution[right]=R->guessedSolution[right];
                    }
                    else{R->guessedSolution[right]=MUST_FALSE;}
                    break;
            }
        }
        else if(R->guessedSolution[i]==DONT_CARE){
            switch(Closure[i]->type){
                int left,right;
                case OR:
                    left=findNextClosure(Closure[i]->outs[0]->index);
                    right=findNextClosure(Closure[i]->outs[1]->index);
                    if(R->obligation[left]==DONT_CARE){
                        R->guessedSolution[left]=DONT_CARE;
                    }
                    if(R->obligation[right]==DONT_CARE){
                        R->guessedSolution[right]=DONT_CARE;
                    }
                    break;
                case AND:
                    left=findNextClosure(Closure[i]->outs[0]->index);
                    right=findNextClosure(Closure[i]->outs[1]->index);
                    if(R->obligation[left]==DONT_CARE){
                        R->guessedSolution[left]=DONT_CARE;
                    }
                    if(R->obligation[right]==DONT_CARE){
                        R->guessedSolution[right]=DONT_CARE;
                    }
                    break;
                case UNTIL:
                    left=findNextClosure(Closure[i]->outs[0]->index);
                    right=findNextClosure(Closure[i]->outs[1]->index);
                    if(R->obligation[left]==DONT_CARE){
                        R->guessedSolution[left]=DONT_CARE;
                    }
                    if(R->obligation[right]==DONT_CARE){
                        R->guessedSolution[right]=DONT_CARE;
                    }
                    break;
                case WNTIL:
                    left=findNextClosure(Closure[i]->outs[0]->index);
                    right=findNextClosure(Closure[i]->outs[1]->index);
                    if(R->obligation[left]==DONT_CARE){
                        R->guessedSolution[left]=DONT_CARE;
                    }
                    if(R->obligation[right]==DONT_CARE){
                        R->guessedSolution[right]=DONT_CARE;
                    }
                    break;
                case NEXT:
                    R->guessedSolution[i]=DONT_CARE;
                    if(R->obligation[findNextClosure(Closure[i]->outs[0]->index)]==DONT_CARE){
                        R->guessedSolution[findNextClosure(Closure[i]->outs[0]->index)]=DONT_CARE;
                    }
                    break;
                case NOT:
                    R->guessedSolution[i]=DONT_CARE;
                    if(R->obligation[findNextClosure(Closure[i]->outs[0]->index)]==DONT_CARE){
                        R->guessedSolution[findNextClosure(Closure[i]->outs[0]->index)]=DONT_CARE;
                    }
                    break;
                default:
                    R->guessedSolution[i]=DONT_CARE;
            }
        }
    }
    for(i=0;i<closureCount;i++){
        cout<<" "<<R->guessedSolution[i];
    }
    cout<<endl;
    return true;
}


int checkLocalUnSAT(Computation_Tree_Node* R){
    cout<<"check local"<<endl;
    int i;
    int checkVisitedResult;
    checkVisitedResult=checkVisited(R);

    if(checkVisitedResult==PASS){
        cout<<"check local PASS"<<endl;    
        return PASS;
    }
    else if(checkVisitedResult==FAIL){
        cout<<"check local fail"<<endl;            
        return FAIL;
    }
    else{
        for(i=closureCount-1;i>=0;i--){
            if(R->guessedSolution[i]!=DONT_CARE){
                switch(Closure[i]->type){
                    int left,right;
                    case TRUE_NODE:
                        if(R->guessedSolution[i]==MUST_FALSE){
                            cout<<"check local fail "<<Closure[i]->index<<endl;                        
                            return FAIL;
                        }
                    break;
                    case FALSE_NODE:
                        if(R->guessedSolution[i]==MUST_TRUE){
                            cout<<"check local fail "<<Closure[i]->index<<endl;
                            return FAIL;
                        }
                    break;
                    case ATOMIC:
                        char aaa[100];
                        strcpy(aaa,Closure[i]->str.c_str());
                        if(red_and(red_diagram(aaa),R->state->red)!=red_false()){
                            if(R->guessedSolution[i]==MUST_FALSE){
                                cout<<"check local fail "<<Closure[i]->index<<endl;    
                                return FAIL;
                            }
                        }
                        else{
                            if(R->guessedSolution[i]==MUST_TRUE){
                                cout<<"check local fail "<<Closure[i]->index<<endl;
                                return FAIL;
                            }
                        }
                    break;
                }
            }
        }
        cout<<"check local continue"<<endl;
        return CONTINUE;
    }
}


bool Create_passDown(Computation_Tree_Node* R,bool &controlled){
/*    cout<<"create pass down:"<<endl;
    bool flag;
    flag=false;
    int i,j,x,y;
    for(i=0;i<closureCount;i++){
        cout<<R->guessedSolution[i]<<" ";
    }
    cout<<endl;
    for(i=0;i<closureCount;i++){
        cout<<R->passed[i]<<" ";
    }
    cout<<endl;    
    for(i=0;i<closureCount;i++){
        if(R->passed[i]==false && ((R->guessedSolution[i]==MUST_TRUE && Closure[i]->type==NEXT)
            ||(R->guessedSolution[i]==MUST_FALSE && Closure[i]->type==NEXT)
            ||(R->guessedSolution[i]==TRUE_GUESSED_PHASE_2 &&(Closure[i]->type==UNTIL || Closure[i]->type==WNTIL))
            ||(R->guessedSolution[i]==FALSE_GUESSED_PHASE_2 && (Closure[i]->type==UNTIL || Closure[i]->type==WNTIL)))){
            flag=true;
            x=i;
            cout<<"closure "<<i;
            for(j=0;j<strategyCount;j++){
                if(Matrix[j][i]==1 && strategy2owner[j]==R->state->owner){
                    y=j;
                    controlled=true;
                    cout<<" controlled by strategy "<<j<<" of player "<<strategy2owner[j]<<endl;
                }
                else {
                    controlled==false;
                    cout<<" uncontrolled"<<endl;
                }
            }
        }
        if(flag==true){
            break;
        }
    }
    if(flag==false){ 
        cout<<"nothing to pass down"<<endl;
        return false;
    }
    cout<<x<<" "<<y<<endl;
    for(i=0;i<closureCount;i++){
        R->passDown[i]=DONT_CARE;
    }
    if(controlled==true){
        for(i=0;i<closureCount;i++){
            if(Matrix[y][i]==1 && R->guessedSolution[i]==MUST_TRUE && Closure[i]->type==NEXT && R->passed[i]==false){
        R->passDown[i]=DONT_CARE;
        R->passDown[findNextClosure(Closure[i]->outs[0]->index)]=MUST_TRUE;
        R->passed[i]=true;
            }
      else if(Matrix[y][i]==1 && R->guessedSolution[i]==TRUE_GUESSED_PHASE_2 && (Closure[i]->type==UNTIL || Closure[i]->type==WNTIL)&& R->passed[i]==false){
                R->passDown[i]=MUST_TRUE;
        R->passed[i]=true;
            }
            else if(Matrix[y][i]==1 && R->guessedSolution[i]==MUST_FALSE && Closure[i]->type==NEXT&& R->passed[i]==false){
        R->passDown[i]=DONT_CARE;
        R->passDown[findNextClosure(Closure[i]->outs[0]->index)]=MUST_FALSE;
        R->passed[i]=true;
        }
      else if(Matrix[y][i]==1 && R->guessedSolution[i]==FALSE_GUESSED_PHASE_2 && (Closure[i]->type==UNTIL || Closure[i]->type==WNTIL) && R->passed[i]==false){
            R->passDown[i]=MUST_FALSE;
        R->passed[i]=true;
            }
        }
    }
    else{
        for(i=0;i<closureCount;i++){
            R->passDown[i]=DONT_CARE;
        }
    if(R->guessedSolution[x]==MUST_TRUE && Closure[x]->type==NEXT && R->passed[i]==false){
      R->passDown[x]=DONT_CARE;
      R->passDown[findNextClosure(Closure[x]->outs[0]->index)]=MUST_TRUE;
            R->passed[x]=true;
    }
    else if(R->guessedSolution[x]==TRUE_GUESSED_PHASE_2 && (Closure[x]->type==UNTIL || Closure[x]->type==WNTIL) && R->passed[i]==false){
      R->passDown[x]=MUST_TRUE;
            R->passed[x]=true;
    }
    else if(R->guessedSolution[x]==MUST_FALSE && Closure[x]->type==NEXT && R->passed[i]==false){
      R->passDown[x]=DONT_CARE;
      R->passDown[findNextClosure(Closure[x]->outs[0]->index)]=MUST_FALSE;
            R->passed[x]=true;
    }
    else if(R->guessedSolution[x]==FALSE_GUESSED_PHASE_2 && (Closure[i]->type==UNTIL || Closure[i]->type==WNTIL) && R->passed[i]==false){
      R->passDown[x]=MUST_FALSE;
            R->passed[x]=true;
    }
    }
    cout<<"pass down created:";    
    for(i=0;i<closureCount;i++){
        cout<<R->passDown[i]<<" ";
    }
    cout<<endl;
    for(i=0;i<closureCount;i++){
        cout<<R->passed[i]<<" ";
    }
    cout<<endl;
    return true;*/
}


bool Check_TCL(Computation_Tree_Node* R){
    int i,j;
    int localCheckResult;    
    cout<<"check TCL on: "<<red_diagram_string(R->state->red)<<endl;
    cout<<"obligation: ";
    for(i=0;i<closureCount;i++){
        cout<<R->obligation[i]<<" ";
    }
    cout<<endl;
    guessFirstPossibleSolution(R);
    localCheckResult=checkLocalUnSAT(R);
    
    if(localCheckResult==PASS){
        if(debugCheckTCL){
            cout<<"checkTCL PASS(local)"<<endl;
        }
        return true;
    }
    /*
    else if(localCheckResult==FAIL){
        while(Guess(R)){
            localCheckResult=checkLocalUnSAT(R);
            if(localCheckResult==PASS){
                cout<<"Check_TCL true"<<endl;
                return true;
            }            
            else if(localCheckResult==CONTINUE){
                for(i=0;i<closureCount;i++){
                    R->passed[i]=false;
                }
                bool controlled=false;
            bool guess_fail;
            guess_fail=false;
                while(Create_passDown(R,controlled) && !guess_fail){
                    bool passDown_success;
                    passDown_success=false;
                    for(i=0;i<R->state->outs.size();i++){
              if(!passDown_success){
                        cout<<"check child "<<i+1<<"/"<<R->state->outs.size()<<endl;
                Computation_Tree_Node* R2=new Computation_Tree_Node;
                R2->state=R->state->outs[i]->dst;
                R2->ins=R;
                R->outs.push_back(R2);
                for(j=0;j<closureCount;j++){
                    R2->obligation[j]=R->passDown[j];
                }
                R2->until_token_old=R->until_token;
                if(controlled){
                    if(Check_TCL(R2)){
                    guess_fail=false;
                    passDown_success=true;
                    }
                } 
                else{
                  if(!Check_TCL(R2)){
                    guess_fail=true;
                    cout<<"guess_fail"<<endl;
                  }
                }
                        }
                    }
                    if(controlled && !passDown_success){guess_fail=true;}
                }
                if(!guess_fail){
              cout<<"guess success"<<endl;
            return true;
                }
            }
            else{
        cout<<"guess_fail"<<endl;
      }
        }
    }
    else if(localCheckResult==CONTINUE){
        for(i=0;i<closureCount;i++){
            R->passed[i]=false;
        }
    bool controlled=false;
    bool guess_fail;
    guess_fail=false;        
        while(Create_passDown(R,controlled) && !guess_fail){
            bool passDown_success;
            passDown_success=false;
            for(i=0;i<R->state->outs.size();i++){
        if(!passDown_success){
                cout<<"check child "<<i+1<<"/"<<R->state->outs.size()<<endl;
          Computation_Tree_Node* R2=new Computation_Tree_Node;
          R2->state=R->state->outs[i]->dst;
          R2->ins=R;
          R->outs.push_back(R2);
          for(j=0;j<closureCount;j++){
            R2->obligation[j]=R->passDown[j];
          }
             R2->until_token_old=R->until_token;
          if(controlled){
            if(Check_TCL(R2)){
              guess_fail=false;
              passDown_success=true;
            }
          }
          else{
            if(!Check_TCL(R2)){
              guess_fail=true;
              cout<<"guess_fail"<<endl;
            }
          }
                }
            }
            if(controlled && !passDown_success){guess_fail=true;}
        }
    if(!guess_fail){
      cout<<"guess success"<<endl;
      return true;
    }
    else{
        guess_fail=false;
      while(Guess(R)){
                localCheckResult=checkLocalUnSAT(R);
                if(localCheckResult==PASS){
                    cout<<"Check_TCL true"<<endl;
                    return true;
                }            
                else if(localCheckResult==CONTINUE){
                    for(i=0;i<closureCount;i++){
                        R->passed[i]=false;
                    }
            bool controlled=false;
                    while(Create_passDown(R,controlled) && !guess_fail){
                        bool passDown_success;
                        passDown_success=false;
                        for(i=0;i<R->state->outs.size();i++){
                    if(!passDown_success){
                            cout<<"check child "<<i+1<<"/"<<R->state->outs.size()<<endl;
                      Computation_Tree_Node* R2=new Computation_Tree_Node;
                      R2->state=R->state->outs[i]->dst;
                      R2->ins=R;
                      R->outs.push_back(R2);
                      for(j=0;j<closureCount;j++){
                        R2->obligation[j]=R->passDown[j];
                      }
                    R2->until_token_old=R->until_token;
                      if(controlled){
                        if(Check_TCL(R2)){
                          guess_fail=false;
                          passDown_success=true;
                        }
                      }
                      else{
                        if(!Check_TCL(R2)){
                          guess_fail=true;
                          cout<<"guess_fail"<<endl;
                        }
                      }
                            }
                        }
                        if(controlled && !passDown_success){guess_fail=true;}
                    }
            if(!guess_fail){
                cout<<"guess success"<<endl;
                return true;
            }
          }
          else{
          cout<<"guess_fail"<<endl;
        }
      }
    }
    }
    */
}


int main(int argc,char** argv) {
    int i,j,k;
    //i,j,k XDrz
    
    sxiCount=0;
    nodeCount=0;
    strategyCount=0;
    closureCount=0;
    parseCount=0;
    type_map[TRUE_NODE]="TRUE";
    type_map[FALSE_NODE]="FALSE";
    type_map[PARSE_ROOT]="ROOT";
    type_map[ATOMIC]="ATOMIC";
    type_map[NOT]="NOT";
    type_map[OR]="OR";
    type_map[AND]="AND";
    type_map[PLUS]="PLUS";
    type_map[MINUS]="MINUS";
    type_map[UNTIL]="UNTIL";
    type_map[WNTIL]="WNTIL";
    type_map[NEXT]="NEXT";    
    //Initial global variables

    red_begin_session(RED_SYSTEM_TIMED, argv[1], -1);    
        //-1 == default(process number)
    red_input_model(argv[1], RED_REFINE_GLOBAL_INVARIANCE);
    red_set_sync_bulk_depth(10); 
        //number of transitions can be involve into a synchronize transition
    TCL_Game_Node* root;
    root=new TCL_Game_Node;
    root->index= nodeCount;
    root->red=red_query_diagram_initial();    
    //Initial root for the model
    
    Nodes.push_back(root);    nodeCount++;
    node_map[temp_string.assign(red_diagram_string(root->red))]=root->index;
    //Add root into list
        
    path=red_query_diagram_enhanced_global_invariance();
    sxiCount=red_query_sync_xtion_count(RED_USE_DECLARED_SYNC_XTION); 
    //number of synchronize transitions in the model
    
    cout<<endl;
    extractModelFromFile(root);
    labelPlayerSelectionOnEdge(root);
    printGameGraph();
    //Draw the graph

    if(argc>2){
        FILE *infile = fopen(argv[2], "r");
        yyin=infile;
        do {
            yyparse();
        } while (!feof(yyin));
        Setup_TCL_Formula(ROOT_ptr);
        print_strategy2owner();
        setupMatrix();
        fill_in_matrix(Parse_Tree[0]);
        print_matrix();
        print_parse_tree(Parse_Tree[0],0);
    }
    else{
        cout<<"No input TCL formula"<<endl;
        return 0;
    }
    //Read in the formula    

    Computation_Tree_Node* R =new Computation_Tree_Node;
    R->ins=NULL;
    R->state=root;
    R->until_token_old=0;
    for(i=0;i<closureCount;i++){
        R->obligation[i]=DONT_CARE;
    }
    R->obligation[0]=MUST_TRUE;
    //Initialize computation tree

    Check_TCL(R);
    //BJ4
    
    return 0;
}




