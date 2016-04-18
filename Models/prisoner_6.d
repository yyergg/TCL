#define Prisoner_Count 6
process count=Prisoner_Count+1;


global discrete
    counter_confess,
    counter_deny:0..Prisoner_Count;
global discrete
    turn:0..Prisoner_Count;


global synchronizer ask,judge; 

mode police(true){
    when !ask@Q (turn<Prisoner_Count && turn==Q) may turn++1;
    when !ask@Q (turn==Prisoner_Count && turn==Q) may turn=0;
    when !(Prisoner_Count)judge (turn==0) may turn=1; counter_confess=0; counter_deny=0;
}

mode prisoner_jail(true){
    when ?ask (true) may counter_confess++1; goto prisoner_confess;
    when ?ask (true) may counter_deny++1; goto prisoner_deny;
}

mode prisoner_free(true){
    when ?ask (true) may counter_confess++1; goto prisoner_confess;
    when ?ask (true) may counter_deny++1; goto prisoner_deny;
}

mode prisoner_confess(true){
    when ?judge (counter_confess>1) may goto prisoner_jail;
    when ?judge (counter_confess==1) may goto prisoner_free;
}

mode prisoner_deny(true){
    when ?judge (counter_confess>0) may goto prisoner_jail;
    when ?judge (counter_confess==0) may goto prisoner_free;
}

initially
turn == 1
&&counter_confess==0
&&counter_deny==0
&&forall k:1..Prisoner_Count, prisoner_free@(k);
&&police@(Prisoner_Count+1);

/*
property
nice 1:
<1>G(prisoner_confess@(2) || ~<+> F prisoner_confess@(1))

2 betray if 1 not nice:
<2>( (<+>G((turn==2) -> (<+>X prisoner_confess@(2)))) || ~(<+1>G(prisoner_confess@(2) || ~<+> F prisoner_confess@(1))))


*/

