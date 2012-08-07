process count=1;

global discrete
	turn:0..2;
	
global discrete
	p,q:0..1;	
	
mode S0(true){
	when(true) may p=0; q=0; turn=2; goto S1;
	when(true) may p=1; q=1; turn=1; goto S0;
}


mode S1(true){
}

mode S2(true){
}

initially 
	turn==1
&&p==1
&&q==0
&&S0@(1);	
