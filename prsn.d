process count=4;

global synchronizer 
  interrogate, 
  coop, 
  deny, 
  jail, 
  acquit; // 

global discrete
  liaobei, turn: 0..#PS;   

mode police_idle(true){
  when !(#PS-1)interrogate (true) may 
    turn = 2; liaobei = 0; goto police_waiting;
}

mode police_waiting (true) { 
  when ?coop@Q (liaobei == 0 && turn < #PS) may turn++1; liaobei = Q; 
  
  when ?coop@Q (liaobei == 0 && turn == #PS) may turn=1; liaobei = Q; 
  
  when ?coop (liaobei != 0) may turn = 1; liaobei = 0; goto police_jail_all; 
  
  when ?deny (turn < #PS) may turn++1; 
  when ?deny (turn == #PS) may turn=1; 
  
  when !(#PS-1)acquit (liaobei == 0 && forall h:2..#PS, criminal_waiting@(h)) may 
    turn = 1; goto police_idle; 
  
  when !acquit@Q (liaobei==Q && forall h:2..#PS, criminal_waiting@(h)) may 
    turn = 1; goto police_jail_rest; 
}

mode police_jail_all (true) { 
  when !(#PS-1)jail (true) may 
    turn = 1; goto police_idle; 
}

mode police_jail_rest (true) { 
  when !(#PS-2)jail (true) may 
    turn = 1; liaobei = 0; goto police_idle; 
} 


mode criminal_free (true) { 
  when ?interrogate (true) may goto criminal_agonized;
}

mode criminal_jail (true) { 
  when ?interrogate (true) may goto criminal_agonized;
}

mode criminal_agonized (true) { 
  when !coop (turn == P) may goto criminal_waiting; 
  when !deny (turn == P) may goto criminal_waiting; 
  when ?jail (true) may goto criminal_jail; 
} 

mode criminal_waiting (true) { 
  when ?acquit (true) may goto criminal_free; 
  when ?jail (true) may goto criminal_jail; 
} 

initially 
    turn==1
&&  liaobei==0
&&  police_idle@(1)  
&&  forall k:2..#PS, criminal_free@(k);


/* 
==============================================================
Property 1: The other criminals have a strategy to free criminal 2. 
  But with this strategy, they let criminal decide 
  whether they will be in jail.
  
<3..#PS>(
     <+> always ~criminal_jail@(2) 
  && <+> eventually <+2> always forall k:3..#PS, ~criminal_jail@(k)
  && <+> eventually <+2> always forall k:3..#PS, ~criminal_free@(k)
)

================================================================
Property 2: Criminal 2 has a strategy to jail the other criminals 
  and let the others decide whether he/she will be in jail. 
  
<2>(
     <+> always forall k:3..#PS, ~criminal_free@(k) 
  && <+> eventually <+3..#PS> always ~criminal_jail@(2)
  && <+> eventually <+3..#PS> always ~criminal_free@(2)
)

================================================================
Property 2: A Nash equillibrium 

<2..#PS>(
     <+> always forall k:2..#PS, ~criminal_free@(k) 
  && forall k:2..#PS, [+k] always ~criminal_free@(k)
)

*/  