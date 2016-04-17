#include "TCL.h"

/*extern function from parser*/
extern "C" int yylex();
extern "C" int yyparse();
extern "C" FILE *yyin;

//Global variables
int g_sxi_count;
int g_node_count;
int g_strategy_count;
int g_parse_count;

char input_owner[20] = "turn";
redgram path;



map<string, int> node_map;
map<int, string> type_map;
vector<TCLGameNode*> g_nodes;
vector<TCLFormula*> g_parse_tree;
vector<int> g_strategy_to_owner;

TCLFormula* g_parse_root;

int** g_strategy_formula_matrix;
//Global End

int cplugin_proc(int module_index, int proc_index) {
    switch (module_index) {
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

ComputationTreeNode::ComputationTreeNode() {
    this->obligation = vector<int>(g_parse_count, DONT_CARE);
    this->pass_down = vector<int>(g_parse_count, DONT_CARE);
}

void ComputationTreeNode::ListAllPossibleGuess() {
    int i, j;
    vector<int> new_guess(g_parse_count, DONT_CARE);
    new_guess = this->obligation;
    this->all_possible_guess.push_back(new_guess);
    i = 0;
    while (i < this->all_possible_guess.size()) {
        // deducting until first unguessed g_parse_tree found
        for (j = 0; j < g_parse_count; j++) {
            if (this->all_possible_guess[i][j] == MUST_TRUE) {
                if (g_parse_tree[j]->type == NOT) {
                    this->all_possible_guess[i][j] = DONT_CARE;
                    this->all_possible_guess[i][g_parse_tree[j]->outs[0]->index] = MUST_FALSE;
                } else if (g_parse_tree[j]->type == PLUS
                           || g_parse_tree[j]->type == MINUS
                           || g_parse_tree[j]->type == PARSE_ROOT) {
                    this->all_possible_guess[i][j] = DONT_CARE;
                    this->all_possible_guess[i][g_parse_tree[j]->outs[0]->index] = MUST_TRUE;
                } else if (g_parse_tree[j]->type == NEXT) {
                    this->all_possible_guess[i][j] = DONT_CARE;
                    if (this->all_possible_guess[i][g_parse_tree[j]->outs[0]->index] == DONT_CARE) {
                        this->all_possible_guess[i][g_parse_tree[j]->outs[0]->index] = PASS_DOWN_MUST_TRUE;
                    } else {
                        this->all_possible_guess[i][g_parse_tree[j]->outs[0]->index] = PASS_DOWN_AND_MUST_TRUE;
                    }
                } else if (g_parse_tree[j]->type == TRUE_NODE
                           || g_parse_tree[j]->type == FALSE_NODE
                           || g_parse_tree[j]->type == ATOMIC) {
                    continue;
                } else { //OR AND UNTIL WNTIL
                    // Already satisfied or 1 way
                    int left = g_parse_tree[j]->outs[0]->index;
                    int right = g_parse_tree[j]->outs[1]->index;
                    if (g_parse_tree[j]->type == OR) {
                        if (this->all_possible_guess[i][left] == MUST_TRUE
                                || this->all_possible_guess[i][right] == MUST_TRUE) {
                            this->all_possible_guess[i][j] = DONT_CARE;
                            continue;
                        }
                    } else if (g_parse_tree[j]->type == AND) {
                        this->all_possible_guess[i][j] = DONT_CARE;
                        this->all_possible_guess[i][left] = MUST_TRUE;
                        this->all_possible_guess[i][right] = MUST_TRUE;
                        continue;
                    } else {
                        if (this->all_possible_guess[i][right] == MUST_TRUE) {
                            this->all_possible_guess[i][j] = DONT_CARE;
                            continue;
                        }
                    }
                    // all the cases need guessing will come to this break
                    break;
                }
            } else if (this->all_possible_guess[i][j] == MUST_FALSE) {
                if (g_parse_tree[j]->type == NOT) {
                    this->all_possible_guess[i][j] = DONT_CARE;
                    this->all_possible_guess[i][g_parse_tree[j]->outs[0]->index] = MUST_TRUE;
                } else if (g_parse_tree[j]->type == PLUS
                           || g_parse_tree[j]->type == MINUS
                           || g_parse_tree[j]->type == PARSE_ROOT) {
                    this->all_possible_guess[i][j] = DONT_CARE;
                    this->all_possible_guess[i][g_parse_tree[j]->outs[0]->index] = MUST_FALSE;
                } else if (g_parse_tree[j]->type == NEXT) {
                    this->all_possible_guess[i][j] = DONT_CARE;
                    if (this->all_possible_guess[i][g_parse_tree[j]->outs[0]->index] == DONT_CARE) {
                        this->all_possible_guess[i][g_parse_tree[j]->outs[0]->index] = PASS_DOWN_MUST_FALSE;
                    } else {
                        this->all_possible_guess[i][g_parse_tree[j]->outs[0]->index] = PASS_DOWN_AND_MUST_FALSE;
                    }
                } else if (g_parse_tree[j]->type == TRUE_NODE
                           || g_parse_tree[j]->type == FALSE_NODE
                           || g_parse_tree[j]->type == ATOMIC) {
                    continue;
                } else { //OR AND UNTIL WNTIL
                    // Already satisfied or 1 way
                    int left = g_parse_tree[j]->outs[0]->index;
                    int right = g_parse_tree[j]->outs[1]->index;
                    if (g_parse_tree[j]->type == OR) {
                        this->all_possible_guess[i][j] = DONT_CARE;
                        this->all_possible_guess[i][left] = MUST_FALSE;
                        this->all_possible_guess[i][right] = MUST_FALSE;
                    } else if (g_parse_tree[j]->type == AND) {
                        if (this->all_possible_guess[i][left] == MUST_FALSE
                                || this->all_possible_guess[i][right] == MUST_FALSE) {
                            this->all_possible_guess[i][j] = DONT_CARE;
                            continue;
                        }
                    } else {
                        if (this->all_possible_guess[i][left] == MUST_FALSE
                                && this->all_possible_guess[i][right] == MUST_FALSE) {
                            this->all_possible_guess[i][j] = DONT_CARE;
                            continue;
                        }
                    }
                    // all the cases need guessing will come to this break
                    break;
                }
            } else { //DONT_CARE or PASS_DOWN_* or GUESSED_*
                continue;
            }
        }

        // if this guess array is completed
        if (j == g_parse_count) {
            i++;
            continue;
        }

        // [todo] guess deeper and delete current unfinished guess
        int left = g_parse_tree[j]->outs[0]->index;
        int right = g_parse_tree[j]->outs[1]->index;

        // All the cases require guessing
        if (g_parse_tree[j]->type == OR) {
            if (this->all_possible_guess[i][j] == MUST_TRUE || this->all_possible_guess[i][j] == PASS_DOWN_AND_MUST_TRUE) {
                vector<int> deductionChild_1(g_parse_count, DONT_CARE);
                vector<int> deductionChild_2(g_parse_count, DONT_CARE);

                deductionChild_1 = this->all_possible_guess[i];
                deductionChild_1[j] = DONT_CARE;
                deductionChild_1[right] = MUST_TRUE;

                deductionChild_2 = this->all_possible_guess[i];
                deductionChild_2[j] = DONT_CARE;
                deductionChild_2[left] = MUST_TRUE;

                if (this->all_possible_guess[i][j] == PASS_DOWN_AND_MUST_TRUE) {
                    deductionChild_1[j] = PASS_DOWN_MUST_TRUE;
                    deductionChild_2[j] = PASS_DOWN_MUST_TRUE;
                }

                this->all_possible_guess.erase(this->all_possible_guess.begin() + i);
                this->all_possible_guess.push_back(deductionChild_1);
                this->all_possible_guess.push_back(deductionChild_2);
            }
        } else if (g_parse_tree[j]->type == AND) {
            if (this->all_possible_guess[i][j] == MUST_FALSE || this->all_possible_guess[i][j] == PASS_DOWN_AND_MUST_FALSE) {
                vector<int> deductionChild_1(g_parse_count, DONT_CARE);
                vector<int> deductionChild_2(g_parse_count, DONT_CARE);

                deductionChild_1 = this->all_possible_guess[i];
                deductionChild_1[j] = DONT_CARE;
                deductionChild_1[right] = MUST_FALSE;

                deductionChild_2 = this->all_possible_guess[i];
                deductionChild_2[j] = DONT_CARE;
                deductionChild_2[left] = MUST_FALSE;

                if (this->all_possible_guess[i][j] == PASS_DOWN_AND_MUST_FALSE) {
                    deductionChild_1[j] = PASS_DOWN_MUST_FALSE;
                    deductionChild_2[j] = PASS_DOWN_MUST_FALSE;
                }

                this->all_possible_guess.erase(this->all_possible_guess.begin() + i);
                this->all_possible_guess.push_back(deductionChild_1);
                this->all_possible_guess.push_back(deductionChild_2);
            }
        } else {
            //UNTIL and WNTIL
            if (this->all_possible_guess[i][j] == MUST_TRUE || this->all_possible_guess[i][j] == PASS_DOWN_AND_MUST_TRUE) {
                vector<int> deductionChild_1(g_parse_count, DONT_CARE);
                vector<int> deductionChild_2(g_parse_count, DONT_CARE);

                deductionChild_1 = this->all_possible_guess[i];
                deductionChild_1[j] = DONT_CARE;
                deductionChild_1[right] = MUST_TRUE;
                if (this->all_possible_guess[i][j] == PASS_DOWN_AND_MUST_TRUE) {
                    deductionChild_1[j] = PASS_DOWN_MUST_TRUE;
                }

                deductionChild_2 = this->all_possible_guess[i];
                deductionChild_2[j] = GUESSED_UNTIL_TRUE;
                deductionChild_2[left] = MUST_TRUE;

                this->all_possible_guess.erase(this->all_possible_guess.begin() + i);
                this->all_possible_guess.push_back(deductionChild_1);
                this->all_possible_guess.push_back(deductionChild_2);
            } else if (this->all_possible_guess[i][j] == MUST_FALSE || this->all_possible_guess[i][j] == PASS_DOWN_AND_MUST_FALSE) {
                vector<int> deductionChild_1(g_parse_count, DONT_CARE);
                vector<int> deductionChild_2(g_parse_count, DONT_CARE);

                deductionChild_1 = this->all_possible_guess[i];
                deductionChild_1[j] = DONT_CARE;
                deductionChild_1[left] = MUST_FALSE;
                deductionChild_1[right] = MUST_FALSE;
                if (this->all_possible_guess[i][j] == PASS_DOWN_AND_MUST_FALSE) {
                    deductionChild_1[j] = PASS_DOWN_MUST_FALSE;
                }

                deductionChild_2 = this->all_possible_guess[i];
                deductionChild_2[j] = GUESSED_UNTIL_FALSE;
                deductionChild_2[right] = MUST_FALSE;

                this->all_possible_guess.erase(this->all_possible_guess.begin() + i);
                this->all_possible_guess.push_back(deductionChild_1);
                this->all_possible_guess.push_back(deductionChild_2);
            }
        }
        continue;
    }
}

void ComputationTreeNode::PrintAllPossibleGuess() {
    int i, j;
    for (i = 0; i < g_parse_count; i++) {
        cout << setw(3) << g_parse_tree[i]->index;
    }
    cout << endl;
    for (i = 0; i < this->all_possible_guess.size(); i++) {
        for (j = 0; j < g_parse_tree.size(); j++) {
            cout << setw(3) << this->all_possible_guess[i][j];
        }
        cout << endl;
    }
}

void ComputationTreeNode::CheckLocal() {
    cout << "CheckLocal" << endl;
    int i, j;
    for (i = 0; i < this->all_possible_guess.size(); i++) {
        bool check_fail;
        check_fail = false;
        for (j = 0; j < g_parse_count; j++) {
            //List all fail cases
            //(MUST_TRUE,MUST_FALSE,OTHER)x(ATOMIC,TRUE_NODE,FALSE_NODE)
            if (this->all_possible_guess[i][j] == MUST_TRUE
                    || this->all_possible_guess[i][j] == PASS_DOWN_AND_MUST_TRUE) {
                if (g_parse_tree[j]->type == ATOMIC) {
                    char atomic_str[1000];
                    strcpy(atomic_str, g_parse_tree[j]->str.c_str());
                    if (red_and(red_diagram(atomic_str), this->state->red) == red_false()) {
                        check_fail = true;
                        break;
                    }
                } else if (g_parse_tree[j]->type == FALSE_NODE) {
                    check_fail = true;
                    break;
                }
            } else if (this->all_possible_guess[i][j] == MUST_FALSE
                       || this->all_possible_guess[i][j] == PASS_DOWN_AND_MUST_FALSE) {
                if (g_parse_tree[j]->type == ATOMIC) {
                    char atomic_str[1000];
                    strcpy(atomic_str, g_parse_tree[j]->str.c_str());
                    if (red_and(red_diagram(atomic_str), this->state->red) != red_false()) {
                        check_fail = true;
                        break;
                    }
                } else if (g_parse_tree[j]->type == TRUE_NODE) {
                    check_fail = true;
                    break;
                }
            }
        }
        if (check_fail) {
            this->all_possible_guess.erase(this->all_possible_guess.begin() + i);
            i--;
        }
    }
}

bool ComputationTreeNode::isDifferent(ComputationTreeNode* target){
    cout<<"diff: "<<this<<" "<<target<<endl;
    if(target->state == this->state){
        int i;
        for(i=0;i<g_parse_count;i++){
            if(g_parse_tree[i]->type == UNTIL || g_parse_tree[i]->type == WNTIL){
                if(this->all_possible_guess[this->current_guess][i]
                    != target->all_possible_guess[target->current_guess][i]){
                    return true;
                }
            }
        }
        return false;
    }
    return true;
}

int ComputationTreeNode::GetMaxNextHeight(){
    int i;
    int max = 0;
    for(i=0;i<g_parse_count;i++){
        if(this->obligation[i]!=DONT_CARE && max < g_parse_tree[i]->next_height){
            max = g_parse_tree[i]->next_height;
        }
    }
    return max;
}

int ComputationTreeNode::CheckPath(){
    cout<<"CheckPath: ";
    int i,j;
    vector<ComputationTreeNode*> repeated_node;
    int temp_length=0;
    int lasso_length=0;
    ComputationTreeNode* parent_node;
    parent_node = this->parent;
    while(parent_node != NULL){
        temp_length++;
        if(!this->isDifferent(parent_node)){
            lasso_length = lasso_length + temp_length;
            temp_length = 0;
            repeated_node.push_back(parent_node);
        }
        parent_node = parent_node->parent;
    }

    // there is no lasso
    if(repeated_node.size()==0){
        cout<<"Continue, no lasso"<<endl;
        return CONTINUE;
    }
    cout<<"lasso_length:"<< lasso_length << endl;
    // has not reached the depth of next
    if(this->GetMaxNextHeight() > lasso_length){
        cout<<"Continue, lasso shorter than NEXT depth."<<endl;
        return CONTINUE;
    }

    // MUST_TRUE UNTIL and MUST_FALSE WNTIL cannot be always passed down in a lasso
    // prepare unitl_array
    bool isUNTIL = false;
    vector<int> until_array = vector<int>(g_parse_count,-1);
    for(i=0;i<g_parse_count;i++){
        if((this->all_possible_guess[this->current_guess][i] == GUESSED_UNTIL_TRUE
            && g_parse_tree[i]->type == UNTIL)
            || (this->all_possible_guess[this->current_guess][i] == GUESSED_UNTIL_FALSE
            && g_parse_tree[i]->type == WNTIL)){
            isUNTIL = true;
            until_array[i] = 0;
        }
    }
    if(!isUNTIL){
        cout<<"PASS, no MUST_TRUE_UNTIL or MUST_FALSE_WNTIL exists."<<endl;
        return PASS;
    }
    // pass up unitl_array
    parent_node = this->parent;
    for(i=0;i<repeated_node.size();i++){
        bool something_satisfied;
        something_satisfied = false;
        while(parent_node != repeated_node[i]){
            // check all the entry in until_array
            for(j=0;j<g_parse_count;j++){
                if(until_array[j] != -1
                    && parent_node->all_possible_guess[parent_node->current_guess][j] < GUESSED_UNTIL_TRUE){
                    something_satisfied = true;
                    // label: satisfied by sub-lasso i + 1
                    until_array[j] = i + 1;
                }
            }
            parent_node = parent_node->parent;
        }
        if(!something_satisfied){
            cout<<"no UNTIL is satisfied in current sub-lasso"<<endl;
            return FAIL;
        }
    }
    bool satisfied_something_new = false;
    bool some_until_left = false;
    for(i=0;i<g_parse_count;i++){
        if(until_array[i] == 0){
            some_until_left = true;
        } else if(until_array[i] == 1){
            satisfied_something_new = true;
        }
    }
    if(!some_until_left){
        cout<<"PASS, all until satisfied.";
        return PASS;
    } else if(satisfied_something_new){
        cout<<"CONTINUE, still some until are unsat.";
        return CONTINUE;
    }
    cout<<"FAIL, current sub-lasso dosent satisfied any new UNTIL";
    return FAIL;
}

bool ComputationTreeNode::CreatePassDown() {
    int i;

    bool something_to_pass_down;
    something_to_pass_down = false;

    for (i = 0; i < g_parse_count; i++) {
        if (this->all_possible_guess[this->current_guess][i] == PASS_DOWN_MUST_TRUE
                || this->all_possible_guess[this->current_guess][i] == PASS_DOWN_AND_MUST_TRUE
                || this->all_possible_guess[this->current_guess][i] == GUESSED_UNTIL_TRUE) {
            something_to_pass_down = true;
            this->pass_down[i] = MUST_TRUE;
        } else if (this->all_possible_guess[this->current_guess][i] == PASS_DOWN_MUST_FALSE
                   || this->all_possible_guess[this->current_guess][i] == PASS_DOWN_AND_MUST_FALSE
                   || this->all_possible_guess[this->current_guess][i] == GUESSED_UNTIL_FALSE) {
            something_to_pass_down = true;
            this->pass_down[i] = MUST_FALSE;
        } else {
            continue;
        }
    }
    return something_to_pass_down;
}

void ComputationTreeNode::GetFirstStrategySelection() {
    int i, j;
    i = 0;

    cout << "current owner " << this->state->owner << endl;
    cout << "number of children " << this->state->outs.size() << endl;
    cout << "pass_down:";
    for (i = 0; i < g_parse_count; i++) {
        cout << this->pass_down[i] << " ";
    }
    cout << endl;

    this->strategy_selection.clear();

    vector<int> involved_owner_strategy = vector<int>(g_strategy_count, 0);
    i = 0;
    while (i < g_parse_count) {
        if (this->pass_down[i] == DONT_CARE) {
            i++;
            continue;
        }
        //copy the corresponding column from g_strategy_formula_matrix
        for (j = 0; j < g_strategy_count; j++) {
            if (g_strategy_formula_matrix[j][i] == 1
                    && g_strategy_to_owner[j] == this->state->owner) {
                involved_owner_strategy[j] = 1;
            }
        }
        i++;
    }
    for (i = 0; i < g_strategy_count; i++) {
        if (involved_owner_strategy[i] == 1) {
            this->strategy_selection[i] = 0;
        }
    }
}

bool ComputationTreeNode::GetNextStrategySelection() {
    for (map<int, int>::iterator it = strategy_selection.begin(); it != strategy_selection.end(); ++it) {
        if (it->second == this->state->outs.size() - 1) {
            it->second = 0;
        } else {
            it->second++;
            break;
        }
    }
}

void ComputationTreeNode::PrintStrategySelection() {
    int i;
    cout << "strategy_selection:" << endl;
    for (map<int, int>::iterator it = strategy_selection.begin(); it != strategy_selection.end(); ++it) {
        cout << it->first << " => " << it->second << endl;
    }
}

bool ComputationTreeNode::PassDown() {
    int i, j, k;
    for (i = 0; i < this->state->outs.size(); i++) {
        ComputationTreeNode child = ComputationTreeNode();
        child.parent = this;
        child.state = this->state->outs[i]->dst;
        //prepare the obligation of child
        for (j = 0; j < g_parse_count; j++) {
            if (this->pass_down[j] == DONT_CARE) {
                child.obligation[j] = DONT_CARE;
                continue;
            } else {
                bool is_controllable = false;
                bool is_target = false;
                for (k = 0; k < g_strategy_count; k++) {
                    if (g_strategy_formula_matrix[k][j] == 1
                            && g_strategy_to_owner[k] == this->state->owner) {
                        is_controllable = true;
                        if (strategy_selection[k] == i) {
                            is_target = true;
                            child.obligation[j] = this->pass_down[j];
                            break;
                        }
                    }
                }
                if (!is_controllable) {
                    child.obligation[j] = this->pass_down[j];
                } else if (!is_target) {
                    child.obligation[j] = DONT_CARE;
                }
            }
        }
        if(!child.CheckTCL()){
            return false;
        }
    }
    return true;
}

bool ComputationTreeNode::CheckTCL() {
    cout << "CheckTCL:" << endl;
    cout << red_diagram_string(this->state->red)<<endl;
    int i;
    for (i = 0; i < g_parse_count; i++) {
        cout << this->obligation[i] << " ";
    }
    cout << endl;
    this->ListAllPossibleGuess();
    this->PrintAllPossibleGuess();
    this->CheckLocal();
    this->PrintAllPossibleGuess();
    for (i = 0; i < this->all_possible_guess.size(); i++) {
        this->current_guess = i;
        if (!this->CreatePassDown()) {
            cout << "Nothing to pass down." << endl;
            cout << "<<<<<<<<<<<<<<<<<"<<endl;
            return true;
        }
        int check_path_result;
        check_path_result = this->CheckPath();
        if (check_path_result == PASS) {
            cout << "<<<<<<<<<<<<<<<<<"<<endl;
            return true;
        } else if (check_path_result == FAIL) {
            continue;
        }

        // Pass down according to strategy selection
        this->GetFirstStrategySelection();
        do {
            this->PrintStrategySelection();
            cout<<">>>>>>>>>>>>>>>>>>>>"<<endl;
            if (this->PassDown()) {
                cout << "All PassDown"<<endl;
                cout << "<<<<<<<<<<<<<<<<<"<<endl;
                return true;
            }
        } while (this->GetNextStrategySelection());
    }
    cout<<"no possible guess satisfied."<<endl;
    cout << "<<<<<<<<<<<<<<<<<"<<endl;
    return false;
}

void PrintGameGraph(TCLGameNode* root) {
    cout << root->index << ": " << red_diagram_string(root->red) << endl;
    int i;
    int lb, hb;
    TCLGameEdge* temp_edge;
    TCLGameNode* temp_node;
    string temp_string;
    for (i = 1; i < g_sxi_count; i++) {
        redgram temp_red = red_sync_xtion_fwd(
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
        if (red_and(temp_red, red_query_diagram_enhanced_global_invariance()) != red_false()) {
            if (node_map[temp_string.assign(red_diagram_string(temp_red))] == 0) {
                temp_edge = new TCLGameEdge;
                temp_node = new TCLGameNode;
                temp_edge->sxi = i;
                temp_edge->src = root;
                temp_edge->dst = temp_node;
                root->outs.push_back(temp_edge);
                temp_node->index = g_node_count;
                g_node_count++;
                temp_node->red = temp_red;
                red_get_cube_discrete_value(temp_node->red, input_owner, &lb, &hb);
                temp_node->owner = lb;
                temp_node->ins.push_back(temp_edge);
                g_nodes.push_back(temp_node);
                node_map[temp_string.assign(red_diagram_string(temp_red))] = temp_node->index;
                cout << root->index << "--" << temp_edge->sxi << "-->" << temp_node->index << endl;
                PrintGameGraph(temp_node);
            } else {
                temp_node = g_nodes[node_map[temp_string.assign(red_diagram_string(temp_red))]];
                temp_edge = new TCLGameEdge;
                temp_edge->sxi = i;
                temp_edge->src = root;
                temp_edge->dst = temp_node;
                root->outs.push_back(temp_edge);
                temp_node->ins.push_back(temp_edge);
                cout << root->index << "--" << temp_edge->sxi << "-->" << temp_node->index << endl;
            }
        }
    }
}

void PrintParseTree(TCLFormula* F, int depth) {
    int i;
    for (i = 0; i < depth; i++) {
        cout << "  ";
    }
    cout << F->index << " " << type_map[F->type] << " ";
    if (F->type == ATOMIC) {cout << F->str;}
    else if (F->type == PARSE_ROOT || F->type == PLUS) {cout << F->owner;}
    cout << endl;
    for (i = 0; i < F->outs.size(); i++) {
        PrintParseTree(F->outs[i], depth + 1);
    }
}

void PrintMatrix() {
    int i, j;
    cout << setw(24) << " strategy_formula_matrix";
    for (i = 0; i < g_parse_count; i++) {
        cout << setw(3) << g_parse_tree[i]->index;
    }
    cout << endl;
    for (i = 0; i < g_strategy_count; i++) {
        string temp_string;
        stringstream ss(temp_string);
        ss << i << "(" << g_strategy_to_owner[i] << ")";
        cout << setw(24) << ss.str();
        for (j = 0; j < g_parse_count; j++) {
            cout << setw(3) << g_strategy_formula_matrix[i][j];
        }
        cout << endl;
    }
    cout << endl;
}

void FillMatrix(TCLFormula* F, int* inheritedStrategy) {
    int i;
    int* currentStrategy = new int[g_strategy_count]();
    for (i = 0; i < g_strategy_count; i++) {
        currentStrategy[i] = inheritedStrategy[i];
    }

    // start preparing currentStrategy
    // clean previous strategy with the same owner
    if (F->type == PLUS || F->type == MINUS) {
        for (i = 0; i < g_strategy_count; i++) {
            if (g_strategy_to_owner[i] == F->owner) {
                currentStrategy[i] = 0;
            }
        }
    }
    // Add current owner's strategy
    if (F->type == PARSE_ROOT || F->type == PLUS || F->type == MINUS) {
        currentStrategy[F->strategy_index] = 1;
    }
    // finish preparing currentStrategy

    // fill the matrix according to currentStrategy
    for (i = 0; i < g_strategy_count; i++) {
        g_strategy_formula_matrix[i][F->index] = currentStrategy[i];
    }

    // 1 way pass down if needed
    if (F->type == PARSE_ROOT || F->type == PLUS || F->type == MINUS
            || F->type == NOT || F->type == NEXT) {
        FillMatrix(F->outs[0], currentStrategy);
    }

    // 2 ways pass down if needed
    if (F->type == OR || F->type == AND || F->type == UNTIL
            || F->type == WNTIL) {
        FillMatrix(F->outs[0], currentStrategy);
        FillMatrix(F->outs[1], currentStrategy);
    }
    delete currentStrategy;
}

void SetupMatrix() {
    int i, j;
    g_strategy_formula_matrix = new int*[g_strategy_count];
    for (i = 0; i < g_strategy_count; i++) {
        g_strategy_formula_matrix[i] = new int[g_parse_count]();
    }
}

void PrintStrategy2owner() {
    int i;
    cout << "g_strategy_to_owner:" << endl;
    for (i = 0; i < g_strategy_to_owner.size(); i++) {
        cout << g_strategy_to_owner[i] << " ";
    }
    cout << endl;
}

void SetupTCLFormula(TCLFormula* F) {
    int i;
    F->next_height=0;
    g_parse_tree.push_back(F);
    F->index = g_parse_count;
    g_parse_count++;
    if (F->type == PARSE_ROOT || F->type == PLUS) {
        g_strategy_to_owner.push_back(F->owner);
        F->strategy_index = g_strategy_count;
        g_strategy_count++;
    } else if (F->type == MINUS) {
        F->strategy_index = -1;
    } else {
        F->strategy_index = 0;
    }
    for (i = 0; i < F->outs.size(); i++) {
        SetupTCLFormula(F->outs[i]);
    }
    for (i = 0; i < F->outs.size(); i++) {
        if(F->next_height < F->outs[i]->next_height){
            F->next_height = F->outs[i]->next_height;
        }
    }
    if(F->type == NEXT){
        F->next_height++;
    }
}

int main(int argc, char** argv) {
    int i, j, k;
    int lb, hb;
    string temp_string;
    g_sxi_count = 0;
    g_node_count = 1;
    g_strategy_count = 0;
    g_parse_count = 0;
    g_parse_count = 0;
    //initialize global variables

    red_begin_session(RED_SYSTEM_TIMED, argv[1], -1); // -P n; n = process number, -1 == default
    red_input_model(argv[1], RED_REFINE_GLOBAL_INVARIANCE);
    red_set_sync_bulk_depth(3);
    //read in the model

    TCLGameNode* root;
    root = new TCLGameNode;
    g_nodes.push_back(root);  //dummy one
    g_nodes.push_back(root);
    root->red = red_query_diagram_initial();
    red_get_cube_discrete_value(root->red, input_owner, &lb, &hb);
    root->owner = lb;
    root->index = g_node_count; g_node_count++;
    node_map[temp_string.assign(red_diagram_string(root->red))] = root->index;
    path = red_query_diagram_enhanced_global_invariance();
    g_sxi_count = red_query_sync_xtion_count(RED_USE_DECLARED_SYNC_XTION);
    type_map[TRUE_NODE] = "TRUE";
    type_map[FALSE_NODE] = "FALSE";
    type_map[PARSE_ROOT] = "ROOT";
    type_map[ATOMIC] = "ATOMIC";
    type_map[NOT] = "NOT";
    type_map[OR] = "OR";
    type_map[AND] = "AND";
    type_map[PLUS] = "PLUS";
    type_map[MINUS] = "MINUS";
    type_map[UNTIL] = "UNTIL";
    type_map[WNTIL] = "WNTIL";
    type_map[NEXT] = "NEXT";
    //initial variables

    cout << endl;
    PrintGameGraph(root);
    //Draw the graph

    FILE *infile = fopen(argv[2], "r");
    yyin = infile;
    do {
        yyparse();
    } while (!feof(yyin));

    SetupTCLFormula(g_parse_root);

    PrintStrategy2owner();

    SetupMatrix();

    int* currentStrategy = new int[g_strategy_count]();

    FillMatrix(g_parse_tree[0], currentStrategy);

    PrintMatrix();
    PrintParseTree(g_parse_tree[0], 0);
    //read in the formula

    ComputationTreeNode* R = new ComputationTreeNode;
    R->parent = NULL;
    R->state = root;
    R->obligation[0] = MUST_TRUE;
    //Init computation tree

    //CheckTCL(R);
    cout << "Start Guessing" << endl;
    cout << R->CheckTCL() << endl;

    return 0;
}

