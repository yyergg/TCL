#include "PSIL.h"

/*extern function from parser*/
extern "C" int yylex();
extern "C" int yyparse();
extern "C" FILE *yyin;

//Global variables
int sxi_COUNT;
int node_COUNT;
int strategy_COUNT;
int closure_COUNT;
int parse_COUNT;

char input_owner[20]="turn";
redgram path;
PSIL_Game_Edge* temp_edge;
PSIL_Game_Node* temp_node;
string temp_string;
char temp_char[1000];
map<string,int> node_map;
map<int,string> type_map;
vector<PSIL_Game_Node*> Nodes;
vector<PSIL_Formula*> Parse_Tree;
vector<PSIL_Formula*> Closure;
vector<int> strategy2owner;

PSIL_Formula* ROOT_ptr;

int ** Matrix;
int* strategy_stack;
//Global End

int	cplugin_proc(int module_index,int	proc_index) {
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




Computation_Tree_Node::Computation_Tree_Node(){
	int i;
	G=new int[closure_COUNT];
	until_token=0;
	until_token_old=0;
	obligation=new int[closure_COUNT];
	pass_down=new int[closure_COUNT];
	passed=new bool[closure_COUNT];
	for(i=0;i<closure_COUNT;i++){
		G[i]=DONT_CARE;
		obligation[i]=0;
		pass_down[i]=0;
		passed[i]=false;
	}
}

Computation_Tree_Node::~Computation_Tree_Node(){
	delete G;
	delete obligation;
	delete pass_down;
	delete passed;
}





void print_parse_tree(PSIL_Formula* F, int depth){
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

void Setup_PSIL_Formula(PSIL_Formula* F){
	int i;
	Parse_Tree.push_back(F);
	if(F->type==PARSE_ROOT || F->type==PLUS){
		strategy2owner.push_back(F->owner);
		F->index=parse_COUNT;
		parse_COUNT++;
		F->strategy_index=strategy_COUNT;
		strategy_COUNT++;
		F->closure_index=0;
		for(i=0;i<F->outs.size();i++){
			Setup_PSIL_Formula(F->outs[i]);
		}
	}
	else if(F->type==MINUS){
		F->index=parse_COUNT;
		parse_COUNT++;
		F->strategy_index=-1;
		F->closure_index=0;
		for(i=0;i<F->outs.size();i++){
			Setup_PSIL_Formula(F->outs[i]);
		}
	}
	else if(F->type==ATOMIC){
		F->index=parse_COUNT;
		parse_COUNT++;
		F->closure_index=closure_COUNT;
		Closure.push_back(F);
		closure_COUNT++;
		F->strategy_index=0;
		for(i=0;i<F->outs.size();i++){
			Setup_PSIL_Formula(F->outs[i]);
		}
	}
	else{
		F->index=parse_COUNT;
		parse_COUNT++;
		F->closure_index=closure_COUNT;
		Closure.push_back(F);
		closure_COUNT++;
		F->strategy_index=0;
		for(i=0;i<F->outs.size();i++){
			Setup_PSIL_Formula(F->outs[i]);
		}
	}
}

void fill_in_matrix(PSIL_Formula* F){
	int i;
	switch(F->type){
		case TRUE_NODE:
			for(i=0;i<strategy_COUNT;i++){
				Matrix[i][F->closure_index]=strategy_stack[i];
			}
			break;
		case FALSE_NODE:
			for(i=0;i<strategy_COUNT;i++){
				Matrix[i][F->closure_index]=strategy_stack[i];
			}
			break;
		case PARSE_ROOT:
//			cout<<"PARSE_ROOT:"<<endl;
			for(i=0;i<strategy_COUNT;i++){
				if(strategy2owner[i]==F->owner){
					strategy_stack[i]=0;
				}
			} //clean the previous strategy of the same player
			strategy_stack[F->strategy_index]= 1;
			fill_in_matrix(F->outs[0]);
			strategy_stack[F->strategy_index]= 0;
//			cout<<"END"<<endl;
			break;
		case ATOMIC:
			for(i=0;i<strategy_COUNT;i++){
				Matrix[i][F->closure_index]=strategy_stack[i];
			}
			break;
    case NOT:
			for(i=0;i<strategy_COUNT;i++){
				Matrix[i][F->closure_index]=strategy_stack[i];
			}
    	fill_in_matrix(F->outs[0]);
    	break;
		case OR:
			for(i=0;i<strategy_COUNT;i++){
				Matrix[i][F->closure_index]=strategy_stack[i];
			}
			fill_in_matrix(F->outs[0]);
			fill_in_matrix(F->outs[1]);
			break;
		case AND:
			for(i=0;i<strategy_COUNT;i++){
				Matrix[i][F->closure_index]=strategy_stack[i];
			}
			fill_in_matrix(F->outs[0]);
			fill_in_matrix(F->outs[1]);
			break;
		case PLUS:
			for(i=0;i<strategy_COUNT;i++){
				if(strategy2owner[i]==F->owner){
					strategy_stack[i]=0;
				}
			} //clean the previous strategy of the same player
			strategy_stack[F->strategy_index]= 1;
			fill_in_matrix(F->outs[0]);
			strategy_stack[F->strategy_index]= 0;
			break;
		case MINUS:
			for(i=0;i<strategy_COUNT;i++){
				if(strategy2owner[i]==F->owner){
					strategy_stack[i]=0;
				}
			} //clean the previous strategy of the same player
			fill_in_matrix(F->outs[0]);
			break;
		case UNTIL:
//			cout<<"UNTIL"<<endl;
			for(i=0;i<strategy_COUNT;i++){
				Matrix[i][F->closure_index]=strategy_stack[i];
			}
			fill_in_matrix(F->outs[0]);
			fill_in_matrix(F->outs[1]);
			break;
		case WNTIL:
			for(i=0;i<strategy_COUNT;i++){
				Matrix[i][F->closure_index]=strategy_stack[i];
			}
			fill_in_matrix(F->outs[0]);
			fill_in_matrix(F->outs[1]);
			break;
		case NEXT:
//			cout<<"NEXT"<<endl;
			for(i=0;i<strategy_COUNT;i++){
				Matrix[i][F->closure_index]=strategy_stack[i];
			}
    	fill_in_matrix(F->outs[0]);
			break;
	}
}




int find_next_closure(int x){
	if(Parse_Tree[x]->type==PARSE_ROOT || Parse_Tree[x]->type==PLUS || Parse_Tree[x]->type==MINUS){
		return find_next_closure(Parse_Tree[x]->outs[0]->index);
	}
	else {return Parse_Tree[x]->closure_index;}
}


void setup_matrix(){
	int i,j;
	Matrix=new int*[strategy_COUNT];
	for(i=0;i<strategy_COUNT;i++){
		Matrix[i]=new int[closure_COUNT];
	}
	for(i=0;i<strategy_COUNT;i++){
		for(j=0;j<closure_COUNT;j++){
			Matrix[i][j]=0;
		}
	}
	//setup strategy_stack, conveniently
	strategy_stack=new int[strategy_COUNT];
	for(i=0;i<strategy_COUNT;i++){
		strategy_stack[i]=0;
	}
}

void print_matrix(){
	int i,j;
	cout<<setw(7)<<"*******";
	for(i=0;i<closure_COUNT;i++){
		cout<<setw(5)<<Closure[i]->index;
	}
	cout<<endl;
	for(i=0;i<strategy_COUNT;i++){
		string temp_string;
		stringstream ss(temp_string);
		ss<<i<<"("<<strategy2owner[i]<<")";
		cout<<setw(7)<<ss.str();
		for(j=0;j<closure_COUNT;j++){
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

void Draw(PSIL_Game_Node* root){
	cout<<root->index<<": "<<red_diagram_string(root->red)<<endl;
	int i;
	int lb,hb;
	for(i=1;i<sxi_COUNT;i++){
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
			  temp_edge=new PSIL_Game_Edge;
			  temp_node=new PSIL_Game_Node;
			  temp_edge->sxi=i;
			  temp_edge->src=root;
			  temp_edge->dst=temp_node;
			  root->outs.push_back(temp_edge);
			  temp_node->index=node_COUNT; node_COUNT++;
			  temp_node->red=temp_red;
			  red_get_cube_discrete_value(temp_node->red,input_owner,&lb,&hb);
			  temp_node->owner=lb;
			  temp_node->ins.push_back(temp_edge);
			  Nodes.push_back(temp_node);
			  node_map[temp_string.assign(red_diagram_string(temp_red))]=temp_node->index;
			  cout<<root->index<<"--"<<temp_edge->sxi<<"-->"<<temp_node->index<<endl;
			  Draw(temp_node);
			}
			else{
			  temp_node=Nodes[node_map[temp_string.assign(red_diagram_string(temp_red))]];
			  temp_edge=new PSIL_Game_Edge;
			  temp_edge->sxi=i;
			  temp_edge->src=root;
			  temp_edge->dst=temp_node;
			  root->outs.push_back(temp_edge);
			  temp_node->ins.push_back(temp_edge);
			  cout<<root->index<<"--"<<temp_edge->sxi<<"-->"<<temp_node->index<<endl;
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



int Check_Visited(Computation_Tree_Node* R){
	cout<<"check visited"<<endl;
	int i;
	if(R->until_token_old==0){
		R->until_token=(R->until_token_old+1)%(closure_COUNT+1);
	}
	else if(Closure[R->until_token_old-1]->type==UNTIL && R->G[R->until_token_old-1]==TRUE_GUESSED_PHASE_2){
		R->until_token=R->until_token_old;
	}
	else if(Closure[R->until_token_old-1]->type==WNTIL && R->G[R->until_token_old-1]==FALSE_GUESSED_PHASE_2){
		R->until_token=R->until_token_old;
	}	
	else{
		R->until_token=(R->until_token_old+1)%(closure_COUNT+1);
		while(R->until_token!=0 && (Closure[R->until_token-1]->type!=UNTIL && Closure[R->until_token-1]->type!=WNTIL)){
			R->until_token=(R->until_token+1)%(closure_COUNT+1);
		}
	}

	cout<<"Check_visited(state,token):("<<R->state->index<<","<<R->until_token<<")"<<endl;
	
	
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
				for(i=0;i<closure_COUNT;i++){
					if(R->G[i]!=ancestor->G[i]){diff_G=true;}
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

void First_Guess(Computation_Tree_Node* R){
	int i,j;
	for(i=0;i<closure_COUNT;i++){
		R->G[i]=R->obligation[i];
	}
	for(i=0;i<closure_COUNT;i++){
		if(R->G[i]==MUST_TRUE){
			switch(Closure[i]->type){
				int left,right;
				case OR:
					left=find_next_closure(Closure[i]->outs[0]->index);
					right=find_next_closure(Closure[i]->outs[1]->index);
					R->G[i]=TRUE_GUESSED_PHASE_1;
					R->G[left]=MUST_TRUE;
					R->G[right]=DONT_CARE;
					break;
				case AND:
					left=find_next_closure(Closure[i]->outs[0]->index);
					right=find_next_closure(Closure[i]->outs[1]->index);
					R->G[i]=MUST_TRUE;
					R->G[left]=MUST_TRUE;
					R->G[right]=MUST_TRUE;
					break;
				case UNTIL:
					//not sure
					left=find_next_closure(Closure[i]->outs[0]->index);
					right=find_next_closure(Closure[i]->outs[1]->index);
					R->G[i]=TRUE_GUESSED_PHASE_1;
					R->G[right]=MUST_TRUE;
					break;
				case WNTIL:
					//not sure
					left=find_next_closure(Closure[i]->outs[0]->index);
					right=find_next_closure(Closure[i]->outs[1]->index);
					R->G[i]=TRUE_GUESSED_PHASE_1;
					R->G[right]=MUST_TRUE;
					break;
				case NOT:
					R->G[i]=DONT_CARE;
					R->G[find_next_closure(Closure[i]->outs[0]->index)]=MUST_FALSE;
					break;
				default:
					R->G[i]=MUST_TRUE;
			}
		}
		else if(R->G[i]==MUST_FALSE){
			switch(Closure[i]->type){
				int left,right;
				case OR:
					left=find_next_closure(Closure[i]->outs[0]->index);
					right=find_next_closure(Closure[i]->outs[1]->index);
					R->G[i]=DONT_CARE;
					R->G[left]=MUST_FALSE;
					R->G[right]=MUST_FALSE;
					break;
				case AND:
					left=find_next_closure(Closure[i]->outs[0]->index);
					right=find_next_closure(Closure[i]->outs[1]->index);
					R->G[i]=FALSE_GUESSED_PHASE_1;
					R->G[left]=MUST_FALSE;
					break;
				case UNTIL:
					//Not sure
					left=find_next_closure(Closure[i]->outs[0]->index);
					right=find_next_closure(Closure[i]->outs[1]->index);
					R->G[i]=FALSE_GUESSED_PHASE_1;
					R->G[left]=MUST_FALSE;
					R->G[right]=MUST_FALSE;
					break;
				case WNTIL:
					//not sure
					left=find_next_closure(Closure[i]->outs[0]->index);
					right=find_next_closure(Closure[i]->outs[1]->index);
					R->G[i]=FALSE_GUESSED_PHASE_1;
					R->G[left]=MUST_FALSE;
					R->G[right]=MUST_FALSE;
					break;
				case NOT:
					R->G[i]=DONT_CARE;
					R->G[find_next_closure(Closure[i]->outs[0]->index)]=MUST_TRUE;
					break;
				default:
					R->G[i]=MUST_FALSE;
			}
		}
	}
	cout<<"After first guess:";
	for(i=0;i<closure_COUNT;i++){
		cout<<" "<<R->G[i];
	}
	cout<<endl;
}

bool Guess(Computation_Tree_Node* R){
	int i,j;
	cout<<"before guess:";
	for(i=0;i<closure_COUNT;i++){
		cout<<" "<<R->G[i];
	}
	cout<<endl;
	bool carry;
	carry=true;
	for(i=closure_COUNT-1;i>=0;i--){
		if(carry){
			if(R->G[i]==TRUE_GUESSED_PHASE_1){
				R->G[i]=TRUE_GUESSED_PHASE_2;
				carry=false;
			}
			else if(R->G[i]==TRUE_GUESSED_PHASE_2){
				R->G[i]=TRUE_GUESSED_PHASE_1;
				carry=true;
			}
			else if(R->G[i]==FALSE_GUESSED_PHASE_1){
				R->G[i]=FALSE_GUESSED_PHASE_2;
				carry=false;
			}
			else if(R->G[i]==FALSE_GUESSED_PHASE_2){
				R->G[i]=FALSE_GUESSED_PHASE_1;
				carry=true;
			}
		}
	}
	if(carry){
		cout<<"No other possibility"<<endl;
		return false;
	}
	cout<<" after carry: ";
	for(i=0;i<closure_COUNT;i++){
        cout<<R->G[i]<<" ";
	}
	cout<<endl;

	for(i=0;i<closure_COUNT;i++){
		cout<<" pass down "<<i<<": ";
		for(j=0;j<closure_COUNT;j++){
      cout<<R->G[j]<<" ";
		}
		cout<<endl;
		if(R->G[i]==MUST_TRUE){
			switch(Closure[i]->type){
				int left,right;
				case OR:
					left=find_next_closure(Closure[i]->outs[0]->index);
					right=find_next_closure(Closure[i]->outs[1]->index);
					R->G[i]=TRUE_GUESSED_PHASE_1;
					if(R->obligation[left]==DONT_CARE){
						R->G[left]=MUST_TRUE;
					}
					if(R->obligation[right]==DONT_CARE){
						R->G[right]=DONT_CARE;
					}
					break;
				case AND:
					left=find_next_closure(Closure[i]->outs[0]->index);
					right=find_next_closure(Closure[i]->outs[1]->index);
					R->G[i]=MUST_TRUE;
					if(R->G[left]==DONT_CARE){
						R->G[left]=MUST_TRUE;
					}
					if(R->G[right]==DONT_CARE){
						R->G[right]=MUST_TRUE;
					}
					break;
				case UNTIL:
					//not sure
					left=find_next_closure(Closure[i]->outs[0]->index);
					right=find_next_closure(Closure[i]->outs[1]->index);
					R->G[i]=TRUE_GUESSED_PHASE_1;
					if(R->obligation[left]==DONT_CARE){
						R->G[left]=DONT_CARE;
					}
					if(R->obligation[right]==DONT_CARE){
						R->G[right]=MUST_TRUE;
					}
					break;
				case WNTIL:
					left=find_next_closure(Closure[i]->outs[0]->index);
					right=find_next_closure(Closure[i]->outs[1]->index);
					R->G[i]=TRUE_GUESSED_PHASE_1;
					if(R->obligation[left]==DONT_CARE){
						R->G[left]=DONT_CARE;
					}
					if(R->obligation[right]==DONT_CARE){
						R->G[right]=MUST_TRUE;
					}
					break;
				case NEXT:
					if(R->obligation[find_next_closure(Closure[i]->outs[0]->index)]==DONT_CARE){
						R->G[find_next_closure(Closure[i]->outs[0]->index)]=DONT_CARE;
					}
					break;
				case NOT:
					R->G[i]=DONT_CARE;
					if(R->obligation[find_next_closure(Closure[i]->outs[0]->index)]==DONT_CARE){
						R->G[find_next_closure(Closure[i]->outs[0]->index)]=MUST_FALSE;
					}
					break;
				default:
					R->G[i]=MUST_TRUE;
			}
		}
		else if(R->G[i]==MUST_FALSE){
			switch(Closure[i]->type){
				int left,right;
				case OR:
					left=find_next_closure(Closure[i]->outs[0]->index);
					right=find_next_closure(Closure[i]->outs[1]->index);
					R->G[i]=MUST_FALSE;
					if(R->G[left]==DONT_CARE){
						R->G[left]=MUST_FALSE;
					}
					if(R->G[right]==DONT_CARE){
						R->G[right]=MUST_FALSE;
					}
					break;
				case AND:
					left=find_next_closure(Closure[i]->outs[0]->index);
					right=find_next_closure(Closure[i]->outs[1]->index);
					R->G[i]=FALSE_GUESSED_PHASE_1;
					if(R->obligation[left]==DONT_CARE){
						R->G[left]=MUST_FALSE;
					}
					if(R->obligation[right]==DONT_CARE){
						R->G[right]=DONT_CARE;
					}
					break;
				case UNTIL:
					left=find_next_closure(Closure[i]->outs[0]->index);
					right=find_next_closure(Closure[i]->outs[1]->index);
					R->G[i]=FALSE_GUESSED_PHASE_1;
					if(R->obligation[left]==DONT_CARE){
						R->G[left]=MUST_FALSE;
					}
					if(R->obligation[right]==DONT_CARE){
						R->G[right]=MUST_FALSE;
					}
					break;
				case WNTIL:
					left=find_next_closure(Closure[i]->outs[0]->index);
					right=find_next_closure(Closure[i]->outs[1]->index);
					R->G[i]=FALSE_GUESSED_PHASE_1;
					if(R->obligation[left]==DONT_CARE){
						R->G[left]=MUST_FALSE;
					}
					if(R->obligation[right]==DONT_CARE){
						R->G[right]=MUST_FALSE;
					}
					break;
				case NEXT:
					if(R->obligation[find_next_closure(Closure[i]->outs[0]->index)]==DONT_CARE){
						R->G[find_next_closure(Closure[i]->outs[0]->index)]=DONT_CARE;
					}
					break;
				case NOT:
					R->G[i]=DONT_CARE;
					R->G[find_next_closure(Closure[i]->outs[0]->index)]=MUST_TRUE;
					break;
				default:
					R->G[i]=MUST_FALSE;
			}
		}
		else if(R->G[i]==TRUE_GUESSED_PHASE_1){
			switch(Closure[i]->type){
				int left,right;
				case OR:
					left=find_next_closure(Closure[i]->outs[0]->index);
					right=find_next_closure(Closure[i]->outs[1]->index);
					R->G[i]=TRUE_GUESSED_PHASE_1;
					if(R->G[left]==TRUE_GUESSED_PHASE_1 || R->G[left]==TRUE_GUESSED_PHASE_2
					||R->G[left]==FALSE_GUESSED_PHASE_1 || R->G[left]==FALSE_GUESSED_PHASE_2){
						R->G[left]=R->G[left];
					}
					else{R->G[left]=MUST_TRUE;}
					if(R->obligation[right]==DONT_CARE){
						R->G[right]=DONT_CARE;
					}
					break;
				case UNTIL:
					left=find_next_closure(Closure[i]->outs[0]->index);
					right=find_next_closure(Closure[i]->outs[1]->index);
					R->G[i]=TRUE_GUESSED_PHASE_1;
					if(R->obligation[left]==DONT_CARE){
						R->G[left]=DONT_CARE;
					}
					if(R->G[right]==TRUE_GUESSED_PHASE_1 || R->G[right]==TRUE_GUESSED_PHASE_2
					||R->G[right]==FALSE_GUESSED_PHASE_1 || R->G[right]==FALSE_GUESSED_PHASE_2){
						R->G[right]=R->G[right];
					}
					else{R->G[right]=MUST_TRUE;}
					break;
				case WNTIL:
					left=find_next_closure(Closure[i]->outs[0]->index);
					right=find_next_closure(Closure[i]->outs[1]->index);
					R->G[i]=TRUE_GUESSED_PHASE_1;
					if(R->obligation[left]==DONT_CARE){
						R->G[left]=DONT_CARE;
					}
					if(R->G[right]==TRUE_GUESSED_PHASE_1 || R->G[right]==TRUE_GUESSED_PHASE_2
					||R->G[right]==FALSE_GUESSED_PHASE_1 || R->G[right]==FALSE_GUESSED_PHASE_2){
						R->G[right]=R->G[right];
					}
					else{R->G[right]=MUST_TRUE;}
					break;
			}
		}
		else if(R->G[i]==TRUE_GUESSED_PHASE_2){
			switch(Closure[i]->type){
				int left,right;
				case OR:
					left=find_next_closure(Closure[i]->outs[0]->index);
					right=find_next_closure(Closure[i]->outs[1]->index);
					R->G[i]=TRUE_GUESSED_PHASE_2;
					if(R->obligation[left]==DONT_CARE){
						R->G[left]=DONT_CARE;
					}
					if(R->G[right]==TRUE_GUESSED_PHASE_1 || R->G[right]==TRUE_GUESSED_PHASE_2
					||R->G[right]==FALSE_GUESSED_PHASE_1 || R->G[right]==FALSE_GUESSED_PHASE_2){
						R->G[right]=R->G[right];
					}
					else{R->G[right]=MUST_TRUE;}
					break;
				case UNTIL:
					left=find_next_closure(Closure[i]->outs[0]->index);
					right=find_next_closure(Closure[i]->outs[1]->index);
					R->G[i]=TRUE_GUESSED_PHASE_2;
					if(R->G[left]==TRUE_GUESSED_PHASE_1 || R->G[left]==TRUE_GUESSED_PHASE_2
					||R->G[left]==FALSE_GUESSED_PHASE_1 || R->G[left]==FALSE_GUESSED_PHASE_2){
						R->G[left]=R->G[left];
					}
					else{R->G[left]=MUST_TRUE;}
					if(R->obligation[right]==DONT_CARE){
						R->G[right]=DONT_CARE;
					}
					break;
				case WNTIL:
					left=find_next_closure(Closure[i]->outs[0]->index);
					right=find_next_closure(Closure[i]->outs[1]->index);
					R->G[i]=TRUE_GUESSED_PHASE_2;
					if(R->G[left]==TRUE_GUESSED_PHASE_1 || R->G[left]==TRUE_GUESSED_PHASE_2
					||R->G[left]==FALSE_GUESSED_PHASE_1 || R->G[left]==FALSE_GUESSED_PHASE_2){
						R->G[left]=R->G[left];
					}
					else{R->G[left]=MUST_TRUE;}
					if(R->obligation[right]==DONT_CARE){
						R->G[right]=DONT_CARE;
					}
					break;
			}
		}
		else if(R->G[i]==FALSE_GUESSED_PHASE_1){
			switch(Closure[i]->type){
				int left,right;
				case AND:
					left=find_next_closure(Closure[i]->outs[0]->index);
					right=find_next_closure(Closure[i]->outs[1]->index);
					R->G[i]=FALSE_GUESSED_PHASE_1;
					if(R->G[left]==TRUE_GUESSED_PHASE_1 || R->G[left]==TRUE_GUESSED_PHASE_2
					||R->G[left]==FALSE_GUESSED_PHASE_1 || R->G[left]==FALSE_GUESSED_PHASE_2){
						R->G[left]=R->G[left];
					}
					else{R->G[left]=MUST_FALSE;}
					if(R->obligation[right]==DONT_CARE){
						R->G[right]=DONT_CARE;
					}
					break;
				case UNTIL:
					left=find_next_closure(Closure[i]->outs[0]->index);
					right=find_next_closure(Closure[i]->outs[1]->index);
					R->G[i]=FALSE_GUESSED_PHASE_1;
					if(R->G[left]==TRUE_GUESSED_PHASE_1 || R->G[left]==TRUE_GUESSED_PHASE_2
					||R->G[left]==FALSE_GUESSED_PHASE_1 || R->G[left]==FALSE_GUESSED_PHASE_2){
						R->G[left]=R->G[left];
					}
					else{R->G[left]=MUST_FALSE;}
					if(R->G[right]==TRUE_GUESSED_PHASE_1 || R->G[right]==TRUE_GUESSED_PHASE_2
					||R->G[right]==FALSE_GUESSED_PHASE_1 || R->G[right]==FALSE_GUESSED_PHASE_2){
						R->G[right]=R->G[right];
					}
					else{R->G[right]=MUST_FALSE;}
					break;
				case WNTIL:
					left=find_next_closure(Closure[i]->outs[0]->index);
					right=find_next_closure(Closure[i]->outs[1]->index);
					R->G[i]=FALSE_GUESSED_PHASE_1;
					if(R->G[left]==TRUE_GUESSED_PHASE_1 || R->G[left]==TRUE_GUESSED_PHASE_2
					||R->G[left]==FALSE_GUESSED_PHASE_1 || R->G[left]==FALSE_GUESSED_PHASE_2){
						R->G[left]=R->G[left];
					}
					else{R->G[left]=MUST_FALSE;}
					if(R->G[right]==TRUE_GUESSED_PHASE_1 || R->G[right]==TRUE_GUESSED_PHASE_2
					||R->G[right]==FALSE_GUESSED_PHASE_1 || R->G[right]==FALSE_GUESSED_PHASE_2){
						R->G[right]=R->G[right];
					}
					else{R->G[right]=MUST_FALSE;}										
					break;
			}
		}
		else if(R->G[i]==FALSE_GUESSED_PHASE_2){
			switch(Closure[i]->type){
				int left,right;
				case AND:
					left=find_next_closure(Closure[i]->outs[0]->index);
					right=find_next_closure(Closure[i]->outs[1]->index);
					R->G[i]=FALSE_GUESSED_PHASE_2;
					if(R->obligation[left]==DONT_CARE){
						R->G[left]=DONT_CARE;
					}
					if(R->G[right]==TRUE_GUESSED_PHASE_1 || R->G[right]==TRUE_GUESSED_PHASE_2
					||R->G[right]==FALSE_GUESSED_PHASE_1 || R->G[right]==FALSE_GUESSED_PHASE_2){
						R->G[right]=R->G[right];
					}
					else{R->G[right]=MUST_FALSE;}
					break;
				case UNTIL:
					left=find_next_closure(Closure[i]->outs[0]->index);
					right=find_next_closure(Closure[i]->outs[1]->index);
					R->G[i]=FALSE_GUESSED_PHASE_2;
					if(R->G[left]==TRUE_GUESSED_PHASE_1 || R->G[left]==TRUE_GUESSED_PHASE_2
					||R->G[left]==FALSE_GUESSED_PHASE_1 || R->G[left]==FALSE_GUESSED_PHASE_2){
						R->G[left]=R->G[left];
					}
					else{R->G[left]=MUST_TRUE;}
					if(R->G[right]==TRUE_GUESSED_PHASE_1 || R->G[right]==TRUE_GUESSED_PHASE_2
					||R->G[right]==FALSE_GUESSED_PHASE_1 || R->G[right]==FALSE_GUESSED_PHASE_2){
						R->G[right]=R->G[right];
					}
					else{R->G[right]=MUST_FALSE;}
					break;
				case WNTIL:
					left=find_next_closure(Closure[i]->outs[0]->index);
					right=find_next_closure(Closure[i]->outs[1]->index);
					R->G[i]=FALSE_GUESSED_PHASE_2;
					if(R->G[left]==TRUE_GUESSED_PHASE_1 || R->G[left]==TRUE_GUESSED_PHASE_2
					||R->G[left]==FALSE_GUESSED_PHASE_1 || R->G[left]==FALSE_GUESSED_PHASE_2){
						R->G[left]=R->G[left];
					}
					else{R->G[left]=MUST_TRUE;}
					if(R->G[right]==TRUE_GUESSED_PHASE_1 || R->G[right]==TRUE_GUESSED_PHASE_2
					||R->G[right]==FALSE_GUESSED_PHASE_1 || R->G[right]==FALSE_GUESSED_PHASE_2){
						R->G[right]=R->G[right];
					}
					else{R->G[right]=MUST_FALSE;}
					break;
			}
		}
		else if(R->G[i]==DONT_CARE){
			switch(Closure[i]->type){
				int left,right;
				case OR:
					left=find_next_closure(Closure[i]->outs[0]->index);
					right=find_next_closure(Closure[i]->outs[1]->index);
					if(R->obligation[left]==DONT_CARE){
						R->G[left]=DONT_CARE;
					}
					if(R->obligation[right]==DONT_CARE){
						R->G[right]=DONT_CARE;
					}
					break;
				case AND:
					left=find_next_closure(Closure[i]->outs[0]->index);
					right=find_next_closure(Closure[i]->outs[1]->index);
					if(R->obligation[left]==DONT_CARE){
						R->G[left]=DONT_CARE;
					}
					if(R->obligation[right]==DONT_CARE){
						R->G[right]=DONT_CARE;
					}
					break;
				case UNTIL:
					left=find_next_closure(Closure[i]->outs[0]->index);
					right=find_next_closure(Closure[i]->outs[1]->index);
					if(R->obligation[left]==DONT_CARE){
						R->G[left]=DONT_CARE;
					}
					if(R->obligation[right]==DONT_CARE){
						R->G[right]=DONT_CARE;
					}
					break;
				case WNTIL:
					left=find_next_closure(Closure[i]->outs[0]->index);
					right=find_next_closure(Closure[i]->outs[1]->index);
					if(R->obligation[left]==DONT_CARE){
						R->G[left]=DONT_CARE;
					}
					if(R->obligation[right]==DONT_CARE){
						R->G[right]=DONT_CARE;
					}
					break;
				case NEXT:
					R->G[i]=DONT_CARE;
					if(R->obligation[find_next_closure(Closure[i]->outs[0]->index)]==DONT_CARE){
						R->G[find_next_closure(Closure[i]->outs[0]->index)]=DONT_CARE;
					}
					break;
				case NOT:
					R->G[i]=DONT_CARE;
					if(R->obligation[find_next_closure(Closure[i]->outs[0]->index)]==DONT_CARE){
						R->G[find_next_closure(Closure[i]->outs[0]->index)]=DONT_CARE;
					}
					break;
				default:
					R->G[i]=DONT_CARE;
			}
		}
	}
	for(i=0;i<closure_COUNT;i++){
		cout<<" "<<R->G[i];
	}
	cout<<endl;
	return true;
}


int Check_Local(Computation_Tree_Node* R){
	cout<<"check local"<<endl;
	int i;
	int check_visited_result;
	check_visited_result=Check_Visited(R);

	if(check_visited_result==PASS){
		cout<<"check local PASS"<<endl;	
		return PASS;
	}
	else if(check_visited_result==FAIL){
		cout<<"check local fail"<<endl;			
		return FAIL;
	}
	else{
		for(i=closure_COUNT-1;i>=0;i--){
			if(R->G[i]!=DONT_CARE){
				switch(Closure[i]->type){
					int left,right;
					case TRUE_NODE:
						if(R->G[i]==MUST_FALSE){
							cout<<"check local fail "<<Closure[i]->index<<endl;						
							return FAIL;
						}
					break;
					case FALSE_NODE:
						if(R->G[i]==MUST_TRUE){
							cout<<"check local fail "<<Closure[i]->index<<endl;
							return FAIL;
						}
					break;
					case ATOMIC:
						char aaa[100];
						strcpy(aaa,Closure[i]->str.c_str());
						if(red_and(red_diagram(aaa),R->state->red)!=red_false()){
							if(R->G[i]==MUST_FALSE){
								cout<<"check local fail "<<Closure[i]->index<<endl;	
								return FAIL;
							}
						}
						else{
							if(R->G[i]==MUST_TRUE){
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

bool Create_Pass_Down(Computation_Tree_Node* R,bool &controlled){
	cout<<"create pass down:"<<endl;
	bool flag;
	flag=false;
	int i,j,x,y;
	for(i=0;i<closure_COUNT;i++){
		cout<<R->G[i]<<" ";
	}
	cout<<endl;
	for(i=0;i<closure_COUNT;i++){
		cout<<R->passed[i]<<" ";
	}
	cout<<endl;	
	for(i=0;i<closure_COUNT;i++){
		if(R->passed[i]==false && ((R->G[i]==MUST_TRUE && Closure[i]->type==NEXT)
			||(R->G[i]==MUST_FALSE && Closure[i]->type==NEXT)
			||(R->G[i]==TRUE_GUESSED_PHASE_2 &&(Closure[i]->type==UNTIL || Closure[i]->type==WNTIL))
			||(R->G[i]==FALSE_GUESSED_PHASE_2 && (Closure[i]->type==UNTIL || Closure[i]->type==WNTIL)))){
			flag=true;
			x=i;
			cout<<"closure "<<i;
			for(j=0;j<strategy_COUNT;j++){
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
	for(i=0;i<closure_COUNT;i++){
		R->pass_down[i]=DONT_CARE;
	}
	if(controlled==true){
		for(i=0;i<closure_COUNT;i++){
			if(Matrix[y][i]==1 && R->G[i]==MUST_TRUE && Closure[i]->type==NEXT && R->passed[i]==false){
        R->pass_down[i]=DONT_CARE;
        R->pass_down[find_next_closure(Closure[i]->outs[0]->index)]=MUST_TRUE;
        R->passed[i]=true;
			}
      else if(Matrix[y][i]==1 && R->G[i]==TRUE_GUESSED_PHASE_2 && (Closure[i]->type==UNTIL || Closure[i]->type==WNTIL)&& R->passed[i]==false){
				R->pass_down[i]=MUST_TRUE;
        R->passed[i]=true;
			}
			else if(Matrix[y][i]==1 && R->G[i]==MUST_FALSE && Closure[i]->type==NEXT&& R->passed[i]==false){
        R->pass_down[i]=DONT_CARE;
        R->pass_down[find_next_closure(Closure[i]->outs[0]->index)]=MUST_FALSE;
        R->passed[i]=true;
	    }
      else if(Matrix[y][i]==1 && R->G[i]==FALSE_GUESSED_PHASE_2 && (Closure[i]->type==UNTIL || Closure[i]->type==WNTIL) && R->passed[i]==false){
		    R->pass_down[i]=MUST_FALSE;
        R->passed[i]=true;
			}
		}
	}
	else{
		for(i=0;i<closure_COUNT;i++){
			R->pass_down[i]=DONT_CARE;
		}
    if(R->G[x]==MUST_TRUE && Closure[x]->type==NEXT && R->passed[i]==false){
      R->pass_down[x]=DONT_CARE;
      R->pass_down[find_next_closure(Closure[x]->outs[0]->index)]=MUST_TRUE;
			R->passed[x]=true;
    }
    else if(R->G[x]==TRUE_GUESSED_PHASE_2 && (Closure[x]->type==UNTIL || Closure[x]->type==WNTIL) && R->passed[i]==false){
      R->pass_down[x]=MUST_TRUE;
			R->passed[x]=true;
    }
    else if(R->G[x]==MUST_FALSE && Closure[x]->type==NEXT && R->passed[i]==false){
      R->pass_down[x]=DONT_CARE;
      R->pass_down[find_next_closure(Closure[x]->outs[0]->index)]=MUST_FALSE;
			R->passed[x]=true;
    }
    else if(R->G[x]==FALSE_GUESSED_PHASE_2 && (Closure[i]->type==UNTIL || Closure[i]->type==WNTIL) && R->passed[i]==false){
      R->pass_down[x]=MUST_FALSE;
			R->passed[x]=true;
    }
	}
	cout<<"pass down created:";	
	for(i=0;i<closure_COUNT;i++){
		cout<<R->pass_down[i]<<" ";
	}
	cout<<endl;
	for(i=0;i<closure_COUNT;i++){
		cout<<R->passed[i]<<" ";
	}
	cout<<endl;
	return true;
}


bool Check_PSIL(Computation_Tree_Node* R){
	int i,j;
	int local_result;	
	cout<<"check PSIL on:"<<red_diagram_string(R->state->red)<<endl;
	cout<<"obligation:";
	for(i=0;i<closure_COUNT;i++){
  	cout<<R->obligation[i]<<" ";
	}
	cout<<endl;
	First_Guess(R);

	local_result=Check_Local(R);
	if(local_result==PASS){
		cout<<"Check_PSIL true"<<endl;
		return true;
	}
	else if(local_result==FAIL){
		while(Guess(R)){
			local_result=Check_Local(R);
			if(local_result==PASS){
				cout<<"Check_PSIL true"<<endl;
				return true;
			}			
			else if(local_result==CONTINUE){
				for(i=0;i<closure_COUNT;i++){
					R->passed[i]=false;
				}
				bool controlled=false;
		    bool guess_fail;
		    guess_fail=false;
				while(Create_Pass_Down(R,controlled) && !guess_fail){
					bool pass_down_success;
					pass_down_success=false;
					for(i=0;i<R->state->outs.size();i++){
	          if(!pass_down_success){
				    	cout<<"check child "<<i+1<<"/"<<R->state->outs.size()<<endl;
	            Computation_Tree_Node* R2=new Computation_Tree_Node;
	            R2->state=R->state->outs[i]->dst;
	            R2->ins=R;
	            R->outs.push_back(R2);
	            for(j=0;j<closure_COUNT;j++){
		            R2->obligation[j]=R->pass_down[j];
	            }
	            R2->until_token_old=R->until_token;
	            if(controlled){
		            if(Check_PSIL(R2)){
	                guess_fail=false;
	                pass_down_success=true;
		            }
	            } 
	            else{
	              if(!Check_PSIL(R2)){
	                guess_fail=true;
	                cout<<"guess_fail"<<endl;
	              }
	            }
						}
					}
					if(controlled && !pass_down_success){guess_fail=true;}
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
	else if(local_result==CONTINUE){
		for(i=0;i<closure_COUNT;i++){
			R->passed[i]=false;
		}
    bool controlled=false;
    bool guess_fail;
    guess_fail=false;		
		while(Create_Pass_Down(R,controlled) && !guess_fail){
			bool pass_down_success;
			pass_down_success=false;
			for(i=0;i<R->state->outs.size();i++){
        if(!pass_down_success){
		    	cout<<"check child "<<i+1<<"/"<<R->state->outs.size()<<endl;
          Computation_Tree_Node* R2=new Computation_Tree_Node;
          R2->state=R->state->outs[i]->dst;
          R2->ins=R;
          R->outs.push_back(R2);
          for(j=0;j<closure_COUNT;j++){
            R2->obligation[j]=R->pass_down[j];
          }
         	R2->until_token_old=R->until_token;
          if(controlled){
            if(Check_PSIL(R2)){
              guess_fail=false;
              pass_down_success=true;
            }
          }
          else{
            if(!Check_PSIL(R2)){
              guess_fail=true;
              cout<<"guess_fail"<<endl;
            }
          }
				}
			}
			if(controlled && !pass_down_success){guess_fail=true;}
		}
    if(!guess_fail){
      cout<<"guess success"<<endl;
      return true;
    }
    else{
	    guess_fail=false;
      while(Guess(R)){
				local_result=Check_Local(R);
				if(local_result==PASS){
					cout<<"Check_PSIL true"<<endl;
					return true;
				}			
				else if(local_result==CONTINUE){
					for(i=0;i<closure_COUNT;i++){
						R->passed[i]=false;
					}
	        bool controlled=false;
					while(Create_Pass_Down(R,controlled) && !guess_fail){
						bool pass_down_success;
						pass_down_success=false;
						for(i=0;i<R->state->outs.size();i++){
			        if(!pass_down_success){
					    	cout<<"check child "<<i+1<<"/"<<R->state->outs.size()<<endl;
			          Computation_Tree_Node* R2=new Computation_Tree_Node;
			          R2->state=R->state->outs[i]->dst;
			          R2->ins=R;
			          R->outs.push_back(R2);
			          for(j=0;j<closure_COUNT;j++){
			            R2->obligation[j]=R->pass_down[j];
			          }
	            	R2->until_token_old=R->until_token;
			          if(controlled){
			            if(Check_PSIL(R2)){
			              guess_fail=false;
			              pass_down_success=true;
			            }
			          }
			          else{
			            if(!Check_PSIL(R2)){
			              guess_fail=true;
			              cout<<"guess_fail"<<endl;
			            }
			          }
							}
						}
						if(controlled && !pass_down_success){guess_fail=true;}
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
}



int main(int argc,char** argv) {
	int i,j,k;
	int lb,hb;

	sxi_COUNT=0;
	node_COUNT=1;
	strategy_COUNT=0;
	closure_COUNT=0;
	parse_COUNT=0;
	//initial golbal variables

	red_begin_session(RED_SYSTEM_TIMED, argv[1], -1);	// -P n; n = process number, -1 == default
	red_input_model(argv[1], RED_REFINE_GLOBAL_INVARIANCE);
	red_set_sync_bulk_depth(3);
	//read in the model


	PSIL_Game_Node* root;
	root=new PSIL_Game_Node;
	Nodes.push_back(root);  //dummy one
	Nodes.push_back(root);
	root->red=red_query_diagram_initial();
	red_get_cube_discrete_value(root->red,input_owner,&lb,&hb);
	root->owner=lb;
	root->index= node_COUNT; node_COUNT++;
	node_map[temp_string.assign(red_diagram_string(root->red))]=root->index;
	path=red_query_diagram_enhanced_global_invariance();
	sxi_COUNT=red_query_sync_xtion_count(RED_USE_DECLARED_SYNC_XTION);
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
	
	
	
	//initial variables

	cout<<endl;
	Draw(root);
	//Draw the graph

	FILE *infile = fopen(argv[2], "r");
	yyin=infile;
	do {
		yyparse();
	} while (!feof(yyin));
	Setup_PSIL_Formula(ROOT_ptr);
	print_strategy2owner();
	setup_matrix();
	fill_in_matrix(Parse_Tree[0]);
	print_matrix();
	print_parse_tree(Parse_Tree[0],0);
	//read in formula	

	Computation_Tree_Node* R =new Computation_Tree_Node;
	R->ins=NULL;
	R->state=root;
	R->until_token_old=0;
	for(i=0;i<closure_COUNT;i++){
		R->obligation[i]=DONT_CARE;
	}
	R->obligation[0]=MUST_TRUE;

	//Init computation tree

	Check_PSIL(R);

	return 0;
}




