process count=3;

global discrete
    prisoner_1_state,
    prisoner_2_state:0..1;
    
global discrete
    p1_choose,
    p2_choose:0..1;
//variables to record last selection of players    
global synchronizer
    prisoner_1_confess,
    prisoner_1_deny,
    prisoner_2_confess,
    prisoner_2_deny;

mode prisoner_1(true){
    when !prisoner_1_confess (true) may p1_choose=1; goto prisoner_1;
    when !prisoner_1_deny (true) may p1_choose=0; goto prisoner_1;
}    

mode prisoner_2(true){
    when !prisoner_2_confess (true) may p2_choose=1; goto prisoner_2;
    when !prisoner_2_deny (true) may p2_choose=0; goto prisoner_2;
}
    
    
mode police(true){
    when ?prisoner_1_confess ?prisoner_2_confess(true) may prisoner_1_state=1; prisoner_2_state=1; goto police;
    when ?prisoner_1_confess ?prisoner_2_deny(true) may prisoner_1_state=0; prisoner_2_state=1; goto police;
    when ?prisoner_1_deny ?prisoner_2_confess(true) may prisoner_1_state=1; prisoner_2_state=0; goto police;
    when ?prisoner_1_deny ?prisoner_2_deny(true) may prisoner_1_state=0; prisoner_2_state=0; goto police;
}    

initially
prisoner_1_state==0
&&prisoner_2_state==0
&&p1_choose==0
&&p2_choose==0
&&prisoner_1@(1)
&&prisoner_2@(2)
&&police@(3);