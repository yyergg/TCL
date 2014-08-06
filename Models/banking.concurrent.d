process count = 5; // 1: banking system, 2: partner bank, 3..5: customers

global synchronizer query, transfer, busy, done, xchange; 
global discrete request_client_flag, result_client_flag: 0..#PS;

// modes for the banking system 
mode system_standby (true) { 
  when ?query@Q (true) 
    may request_client_flag = Q; goto system_query; 
  when ?transfer@Q (true) 
    may request_client_flag = Q; goto system_transfer; 
} 

mode system_query (true) { 
  when !busy@Q (Q == request_client_flag) 
    may request_client_flag = 0; goto system_standby; 
  when !done@Q (Q == request_client_flag) 
    may 
    result_client_flag = request_client_flag; request_client_flag = 0; 
    goto system_waiting; 
} 

mode system_transfer (true) { 
  when !busy@Q (Q == request_client_flag) may request_client_flag = NULL; goto system_standby; 
  when !xchange@Q (Q == request_client_flag) may goto system_xchange; 
} 

mode system_xchange (true) { 
  when !busy@Q (Q == request_client_flag) 
    may request_client_flag = 0; goto system_standby; 
  when !done@Q (Q == request_client_flag) 
    may 
    result_client_flag = request_client_flag; request_client_flag = 0; 
    goto system_waiting; 
} 

mode system_waiting (true) { 
  when (result_client_flag == 0) may goto system_standby; 
} 

// mode for the partner bank
mode partner (true) { 
  when ?xchange (true) may; 
} 

// mode for the clients 
mode client_idle (true) { 
  when !query (true) may goto client_query; 
  when !transfer (true) may goto client_transfer; 
} 
mode client_query (true) { 
  when ?busy (true) may goto client_idle; 
  when ?done (true) may goto client_result; 
} 

mode client_transfer (true) { 
  when ?busy (true) may goto client_idle; 
  when ?done (true) may goto client_result; 
} 

mode client_result (true) { 
  when (result_client_flag == P) may result_client_flag = 0; goto client_idle;  
} 

initially 
  request_client_flag == 0 
&&  result_client_flag == 0 
&& system_standby@(1) 
&& partner@(2) 
&& forall k: 3..#PS, client_idle@(k); 

// The property is <1> always(system_idle implies <1> (
//     (always ~(client_transfer@(3) || client_query@(3))) implies always result_client_flag!=3)
// && <+2> ((eventually client_query@(3))&& eventually result_client_flag == 3)
// && <+2> ((eventually client_transfer@(3)&& eventually result_client_flag == 3)
// ) 
