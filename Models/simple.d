process count = 3;


global discrete
    p,q:0..1;

global discrete
    turn:0..2;

global synchronizer p1_1,p1_2,p2;

mode dummy_0(true){
    when !p1_1 (true) may turn = 1; q=0; goto dummy_1;
    when !p1_2 (true) may turn = 2; p=0; goto dummy_2;
}

mode dummy_1(true){
    when !p2 (true) may; goto dummy_1;
}

mode dummy_2(true){
    when !p2 (true) may; goto dummy_2;
}

mode player1(true){
    when ?p1_1 (true) may ; goto player1;
    when ?p1_2 (true) may ; goto player1;
}

mode player2(true){
    when ?p2 (true) may ; goto player2;
}

initially
    turn == 1
&& p == 1
&& q == 1
&& dummy_0@(1)
&& player1@(2)
&& player2@(3);
