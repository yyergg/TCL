process count=4; 

global synchronizer bank_pass;
global synchronizer bank_success;
global synchronizer bank_fail;
global synchronizer bank_query;


global synchronizer client_pass;
global synchronizer client_deposit;
global synchronizer client_cancel;
global synchronizer client_commit;
global synchronizer client_ok;
global synchronizer client_transfer;


global synchronizer partnerbank_pass;
global synchronizer partnerbank_busy;
global synchronizer partnerbank_confirm;

global discrete
	bank_choose,
	partner_choose,
	client_choose:0..4;


mode bank(true){
    when !bank_pass (true) may bank_choose=0; goto bank;
    when !bank_success (true) may bank_choose=1; goto bank;
    when !bank_fail (true) may bank_choose=2; goto bank;
    when !bank_query (true) may bank_choose=3; goto bank;
}

mode client(true){
    when !client_pass (true) may client_choose=0; goto client;
    when !client_deposit (true) may client_choose=1; goto client;
    when !client_cancel (true) may client_choose=2; goto client;
    when !client_commit (true) may client_choose=3; goto client;
    when !client_transfer (true) may client_choose=4; goto client;
}

mode partnerbank(true){
    when !partnerbank_pass (true) may partner_choose=0; goto partnerbank;
    when !partnerbank_busy (true) may partner_choose=1; goto partnerbank;
    when !partnerbank_confirm (true) may partner_choose=2; goto partnerbank;    
}

mode gamegraph_idle(true){
    when ?bank_pass ?client_deposit ?partnerbank_pass (true) may goto gamegraph_dep;
    when ?bank_pass ?client_transfer ?partnerbank_pass (true) may goto gamegraph_xfer;
}

mode gamegraph_dep(true){
    when ?bank_success ?client_cancel ?partnerbank_pass (true) may goto gamegraph_idle;
    when ?bank_fail ?client_commit ?partnerbank_pass (true) may goto gamegraph_idle;
    when ?bank_success ?client_commit ?partnerbank_pass (true) may goto gamegraph_depDone;
}

mode gamegraph_depDone(true){
    when ?bank_pass ?client_ok ?partnerbank_pass (true) may goto gamegraph_idle;
}

mode gamegraph_xfer(true){
    when ?bank_query ?client_pass ?partnerbank_busy (true) may goto gamegraph_xfer;
    when ?bank_query ?client_cancel ?partnerbank_confirm (true) may goto gamegraph_idle;
    when ?bank_query ?client_pass ?partnerbank_confirm (true) may goto gamegraph_xferCfm;
}

mode gamegraph_xferCfm(true){
    when ?bank_success ?client_commit ?partnerbank_pass (true) may goto gamegraph_xferDone;
}

mode gamegraph_xferDone(true){
    when ?bank_pass ?client_ok ?partnerbank_pass (true) may goto gamegraph_idle;
}

initially 
    bank@(1)
&& client@(2)
&& partnerbank@(3)
&& gamegraph_idle@(4);
