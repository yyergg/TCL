#define CProc 200

process count=2; 

global synchronizer req, vote, safety_1, fault; 

global discrete 
  cfault, 
  cidle, 
  crun, 
  cgood, 
  cbad, 
  counter_p_idle,
  turn: 0..(CProc+1); 



mode monitor (true) { 
  when !req (true) may ; 
  when !vote (true) may ; 
  when !safety_1 (true) may ; 
}

mode Proc_standby(true){
  when ?req (turn == 1 && counter_p_idle > 0) may 
    turn = 2; 
    cidle = 0;  
    crun = cidle; 
    cfault = 0; 
    cgood = 0; 
    cbad = 0; 

  when ?vote (cgood > (#PS-1)/2 -1) may 
    turn = 1; 
    cidle = CProc; 
    crun = 0; 
    cfault = 0; 
    cgood = 0;  
    cbad = 0;  
    goto Proc_good; 

  when ?vote (cbad > (#PS-1)/2 -1) may 
    turn = 1; 
    cidle = CProc; 
    crun = 0; 
    cfault = 0; 
    cgood = 0;  
    cbad = 0;  
    goto Proc_bad; 

  when ?safety_1 (cfault > #PS/3) may goto Proc_safety; 
  when ?safety_1 (cfault <= #PS/3) may ; 
  
  when (crun > 0) may crun--1; cgood++1; 
  when (crun > 0) may turn++1; crun--1; cfault++1; 
  when (cfault > 0) may turn++1; cfault--1; cbad++1; 
}

mode Proc_safety(true){
  when ?req (turn == 1 && counter_p_idle > 0) may 
    turn = 2; 
    cidle = 0;  
    crun = cidle; 
    cfault = 0; 
    cgood = 0; 
    cbad = 0; 

  when ?vote (cgood > (#PS-1)/2 -1) may 
    turn = 1; 
    cidle = CProc; 
    crun = 0; 
    cfault = 0; 
    cgood = 0;  
    cbad = 0;  
    goto Proc_good; 

  when ?vote (cbad > (#PS-1)/2 -1) may 
    turn = 1; 
    cidle = CProc; 
    crun = 0; 
    cfault = 0; 
    cgood = 0;  
    cbad = 0;  
    goto Proc_bad; 

  when (crun > 0) may crun--1; cgood++1; 
}

mode Proc_good(true){
  when (true) may goto Proc_standby; 
}

mode Proc_bad(true){
  when (true) may goto Proc_standby; 
}



initially 
    turn == 0 
&&  cidle==CProc
&&  crun==0
&&  cfault==0
&&  cgood==CProc
&&  cbad==0
&&  monitor@(1) 
&&  Proc_standby@(2);


/************************************

<> always (cfault < #PS/2 => 
  exists k:2..#PS, <k> eventually <+> always ~Proc_bad@(2))   

*/ 