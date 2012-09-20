#define CProc 200

process count=4; 

global synchronizer req, busy, ack; 

global discrete 
  server, turn: 0..(CProc+1); 



mode client_idle (true) { 
  when !req (true) may goto client_waiting; 
}

mode client_waiting (true) { 
  when ?busy (true) may goto client_idle; 
  when ?ack@Q (true) may server = Q; goto client_idle; 
} 

mode server_standby (true){
  when ?req (turn == P) may goto server_process; 
}

mode server_process (true){
  when !busy (P==#PS) may turn = 2; goto server_standby; 
  when !busy (P<#PS) may turn++1; goto server_standby; 
  when !ack (true) may goto server_standby; 
}



initially 
    turn == 2 
&&  server == 2 
&&  client_idle@(1) 
&&  forall k:2..#PS, server_standby@(k);


/************************************

[1..#PS]always (<2..#PS> eventually server == 1)   

[1..#PS]always forall k:3..#PS,(turn == k 
  => <k,#PS> eventually <+2> always server == 2) 
  
[1..#PS] eventually <+> always server == 2 


*/ 