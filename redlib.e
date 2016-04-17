#ifdef __cplusplus
extern "C" {
#endif

#ifndef _REDLIB_E 
#define _REDLIB_E 

/*****************************************************************
 *  variables and procedures
 */
extern FILE 	*RED_OUT; 

extern struct index_link_type	*add_index(), *add_index_count();
extern int			free_index_list();



extern void	red_switch_output(FILE *out); 

extern char	*red_file_to_string(
  char	*f
); 

extern char    *red_diagram_string(redgram d); 
	
extern int red_diagram_size(
	redgram	d, 
	int	*node_count_ptr, 
	int	*arc_count_ptr 
	); 
	
extern int red_diagram_cube_count( 
	redgram	d
	); 

extern int red_diagram_discrete_model_count( 
	redgram	d
	); 

extern int red_diagram_space(
	redgram	d
); 

extern void	red_begin_session(
	int	type_system, 
	char	*session_name, 
	int	process_count
	); 

extern void	red_end_session();


extern void		red_change_declaration(int flag_vars, int flag_rules); 
extern inline void	red_begin_declaration();


extern void	red_end_declaration(
  int flag_refine_global_invariance
/* Two values for the argument are 
 * 
 *  RED_NO_REFINED_GLOBAL_INVARIANCE
 *  RED_REFINE_GLOBAL_INVARIANCE
 *  The latter says to reconstruct a refined forward reachability. 
 *  The former says no. 
 */ 
); 

extern void 	red_input_model(
  char *mf, 
  int flag_refine_global_invariance
); 
extern void	red_input_rules(
  char *mf, 
  int flag_refine_global_invariance
); 


extern void 	red_comment(char *com); 

extern void 	red_define_const(char *m, int i); 

extern void 	red_declare_variable(
	int vtype, int lb, int ub, char *f, ...
	); 
  
extern void 	red_declare_local_variable(
	int vtype, int lb, int ub, char *f, ...
	); 

extern void	red_begin_mode(
	int flag_urgent, char *name, char *inv, ...
);  

extern void	red_end_mode(); 


extern void 	red_transition(char *rule, ...);

extern void 	red_dense_rate(char *vname, char *interval, ...); 

extern void 	red_set_initial(char *constraint, ...);




extern void 	red_set_spec(int task, char *constraint, ...);  
  
  

extern int 	red_query_var_count(); 
extern int 	red_query_var_attribute(int, int); 
extern char	*red_query_var_name(int);  
extern int 	red_query_local_var_index(char *, int);
extern int 	red_query_var_index(char *);
extern redgram	red_query_var_active(int); 
extern redgram 	red_query_var_inactive(int);

extern int 	red_query_clock_count();
extern int 	red_query_clock_var(int); 
extern int 	red_query_zone_index(int, int); 
  
  

extern int 	red_query_xtion_count(); 
extern int 	red_query_xtion_attribute(int xi, int attr);
extern int	red_query_xtion_stream_operation(int, int); 
extern char 
  *red_query_xtion_src_lines(int), 
  *red_query_xtion_stream_name(int, int); 
extern redgram	red_query_diagram_xtion_trigger(int xi, int pi); 

extern int	red_query_xtion_sync_attribute(int xi, int si, int attr); 
/*  The values of attr can be RED_XTION_SYNC_DIRECTION, 
 *  RED_XTION_SYNC_QUANTIFIED_ADDRESS, RED_XTION_SYNC_VAR_INDEX, and 
 *  RED_XTION_SYNC_QFD_CORRESPONDENCE_VAR_INDEX. 
 *  The return values for attribute RED_XTION_SYNC_DIRECTION are 
 *  RED_XTION_SYNC_XMIT and RED_XTION_SYNC_RECV.  
 *  The return values for attribute case RED_XTION_SYNC_QUANTIFIED_ADDRESS 
 *  can be RED_XTION_SYNC_NO_CORRESPONDENCE_REQUIREMENT, 
 *  RED_XTION_SYNC_QUANTIFIED_CORRESPONDENCE_VAR, and 
 *  RED_XTION_SYNC_CORRESPONDENCE_EXPRESSION. 
 *  The return values for attribute RED_XTION_SYNC_VAR_INDEX 
 *  is the variable index for the synchronizer. 
 * 
 *  If attribute RED_XTION_SYNC_QUANTIFIED_ADDRESS is of value 
 *  RED_XTION_SYNC_QUANTIFIED_CORRESPONDENCE_VAR, 
 *  then the return values for attribute 
 *  RED_XTION_SYNC_QFD_CORRESPONDENCE_VAR_INDEX is the 
 *  variable index for the holder of the address of the process 
 *  corresponding to this synchronization. 
 *  Otherwise, the return value is RED_FLAG_UNKNOWN. 
 */

extern char 
  // The following procedure can be used to query the string for a 
  // synchronization in a transiton. 
  // To get the synchronization names of a transition with index xi, 
  // we can execute the following loop. 
  /* for (i = 0; 
  //      i < red_query_xtion_attribute(xi, RED_XTION_SYNC_COUNT); 
  //      i++
  //      ) {
  //   printf("\nxi:%1d, si:%1d, %s", xi, i, 
  //     red_query_string_xtion_sync(xi,i)
  //   ); 
  // }
  // printf("\n"); 
  */ 
  *red_query_string_xtion_sync(
    int, // This is a transition index
    int  // This is a sync index in the list of synchronizations of the 
         // the transition. 
  ), 
  *red_query_string_xtion_sync_correspondence_exp(
    int, // This is a transition index
    int  // This is a sync index in the list of synchronizations of the 
         // the transition. 
  ); 
extern char 	*red_query_string_xtion_action(int, int); 
extern char 	*red_query_string_xtion(int, int); 

extern int 
  red_query_xtion_process(
    int, // the transition index
    int  // the index to the process index in the list of processes
         // that may execute transitoin xi
  ); 
  
  
extern int 	red_query_process_count();  

extern int	red_query_process_role(int);  

extern int
  red_query_process_mode_count(
    int	// the index of the process whose reachable mode count is queried.
  ),  
  red_query_process_mode(
    int, // the index of the process whose reachable mode is queried. 
    int  // the index to the mode index list of modes that is reachable 
         // by the process.  
  ), 
  red_query_process_xtion_count(
    int // the index of the process whose executable transition count 
        // is queried.
  ),  
  red_query_process_xtion(
    int, // the index of the process whose executable transition count 
         // is queried.
    int  // the index to the transition index list of transitions 
         // that can be executed by the process.  
  );
  
  
  
extern int 	red_query_mode_count(); 
extern int 	red_query_mode_attribute(int mi, int attr); 
extern int 	red_query_mode_rate(int mi, int attr); 
extern char 
  *red_query_mode_name(int /* mi */), 
  *red_query_mode_src_lines(int /* mi*/); 
extern int 	red_query_mode_xtion(int mi, int ixi); 
extern int 	red_query_mode_process(int mi, int ipi);  
extern redgram 	red_query_diagram_mode_invariance(int mi, int pi); 
extern redgram 	red_query_diagram_declared_invariance(); 




extern int red_query_system_type(); 
  
extern int 	red_query_sync_bulk_depth();  
  
extern void    red_set_sync_bulk_depth(int d);

extern int 	red_query_sync_xtion_count(
	int 	flag_sync_xtion_table_choice  
		// This argument is used to choose from the declared 
		// synchronous transition table and the 
		// game synchronous transition table. 
		// There are the following two values.  
		// RED_USE_GAME_SYNC_XTION
		// RED_USE_DECLARED_SYNC_XTION 
);  

extern int 	red_query_sync_xtion_party_count(
	int 	flag_sync_xtion_table_choice,  
		// This argument is used to choose from the declared 
		// synchronous transition table and the 
		// game synchronous transition table. 
		// There are the following two values.  
		// RED_USE_GAME_SYNC_XTION
		// RED_USE_DECLARED_SYNC_XTION 
	int 	sxi
);

extern int 	red_query_sync_xtion_party_process(
  int 	flag_sync_xtion_table_choice, 
  int	sxi, 
  int	party_index
); 



extern int 	red_query_sync_xtion_party_xtion(
  int 	flag_sync_xtion_table_choice, 
  int	sxi, 
  int	party_index
); 


extern redgram red_query_diagram_sync_xtion_trigger(
	int 	flag_sync_xtion_table_choice,  
		// This argument is used to choose from the declared 
		// synchronous transition table and the 
		// game synchronous transition table. 
		// There are the following two values.  
		// RED_USE_GAME_SYNC_XTION
		// RED_USE_DECLARED_SYNC_XTION 
	int 	sxi
); 
  
extern char    *red_query_string_sync_xtion_action(
	int 	flag_sync_xtion_table_choice,  
		// This argument is used to choose from the declared 
		// synchronous transition table and the 
		// game synchronous transition table. 
		// There are the following two values.  
		// RED_USE_GAME_SYNC_XTION
		// RED_USE_DECLARED_SYNC_XTION 
	int 	sxi
);
  
extern char    *red_query_string_sync_xtion(
	int 	flag_sync_xtion_table_choice,  
		// This argument is used to choose from the declared 
		// synchronous transition table and the 
		// game synchronous transition table. 
		// There are the following two values.  
		// RED_USE_GAME_SYNC_XTION
		// RED_USE_DECLARED_SYNC_XTION 
	int 	sxi
); 
  
extern redgram red_query_diagram_xtion_sync_bulk(
	int 	flag_sync_xtion_table_choice  
		// This argument is used to choose from the declared 
		// synchronous transition table and the 
		// game synchronous transition table. 
		// There are the following two values.  
		// RED_USE_GAME_SYNC_XTION
		// RED_USE_DECLARED_SYNC_XTION 
);
  
extern redgram red_query_diagram_xtion_sync_bulk_with_trigger(
	int 	// flag_sync_xtion_table_choice  
		// This argument is used to choose from the declared 
		// synchronous transition table and the 
		// game synchronous transition table. 
		// There are the following two values.  
		// RED_USE_GAME_SYNC_XTION
		// RED_USE_DECLARED_SYNC_XTION 
);

// The following procedure returns the triggering condition
// of those processes with roles specified in the second argument 
// recorded in the red_query_diagram_xtion_sync_bulk_with_trigger(). 
// It is implemented to support experiment that extracts triggering conditions 
// of all transitions.   
extern redgram red_query_diagram_triggers_from_sync_bulk_with_roles(
  int,  	// flag_sync_xtion_table_choice, 
		// This argument is used to choose from the declared 
		// synchronous transition table and the 
		// game synchronous transition table. 
		// There are the following two values.  
		// RED_USE_GAME_SYNC_XTION
		// RED_USE_DECLARED_SYNC_XTION 
  int		// flag_game_roles 
); 

extern redgram red_query_diagram_xtion_sync_bulk_whole(
	int 	flag_sync_xtion_table_choice  
		// This argument is used to choose from the declared 
		// synchronous transition table and the 
		// game synchronous transition table. 
		// There are the following two values.  
		// RED_USE_GAME_SYNC_XTION
		// RED_USE_DECLARED_SYNC_XTION 
);
  
  
  
  
extern int 	red_clock_oo();
  
  
  
extern int  	red_hybrid_oo();
  
  
  
  
extern redgram 	red_true(); 
  
  
  
extern redgram 	red_false();
  
  
  
extern void	red_begin_string(char *f, ...); 
  
extern void	red_concat_string(char *f, ...);   
  
extern char	*red_finalize_string();  

extern redgram red_diagram(char *f, ...); 
  
extern redgram red_diagram_events(char *f, ...); 
  
extern redgram red_diagram_local(int pi, char *f, ...); 

extern redgram red_quantify(redgram d, char *f, ...); 



extern redgram red_and(redgram d1, redgram d2);  
  
  
  
extern redgram red_or(struct red_type *d1, struct red_type *d2);  



extern redgram red_subtract(struct red_type *d1, struct red_type *d2); 

extern redgram red_not(struct red_type *d); 
  
  

extern redgram 
  red_time_bck(struct red_type *dst, struct red_type *path); 

extern redgram 
  red_time_fwd(struct red_type *src, struct red_type *path);
 
extern redgram 	
  red_time_measurement_initial_fwd(
    redgram src, redgram path, int time_lb, int time_ub
  ),   
  red_measured_time_fwd(redgram src, redgram path);  

extern void 
  red_measure_last_time(redgram d, int *lb_ptr, int *ub_ptr), 
  red_measure_acc_time(redgram d, int *lb_ptr, int *ub_ptr);
 
 
  
  


extern struct xtion_type	*parse_xtion(char *xt); 
  
  
  
  
extern redgram red_sync_xtion_string_bck(
  redgram	ddst, 
  redgram       dpath, 
  int		flag_game_roles,  
  int		flag_time_progress, 
		// Two argument values are 
		// RED_NO_TIME_PROGRESS and 
		// RED_TIME_PROGRESS.  
  int		flag_normality, 
  int		flag_action_approx, 
  int		flag_reduction, 
  int		flag_state_approx, 
  int		flag_symmetry,  
  int		flag_experiment,  
  char		*sxt, 
  ...
);  


  
  
extern redgram red_sync_xtion_string_fwd(
  redgram	dsrc, 
  redgram       dpath, 
  int		flag_game_roles,  
  int		flag_time_progress, 
		// Two argument values are 
		// RED_NO_TIME_PROGRESS and 
		// RED_TIME_PROGRESS.  
  int		flag_normality, 
  int		flag_action_approx, 
  int		flag_reduction, 
  int		flag_state_approx, 
  int		flag_symmetry,  
  int		flag_experiment, 
  char		*sxt, ...
); 


  
 
  
extern redgram red_sync_xtion_bck( 
	redgram  ddst, 
	redgram  dpath, 
	int 	flag_sync_xtion_table_choice,  
		// This argument is used to choose from the declared 
		// synchronous transition table and the 
		// game synchronous transition table. 
		// There are the following two values.  
		// RED_USE_GAME_SYNC_XTION
		// RED_USE_DECLARED_SYNC_XTION 
	int	sxi, // if sxi == red_query_sync_xtion_count(), 
		     // this is the sync bulk transition identifer.
	int	flag_game_roles, 
		// The argument consists of three flag values. 
		// Each flag may appear or not. 
		// The three flag values are 
		// RED_GAME_MODL, 
		// RED_GAME_SPEC, 
		// RED_GAME_ENVR. 
	int 	flag_time_progress, 
		// Two argument values are 
		// RED_NO_TIME_PROGRESS and 
		// RED_TIME_PROGRESS.  
	int	flag_normality, 
		// Some argument values are 
		// RED_NORM_ZONE_NONE, 
		// RED_NORM_ZONE_MAGNITUDE_REDUCED, and 
		// RED_NORM_ZONE_CLOSURE. 
	int	flag_action_approx, 
		// Some argument values are 
		// RED_NO_ACTION_APPROX, 
		// RED_ACTION_APPROX_NOXTIVE, and 
		// RED_ACTION_APPROX_UNTIMED.  
	int	flag_reduction, 
		// Two argument values are 
		// RED_NO_REDUCTION_INACTIVE and 
		// RED_REDUCTION_INACTIVE.  
	int	flag_state_approx, 
		// The argument values is the bit-wise 
		// of four flag values of the following form: 
		// fm | fs | fe | fg. 
		// fm is for abstraction of the model variables 
		// and can be of the following values:  
		// RED_NOAPPROX_MODL_GAME
		// RED_OAPPROX_MODL_GAME_DIAG_MAG
		// RED_OAPPROX_MODL_GAME_DIAGONAL
		// RED_OAPPROX_MODL_GAME_MAGNITUDE
		// RED_OAPPROX_MODL_GAME_UNTIMED
		// RED_OAPPROX_MODL_GAME_MODE_ONLY
		// RED_OAPPROX_MODL_GAME_NONE.  
		//
		// fs is for abstraction of the specification variables 
		// and can be of the following values: 
		// RED_NOAPPROX_SPEC_GAME
		// RED_OAPPROX_SPEC_GAME_DIAG_MAG
		// RED_OAPPROX_SPEC_GAME_DIAGONAL
		// RED_OAPPROX_SPEC_GAME_MAGNITUDE
		// RED_OAPPROX_SPEC_GAME_UNTIMED
		// RED_OAPPROX_SPEC_GAME_MODE_ONLY
		// RED_OAPPROX_SPEC_GAME_NONE
		//
		// fe is for abstraction of the environment variables 
		// and can be of the following values: 
		// RED_NOAPPROX_ENVR_GAME
		// RED_OAPPROX_ENVR_GAME_DIAG_MAG
		// RED_OAPPROX_ENVR_GAME_DIAGONAL
		// RED_OAPPROX_ENVR_GAME_MAGNITUDE
		// RED_OAPPROX_ENVR_GAME_UNTIMED
		// RED_OAPPROX_ENVR_GAME_MODE_ONLY
		// RED_OAPPROX_ENVR_GAME_NONE 
		//
		// fe is for abstraction of the global variables 
		// and can be of the following values: 
		// RED_NOAPPROX_GLOBAL_GAME
		// RED_OAPPROX_GLOBAL_GAME_DIAG_MAG
		// RED_OAPPROX_GLOBAL_GAME_DIAGONAL
		// RED_OAPPROX_GLOBAL_GAME_MAGNITUDE
		// RED_OAPPROX_GLOBAL_GAME_UNTIMED
		// RED_OAPPROX_GLOBAL_GAME_MODE_ONLY
		// RED_OAPPROX_GLOBAL_GAME_NONE  
	int	flag_symmetry,  
		// Some possible argument values are
		// RED_NO_SYMMETRY, 
		// RED_SYMMETRY_ZONE, 
		// RED_SYMMETRY_DISCRETE, 
		// RED_SYMMETRY_POINTER, 
		// RED_SYMMETRY_STATE. 
	int 	flag_experiment 
	); 
	
  
  
extern redgram red_sync_xtion_fwd( 
	redgram  dsrc, 
	redgram  dpath, 
	int 	flag_sync_xtion_table_choice,  
		// This argument is used to choose from the declared 
		// synchronous transition table and the 
		// game synchronous transition table. 
		// There are the following two values.  
		// RED_USE_GAME_SYNC_XTION
		// RED_USE_DECLARED_SYNC_XTION 
	int	sxi, // if sxi == red_query_sync_xtion_count(), 
		     // this is the sync bulk transition identifer.
	int	flag_game_roles,  
		// The argument consists of three flag values. 
		// Each flag may appear or not. 
		// The three flag values are 
		// RED_GAME_MODL, 
		// RED_GAME_SPEC, 
		// RED_GAME_ENVR. 
	int 	flag_time_progress, 
		// Two argument values are 
		// RED_NO_TIME_PROGRESS and 
		// RED_TIME_PROGRESS.  
	int	flag_normality, 
		// Some argument values are 
		// RED_NORM_ZONE_NONE, 
		// RED_NORM_ZONE_MAGNITUDE_REDUCED, and 
		// RED_NORM_ZONE_CLOSURE. 
	int	flag_action_approx, 
		// Some argument values are 
		// RED_NO_ACTION_APPROX, 
		// RED_ACTION_APPROX_NOXTIVE, and 
		// RED_ACTION_APPROX_UNTIMED.  
	int	flag_reduction, 
		// Two argument values are 
		// RED_NO_REDUCTION_INACTIVE and 
		// RED_REDUCTION_INACTIVE.  
	int	flag_state_approx, 
		// The argument values is the bit-wise 
		// of four flag values of the following form: 
		// fm | fs | fe | fg. 
		// fm is for abstraction of the model variables 
		// and can be of the following values:  
		// RED_NOAPPROX_MODL_GAME
		// RED_OAPPROX_MODL_GAME_DIAG_MAG
		// RED_OAPPROX_MODL_GAME_DIAGONAL
		// RED_OAPPROX_MODL_GAME_MAGNITUDE
		// RED_OAPPROX_MODL_GAME_UNTIMED
		// RED_OAPPROX_MODL_GAME_MODE_ONLY
		// RED_OAPPROX_MODL_GAME_NONE.  
		//
		// fs is for abstraction of the specification variables 
		// and can be of the following values: 
		// RED_NOAPPROX_SPEC_GAME
		// RED_OAPPROX_SPEC_GAME_DIAG_MAG
		// RED_OAPPROX_SPEC_GAME_DIAGONAL
		// RED_OAPPROX_SPEC_GAME_MAGNITUDE
		// RED_OAPPROX_SPEC_GAME_UNTIMED
		// RED_OAPPROX_SPEC_GAME_MODE_ONLY
		// RED_OAPPROX_SPEC_GAME_NONE
		//
		// fe is for abstraction of the environment variables 
		// and can be of the following values: 
		// RED_NOAPPROX_ENVR_GAME
		// RED_OAPPROX_ENVR_GAME_DIAG_MAG
		// RED_OAPPROX_ENVR_GAME_DIAGONAL
		// RED_OAPPROX_ENVR_GAME_MAGNITUDE
		// RED_OAPPROX_ENVR_GAME_UNTIMED
		// RED_OAPPROX_ENVR_GAME_MODE_ONLY
		// RED_OAPPROX_ENVR_GAME_NONE 
		//
		// fe is for abstraction of the global variables 
		// and can be of the following values: 
		// RED_NOAPPROX_GLOBAL_GAME
		// RED_OAPPROX_GLOBAL_GAME_DIAG_MAG
		// RED_OAPPROX_GLOBAL_GAME_DIAGONAL
		// RED_OAPPROX_GLOBAL_GAME_MAGNITUDE
		// RED_OAPPROX_GLOBAL_GAME_UNTIMED
		// RED_OAPPROX_GLOBAL_GAME_MODE_ONLY
		// RED_OAPPROX_GLOBAL_GAME_NONE  
	int	flag_symmetry,  
		// Some possible argument values are
		// RED_NO_SYMMETRY, 
		// RED_SYMMETRY_ZONE, 
		// RED_SYMMETRY_DISCRETE, 
		// RED_SYMMETRY_POINTER, 
		// RED_SYMMETRY_STATE. 
	int	flag_experiment 
	);
  
  




  
extern redgram red_sync_xtion_all_bck(
	redgram ddst, 
	redgram dpath, 
	int 	flag_sync_xtion_table_choice,  
		// This argument is used to choose from the declared 
		// synchronous transition table and the 
		// game synchronous transition table. 
		// There are the following two values.  
		// RED_USE_GAME_SYNC_XTION
		// RED_USE_DECLARED_SYNC_XTION 
	int	flag_game_roles, 
		// The argument consists of three flag values. 
		// Each flag may appear or not. 
		// The three flag values are 
		// RED_GAME_MODL, 
		// RED_GAME_SPEC, 
		// RED_GAME_ENVR. 
	int 	flag_time_progress, 
		// Two argument values are 
		// RED_NO_TIME_PROGRESS and 
		// RED_TIME_PROGRESS.  
	int	flag_normality, 
		// Some argument values are 
		// RED_NORM_ZONE_NONE, 
		// RED_NORM_ZONE_MAGNITUDE_REDUCED, and 
		// RED_NORM_ZONE_CLOSURE. 
	int	flag_action_approx, 
		// Some argument values are 
		// RED_NO_ACTION_APPROX, 
		// RED_ACTION_APPROX_NOXTIVE, and 
		// RED_ACTION_APPROX_UNTIMED.  
	int	flag_reduction, 
		// Two argument values are 
		// RED_NO_REDUCTION_INACTIVE and 
		// RED_REDUCTION_INACTIVE.  
	int	flag_state_approx, 
		// The argument values is the bit-wise 
		// of four flag values of the following form: 
		// fm | fs | fe | fg. 
		// fm is for abstraction of the model variables 
		// and can be of the following values:  
		// RED_NOAPPROX_MODL_GAME
		// RED_OAPPROX_MODL_GAME_DIAG_MAG
		// RED_OAPPROX_MODL_GAME_DIAGONAL
		// RED_OAPPROX_MODL_GAME_MAGNITUDE
		// RED_OAPPROX_MODL_GAME_UNTIMED
		// RED_OAPPROX_MODL_GAME_MODE_ONLY
		// RED_OAPPROX_MODL_GAME_NONE.  
		//
		// fs is for abstraction of the specification variables 
		// and can be of the following values: 
		// RED_NOAPPROX_SPEC_GAME
		// RED_OAPPROX_SPEC_GAME_DIAG_MAG
		// RED_OAPPROX_SPEC_GAME_DIAGONAL
		// RED_OAPPROX_SPEC_GAME_MAGNITUDE
		// RED_OAPPROX_SPEC_GAME_UNTIMED
		// RED_OAPPROX_SPEC_GAME_MODE_ONLY
		// RED_OAPPROX_SPEC_GAME_NONE
		//
		// fe is for abstraction of the environment variables 
		// and can be of the following values: 
		// RED_NOAPPROX_ENVR_GAME
		// RED_OAPPROX_ENVR_GAME_DIAG_MAG
		// RED_OAPPROX_ENVR_GAME_DIAGONAL
		// RED_OAPPROX_ENVR_GAME_MAGNITUDE
		// RED_OAPPROX_ENVR_GAME_UNTIMED
		// RED_OAPPROX_ENVR_GAME_MODE_ONLY
		// RED_OAPPROX_ENVR_GAME_NONE 
		//
		// fe is for abstraction of the global variables 
		// and can be of the following values: 
		// RED_NOAPPROX_GLOBAL_GAME
		// RED_OAPPROX_GLOBAL_GAME_DIAG_MAG
		// RED_OAPPROX_GLOBAL_GAME_DIAGONAL
		// RED_OAPPROX_GLOBAL_GAME_MAGNITUDE
		// RED_OAPPROX_GLOBAL_GAME_UNTIMED
		// RED_OAPPROX_GLOBAL_GAME_MODE_ONLY
		// RED_OAPPROX_GLOBAL_GAME_NONE  
	int	flag_symmetry,  
		// Some possible argument values are
		// RED_NO_SYMMETRY, 
		// RED_SYMMETRY_ZONE, 
		// RED_SYMMETRY_DISCRETE, 
		// RED_SYMMETRY_POINTER, 
		// RED_SYMMETRY_STATE. 
	int	flag_experiment 
	); 

 
  
  
extern redgram red_sync_xtion_all_fwd( 
	redgram	dsrc, 
	redgram dpath, 
	int 	flag_sync_xtion_table_choice,  
		// This argument is used to choose from the declared 
		// synchronous transition table and the 
		// game synchronous transition table. 
		// There are the following two values.  
		// RED_USE_GAME_SYNC_XTION
		// RED_USE_DECLARED_SYNC_XTION 
	int	flag_game_roles, 
		// The argument consists of three flag values. 
		// Each flag may appear or not. 
		// The three flag values are 
		// RED_GAME_MODL, 
		// RED_GAME_SPEC, 
		// RED_GAME_ENVR. 
	int 	flag_time_progress, 
		// Two argument values are 
		// RED_NO_TIME_PROGRESS and 
		// RED_TIME_PROGRESS.  
	int	flag_normality, 
		// Some argument values are 
		// RED_NORM_ZONE_NONE, 
		// RED_NORM_ZONE_MAGNITUDE_REDUCED, and 
		// RED_NORM_ZONE_CLOSURE. 
	int	flag_action_approx, 
		// Some argument values are 
		// RED_NO_ACTION_APPROX, 
		// RED_ACTION_APPROX_NOXTIVE, and 
		// RED_ACTION_APPROX_UNTIMED.  
	int	flag_reduction, 
		// Two argument values are 
		// RED_NO_REDUCTION_INACTIVE and 
		// RED_REDUCTION_INACTIVE.  
	int	flag_state_approx, 
		// The argument values is the bit-wise 
		// of four flag values of the following form: 
		// fm | fs | fe | fg. 
		// fm is for abstraction of the model variables 
		// and can be of the following values:  
		// RED_NOAPPROX_MODL_GAME
		// RED_OAPPROX_MODL_GAME_DIAG_MAG
		// RED_OAPPROX_MODL_GAME_DIAGONAL
		// RED_OAPPROX_MODL_GAME_MAGNITUDE
		// RED_OAPPROX_MODL_GAME_UNTIMED
		// RED_OAPPROX_MODL_GAME_MODE_ONLY
		// RED_OAPPROX_MODL_GAME_NONE.  
		//
		// fs is for abstraction of the specification variables 
		// and can be of the following values: 
		// RED_NOAPPROX_SPEC_GAME
		// RED_OAPPROX_SPEC_GAME_DIAG_MAG
		// RED_OAPPROX_SPEC_GAME_DIAGONAL
		// RED_OAPPROX_SPEC_GAME_MAGNITUDE
		// RED_OAPPROX_SPEC_GAME_UNTIMED
		// RED_OAPPROX_SPEC_GAME_MODE_ONLY
		// RED_OAPPROX_SPEC_GAME_NONE
		//
		// fe is for abstraction of the environment variables 
		// and can be of the following values: 
		// RED_NOAPPROX_ENVR_GAME
		// RED_OAPPROX_ENVR_GAME_DIAG_MAG
		// RED_OAPPROX_ENVR_GAME_DIAGONAL
		// RED_OAPPROX_ENVR_GAME_MAGNITUDE
		// RED_OAPPROX_ENVR_GAME_UNTIMED
		// RED_OAPPROX_ENVR_GAME_MODE_ONLY
		// RED_OAPPROX_ENVR_GAME_NONE 
		//
		// fe is for abstraction of the global variables 
		// and can be of the following values: 
		// RED_NOAPPROX_GLOBAL_GAME
		// RED_OAPPROX_GLOBAL_GAME_DIAG_MAG
		// RED_OAPPROX_GLOBAL_GAME_DIAGONAL
		// RED_OAPPROX_GLOBAL_GAME_MAGNITUDE
		// RED_OAPPROX_GLOBAL_GAME_UNTIMED
		// RED_OAPPROX_GLOBAL_GAME_MODE_ONLY
		// RED_OAPPROX_GLOBAL_GAME_NONE  
	int	flag_symmetry,  
		// Some possible argument values are
		// RED_NO_SYMMETRY, 
		// RED_SYMMETRY_ZONE, 
		// RED_SYMMETRY_DISCRETE, 
		// RED_SYMMETRY_POINTER, 
		// RED_SYMMETRY_STATE. 
	int	flag_experiment 
	); 


  

extern redgram red_sync_xtion_event_bck(
	redgram	devents, // If it is NULL, then it is treated as red_true(). 
	redgram ddst, 
	redgram dpath, 
	int 	flag_sync_xtion_table_choice,  
		// This argument is used to choose from the declared 
		// synchronous transition table and the 
		// game synchronous transition table. 
		// There are the following two values.  
		// RED_USE_GAME_SYNC_XTION
		// RED_USE_DECLARED_SYNC_XTION 
	int	flag_game_roles, 
		// The argument consists of three flag values. 
		// Each flag may appear or not. 
		// The three flag values are 
		// RED_GAME_MODL, 
		// RED_GAME_SPEC, 
		// RED_GAME_ENVR. 
	int 	flag_time_progress, 
		// Two argument values are 
		// RED_NO_TIME_PROGRESS and 
		// RED_TIME_PROGRESS.  
	int	flag_normality, 
		// Some argument values are 
		// RED_NORM_ZONE_NONE, 
		// RED_NORM_ZONE_MAGNITUDE_REDUCED, and 
		// RED_NORM_ZONE_CLOSURE. 
	int	flag_action_approx, 
		// Some argument values are 
		// RED_NO_ACTION_APPROX, 
		// RED_ACTION_APPROX_NOXTIVE, and 
		// RED_ACTION_APPROX_UNTIMED.  
	int	flag_reduction, 
		// Two argument values are 
		// RED_NO_REDUCTION_INACTIVE and 
		// RED_REDUCTION_INACTIVE.  
	int	flag_state_approx, 
		// The argument values is the bit-wise 
		// of four flag values of the following form: 
		// fm | fs | fe | fg. 
		// fm is for abstraction of the model variables 
		// and can be of the following values:  
		// RED_NOAPPROX_MODL_GAME
		// RED_OAPPROX_MODL_GAME_DIAG_MAG
		// RED_OAPPROX_MODL_GAME_DIAGONAL
		// RED_OAPPROX_MODL_GAME_MAGNITUDE
		// RED_OAPPROX_MODL_GAME_UNTIMED
		// RED_OAPPROX_MODL_GAME_MODE_ONLY
		// RED_OAPPROX_MODL_GAME_NONE.  
		//
		// fs is for abstraction of the specification variables 
		// and can be of the following values: 
		// RED_NOAPPROX_SPEC_GAME
		// RED_OAPPROX_SPEC_GAME_DIAG_MAG
		// RED_OAPPROX_SPEC_GAME_DIAGONAL
		// RED_OAPPROX_SPEC_GAME_MAGNITUDE
		// RED_OAPPROX_SPEC_GAME_UNTIMED
		// RED_OAPPROX_SPEC_GAME_MODE_ONLY
		// RED_OAPPROX_SPEC_GAME_NONE
		//
		// fe is for abstraction of the environment variables 
		// and can be of the following values: 
		// RED_NOAPPROX_ENVR_GAME
		// RED_OAPPROX_ENVR_GAME_DIAG_MAG
		// RED_OAPPROX_ENVR_GAME_DIAGONAL
		// RED_OAPPROX_ENVR_GAME_MAGNITUDE
		// RED_OAPPROX_ENVR_GAME_UNTIMED
		// RED_OAPPROX_ENVR_GAME_MODE_ONLY
		// RED_OAPPROX_ENVR_GAME_NONE 
		//
		// fe is for abstraction of the global variables 
		// and can be of the following values: 
		// RED_NOAPPROX_GLOBAL_GAME
		// RED_OAPPROX_GLOBAL_GAME_DIAG_MAG
		// RED_OAPPROX_GLOBAL_GAME_DIAGONAL
		// RED_OAPPROX_GLOBAL_GAME_MAGNITUDE
		// RED_OAPPROX_GLOBAL_GAME_UNTIMED
		// RED_OAPPROX_GLOBAL_GAME_MODE_ONLY
		// RED_OAPPROX_GLOBAL_GAME_NONE  
	int	flag_symmetry,  
		// Some possible argument values are
		// RED_NO_SYMMETRY, 
		// RED_SYMMETRY_ZONE, 
		// RED_SYMMETRY_DISCRETE, 
		// RED_SYMMETRY_POINTER, 
		// RED_SYMMETRY_STATE. 
	int	flag_experiment 
	); 

 
  
  
extern redgram red_sync_xtion_event_string_bck(
	char	*str_events, // If it is NULL, then it is treated as "true".
	redgram ddst, 
	redgram dpath, 
	int 	flag_sync_xtion_table_choice,  
		// This argument is used to choose from the declared 
		// synchronous transition table and the 
		// game synchronous transition table. 
		// There are the following two values.  
		// RED_USE_GAME_SYNC_XTION
		// RED_USE_DECLARED_SYNC_XTION 
	int	flag_game_roles, 
		// The argument consists of three flag values. 
		// Each flag may appear or not. 
		// The three flag values are 
		// RED_GAME_MODL, 
		// RED_GAME_SPEC, 
		// RED_GAME_ENVR. 
	int 	flag_time_progress, 
		// Two argument values are 
		// RED_NO_TIME_PROGRESS and 
		// RED_TIME_PROGRESS.  
	int	flag_normality, 
		// Some argument values are 
		// RED_NORM_ZONE_NONE, 
		// RED_NORM_ZONE_MAGNITUDE_REDUCED, and 
		// RED_NORM_ZONE_CLOSURE. 
	int	flag_action_approx, 
		// Some argument values are 
		// RED_NO_ACTION_APPROX, 
		// RED_ACTION_APPROX_NOXTIVE, and 
		// RED_ACTION_APPROX_UNTIMED.  
	int	flag_reduction, 
		// Two argument values are 
		// RED_NO_REDUCTION_INACTIVE and 
		// RED_REDUCTION_INACTIVE.  
	int	flag_state_approx, 
		// The argument values is the bit-wise 
		// of four flag values of the following form: 
		// fm | fs | fe | fg. 
		// fm is for abstraction of the model variables 
		// and can be of the following values:  
		// RED_NOAPPROX_MODL_GAME
		// RED_OAPPROX_MODL_GAME_DIAG_MAG
		// RED_OAPPROX_MODL_GAME_DIAGONAL
		// RED_OAPPROX_MODL_GAME_MAGNITUDE
		// RED_OAPPROX_MODL_GAME_UNTIMED
		// RED_OAPPROX_MODL_GAME_MODE_ONLY
		// RED_OAPPROX_MODL_GAME_NONE.  
		//
		// fs is for abstraction of the specification variables 
		// and can be of the following values: 
		// RED_NOAPPROX_SPEC_GAME
		// RED_OAPPROX_SPEC_GAME_DIAG_MAG
		// RED_OAPPROX_SPEC_GAME_DIAGONAL
		// RED_OAPPROX_SPEC_GAME_MAGNITUDE
		// RED_OAPPROX_SPEC_GAME_UNTIMED
		// RED_OAPPROX_SPEC_GAME_MODE_ONLY
		// RED_OAPPROX_SPEC_GAME_NONE
		//
		// fe is for abstraction of the environment variables 
		// and can be of the following values: 
		// RED_NOAPPROX_ENVR_GAME
		// RED_OAPPROX_ENVR_GAME_DIAG_MAG
		// RED_OAPPROX_ENVR_GAME_DIAGONAL
		// RED_OAPPROX_ENVR_GAME_MAGNITUDE
		// RED_OAPPROX_ENVR_GAME_UNTIMED
		// RED_OAPPROX_ENVR_GAME_MODE_ONLY
		// RED_OAPPROX_ENVR_GAME_NONE 
		//
		// fe is for abstraction of the global variables 
		// and can be of the following values: 
		// RED_NOAPPROX_GLOBAL_GAME
		// RED_OAPPROX_GLOBAL_GAME_DIAG_MAG
		// RED_OAPPROX_GLOBAL_GAME_DIAGONAL
		// RED_OAPPROX_GLOBAL_GAME_MAGNITUDE
		// RED_OAPPROX_GLOBAL_GAME_UNTIMED
		// RED_OAPPROX_GLOBAL_GAME_MODE_ONLY
		// RED_OAPPROX_GLOBAL_GAME_NONE  
	int	flag_symmetry,  
		// Some possible argument values are
		// RED_NO_SYMMETRY, 
		// RED_SYMMETRY_ZONE, 
		// RED_SYMMETRY_DISCRETE, 
		// RED_SYMMETRY_POINTER, 
		// RED_SYMMETRY_STATE. 
	int	flag_experiment 
	); 

 
  
  

extern void	red_print_reachable_return(
  struct reachable_return_type	*rr 
); 

	
extern struct reachable_return_type 
  *red_reach_bck(
	redgram	dinit, // redgram for the initial condition. 
	redgram	dpath, // redgram for the path condition. 
	redgram	dgoal, // redgram for the goal condition 
	int 	flag_task, 
		// The argument may assume the following values: 
		// RED_TASK_SAFETY, 
		// RED_TASK_RISK, 
		// RED_TASK_GOAL, 
		// RED_TASK_ZENO, 
		// RED_TASK_DEADLOCK, 
		// RED_TASK_MODEL_CHECK, 
		// RED_TASK_BRANCH_SIM_CHECK, 
		// RED_TASK_BRANCH_BISIM_CHECK
	int	flag_parametric_analysis, 
		// The possible argument values are 
		// RED_PARAMETRIC_ANALYSIS and 
		// RED_NO_PARAMETRIC_ANALYSIS. 
	int	flag_game_roles, 
		// The argument consists of three flag values. 
		// Each flag may appear or not. 
		// The three flag values are 
		// RED_GAME_MODL, 
		// RED_GAME_SPEC, 
		// RED_GAME_ENVR. 
	int	flag_full_reachability, 
		// Two argument values are
		// RED_FULL_REACHABILITY and 
		// RED_NO_FULL_REACHABILITY.
	int	flag_reachability_depth_bound, 
		// -1 means no bound. 
	int	flag_counter_example, 
		// Two argument values are 
		// RED_COUNTER_EXAMPLE and 
		// RED_NO_COUNTER_EXAMPLE.  
	int 	flag_time_progress, 
		// Two argument values are 
		// RED_NO_TIME_PROGRESS and 
		// RED_TIME_PROGRESS.  
	int	flag_normality, 
		// Some argument values are 
		// RED_NORM_ZONE_NONE, 
		// RED_NORM_ZONE_MAGNITUDE_REDUCED, and 
		// RED_NORM_ZONE_CLOSURE. 
	int	flag_action_approx, 
		// Some argument values are 
		// RED_NO_ACTION_APPROX, 
		// RED_ACTION_APPROX_NOXTIVE, and 
		// RED_ACTION_APPROX_UNTIMED.  
	int	flag_reduction, 
		// Two argument values are 
		// RED_NO_REDUCTION_INACTIVE and 
		// RED_REDUCTION_INACTIVE.  
	int	flag_state_approx, 
		// The argument values is the bit-wise 
		// of four flag values of the following form: 
		// fm | fs | fe | fg. 
		// fm is for abstraction of the model variables 
		// and can be of the following values:  
		// RED_NOAPPROX_MODL_GAME
		// RED_OAPPROX_MODL_GAME_DIAG_MAG
		// RED_OAPPROX_MODL_GAME_DIAGONAL
		// RED_OAPPROX_MODL_GAME_MAGNITUDE
		// RED_OAPPROX_MODL_GAME_UNTIMED
		// RED_OAPPROX_MODL_GAME_MODE_ONLY
		// RED_OAPPROX_MODL_GAME_NONE.  
		//
		// fs is for abstraction of the specification variables 
		// and can be of the following values: 
		// RED_NOAPPROX_SPEC_GAME
		// RED_OAPPROX_SPEC_GAME_DIAG_MAG
		// RED_OAPPROX_SPEC_GAME_DIAGONAL
		// RED_OAPPROX_SPEC_GAME_MAGNITUDE
		// RED_OAPPROX_SPEC_GAME_UNTIMED
		// RED_OAPPROX_SPEC_GAME_MODE_ONLY
		// RED_OAPPROX_SPEC_GAME_NONE
		//
		// fe is for abstraction of the environment variables 
		// and can be of the following values: 
		// RED_NOAPPROX_ENVR_GAME
		// RED_OAPPROX_ENVR_GAME_DIAG_MAG
		// RED_OAPPROX_ENVR_GAME_DIAGONAL
		// RED_OAPPROX_ENVR_GAME_MAGNITUDE
		// RED_OAPPROX_ENVR_GAME_UNTIMED
		// RED_OAPPROX_ENVR_GAME_MODE_ONLY
		// RED_OAPPROX_ENVR_GAME_NONE 
		//
		// fe is for abstraction of the global variables 
		// and can be of the following values: 
		// RED_NOAPPROX_GLOBAL_GAME
		// RED_OAPPROX_GLOBAL_GAME_DIAG_MAG
		// RED_OAPPROX_GLOBAL_GAME_DIAGONAL
		// RED_OAPPROX_GLOBAL_GAME_MAGNITUDE
		// RED_OAPPROX_GLOBAL_GAME_UNTIMED
		// RED_OAPPROX_GLOBAL_GAME_MODE_ONLY
		// RED_OAPPROX_GLOBAL_GAME_NONE  
	int	flag_symmetry, 
		// Some possible argument values are
		// RED_NO_SYMMETRY, 
		// RED_SYMMETRY_ZONE, 
		// RED_SYMMETRY_DISCRETE, 
		// RED_SYMMETRY_POINTER, 
		// RED_SYMMETRY_STATE. 
	int	flag_experiment, 
	int	flag_print 
		// Two argument values are 
		// RED_NO_PRINT and 
		// RED_PRINT. 
	); 

extern struct reachable_return_type 
  *red_reach_fwd( 
	redgram  dinit, // redgram for the initial condition. 
	redgram  dpath, // redgram for the path condition. 
	redgram  dgoal, // redgram for the goal condition 
/*4*/	int 	flag_task, 
		// The argument may assume the following values: 
		// RED_TASK_SAFETY, 
		// RED_TASK_RISK, 
		// RED_TASK_GOAL, 
		// RED_TASK_ZENO, 
		// RED_TASK_DEADLOCK, 
		// RED_TASK_MODEL_CHECK, 
		// RED_TASK_BRANCH_SIM_CHECK, 
		// RED_TASK_BRANCH_BISIM_CHECK
/*5*/	int	flag_parametric_analysis, 
		// The possible argument values are 
		// RED_PARAMETRIC_ANALYSIS and 
		// RED_NO_PARAMETRIC_ANALYSIS. 
/*6*/	int	flag_game_roles, 
		// The argument consists of three flag values. 
		// Each flag may appear or not. 
		// The three flag values are 
		// RED_GAME_MODL, 
		// RED_GAME_SPEC, 
		// RED_GAME_ENVR. 
/*7*/	int	flag_full_reachability, 
		// Two argument values are
		// RED_FULL_REACHABILITY and 
		// RED_NO_FULL_REACHABILITY.
/*8*/	int	flag_reachability_depth_bound, 
		// -1 means no bound. 
/*9*/	int	flag_counter_example, 
		// Two argument values are 
		// RED_COUNTER_EXAMPLE and 
		// RED_NO_COUNTER_EXAMPLE.  
/*10*/	int 	flag_time_progress, 
		// Two argument values are 
		// RED_NO_TIME_PROGRESS and 
		// RED_TIME_PROGRESS.  
/*11*/	int	flag_normality, 
		// Some argument values are 
		// RED_NORM_ZONE_NONE, 
		// RED_NORM_ZONE_MAGNITUDE_REDUCED, and 
		// RED_NORM_ZONE_CLOSURE. 
/*12*/	int	flag_action_approx, 
		// Some argument values are 
		// RED_NO_ACTION_APPROX, 
		// RED_ACTION_APPROX_NOXTIVE, and 
		// RED_ACTION_APPROX_UNTIMED.  
/*13*/	int	flag_reduction, 
		// Two argument values are 
		// RED_NO_REDUCTION_INACTIVE and 
		// RED_REDUCTION_INACTIVE.  
/*14*/	int	flag_state_approx, 
		// The argument values is the bit-wise 
		// of four flag values of the following form: 
		// fm | fs | fe | fg. 
		// fm is for abstraction of the model variables 
		// and can be of the following values:  
		// RED_NOAPPROX_MODL_GAME
		// RED_OAPPROX_MODL_GAME_DIAG_MAG
		// RED_OAPPROX_MODL_GAME_DIAGONAL
		// RED_OAPPROX_MODL_GAME_MAGNITUDE
		// RED_OAPPROX_MODL_GAME_UNTIMED
		// RED_OAPPROX_MODL_GAME_MODE_ONLY
		// RED_OAPPROX_MODL_GAME_NONE.  
		//
		// fs is for abstraction of the specification variables 
		// and can be of the following values: 
		// RED_NOAPPROX_SPEC_GAME
		// RED_OAPPROX_SPEC_GAME_DIAG_MAG
		// RED_OAPPROX_SPEC_GAME_DIAGONAL
		// RED_OAPPROX_SPEC_GAME_MAGNITUDE
		// RED_OAPPROX_SPEC_GAME_UNTIMED
		// RED_OAPPROX_SPEC_GAME_MODE_ONLY
		// RED_OAPPROX_SPEC_GAME_NONE
		//
		// fe is for abstraction of the environment variables 
		// and can be of the following values: 
		// RED_NOAPPROX_ENVR_GAME
		// RED_OAPPROX_ENVR_GAME_DIAG_MAG
		// RED_OAPPROX_ENVR_GAME_DIAGONAL
		// RED_OAPPROX_ENVR_GAME_MAGNITUDE
		// RED_OAPPROX_ENVR_GAME_UNTIMED
		// RED_OAPPROX_ENVR_GAME_MODE_ONLY
		// RED_OAPPROX_ENVR_GAME_NONE 
		//
		// fe is for abstraction of the global variables 
		// and can be of the following values: 
		// RED_NOAPPROX_GLOBAL_GAME
		// RED_OAPPROX_GLOBAL_GAME_DIAG_MAG
		// RED_OAPPROX_GLOBAL_GAME_DIAGONAL
		// RED_OAPPROX_GLOBAL_GAME_MAGNITUDE
		// RED_OAPPROX_GLOBAL_GAME_UNTIMED
		// RED_OAPPROX_GLOBAL_GAME_MODE_ONLY
		// RED_OAPPROX_GLOBAL_GAME_NONE  
/*15*/	int	flag_symmetry, 
		// Some possible argument values are
		// RED_NO_SYMMETRY, 
		// RED_SYMMETRY_ZONE, 
		// RED_SYMMETRY_DISCRETE, 
		// RED_SYMMETRY_POINTER, 
		// RED_SYMMETRY_STATE. 
	int	flag_experiment, 
/*16*/	int	flag_print 
		// Two argument values are 
		// RED_NO_PRINT and 
		// RED_PRINT. 
	);
  
extern struct model_check_return_type	*red_model_check(
	redgram	dinit, 
	redgram dpath, 
 	int	flag_normality, 
		// Some argument values are 
		// RED_NORM_ZONE_NONE, 
		// RED_NORM_ZONE_MAGNITUDE_REDUCED, and 
		// RED_NORM_ZONE_CLOSURE. 
	int	flag_action_approx, 
		// Some argument values are 
		// RED_NO_ACTION_APPROX, 
		// RED_ACTION_APPROX_NOXTIVE, and 
		// RED_ACTION_APPROX_UNTIMED.  
	int	flag_reduction, 
		// Two argument values are 
		// RED_NO_REDUCTION_INACTIVE and 
		// RED_REDUCTION_INACTIVE.  
	int	flag_state_approx, 
		// The argument values is the bit-wise 
		// of four flag values of the following form: 
		// fm | fs | fe | fg. 
		// fm is for abstraction of the model variables 
		// and can be of the following values:  
		// RED_NOAPPROX_MODL_GAME
		// RED_OAPPROX_MODL_GAME_DIAG_MAG
		// RED_OAPPROX_MODL_GAME_DIAGONAL
		// RED_OAPPROX_MODL_GAME_MAGNITUDE
		// RED_OAPPROX_MODL_GAME_UNTIMED
		// RED_OAPPROX_MODL_GAME_MODE_ONLY
		// RED_OAPPROX_MODL_GAME_NONE.  
		//
		// fs is for abstraction of the specification variables 
		// and can be of the following values: 
		// RED_NOAPPROX_SPEC_GAME
		// RED_OAPPROX_SPEC_GAME_DIAG_MAG
		// RED_OAPPROX_SPEC_GAME_DIAGONAL
		// RED_OAPPROX_SPEC_GAME_MAGNITUDE
		// RED_OAPPROX_SPEC_GAME_UNTIMED
		// RED_OAPPROX_SPEC_GAME_MODE_ONLY
		// RED_OAPPROX_SPEC_GAME_NONE
		//
		// fe is for abstraction of the environment variables 
		// and can be of the following values: 
		// RED_NOAPPROX_ENVR_GAME
		// RED_OAPPROX_ENVR_GAME_DIAG_MAG
		// RED_OAPPROX_ENVR_GAME_DIAGONAL
		// RED_OAPPROX_ENVR_GAME_MAGNITUDE
		// RED_OAPPROX_ENVR_GAME_UNTIMED
		// RED_OAPPROX_ENVR_GAME_MODE_ONLY
		// RED_OAPPROX_ENVR_GAME_NONE 
		//
		// fe is for abstraction of the global variables 
		// and can be of the following values: 
		// RED_NOAPPROX_GLOBAL_GAME
		// RED_OAPPROX_GLOBAL_GAME_DIAG_MAG
		// RED_OAPPROX_GLOBAL_GAME_DIAGONAL
		// RED_OAPPROX_GLOBAL_GAME_MAGNITUDE
		// RED_OAPPROX_GLOBAL_GAME_UNTIMED
		// RED_OAPPROX_GLOBAL_GAME_MODE_ONLY
		// RED_OAPPROX_GLOBAL_GAME_NONE  
	int 	flag_zeno, 
		// Some argument values are 
		// RED_PLAIN_NONZENO, 
		// RED_APPROX_NONZENO, and 
		// RED_ZENO_TRACES_OK.   
	int	flag_experiment, 
	int	flag_print,  
		// Two argument values are 
		// RED_NO_PRINT and 
		// RED_PRINT. 
	char	*f, 
		// f is a string for a TCTL formula. 
	... 
	);
  

extern void	red_print_sim_check_return(
  struct sim_check_return_type	*sr
); 


extern struct sim_check_return_type	*red_sim_check(
/*1*/	redgram dinit, 
/*2*/	redgram dinit_sim, 
/*3*/	int	flag_complete_greatest_fixpoint, 
		// Two argument values are
		// RED_NO_COMPLETE_GREATEST_FIXPOINT and
		// RED_COMPLETE_GREATEST_FIXPOINT.  
/*4*/	int	flag_fixpoint_iteration_bound, 
		// -1 means no bound. 
/*5*/	int	flag_counter_example, 
		// Two argument values are 
		// RED_COUNTER_EXAMPLE and 
		// RED_NO_COUNTER_EXAMPLE.  
/*6*/	int 	flag_time_progress, 
		// Two argument values are 
		// RED_NO_TIME_PROGRESS and 
		// RED_TIME_PROGRESS.  
/*7*/	int	flag_normality, 
		// Some argument values are 
		// RED_NORM_ZONE_NONE, 
		// RED_NORM_ZONE_MAGNITUDE_REDUCED, and 
		// RED_NORM_ZONE_CLOSURE. 
		// However, at this moment, this argument is 
		// neglected since REDLIB always use the closure form 
		// in simulation checking.  
		// We keep this argument for future research. 
/*8*/	int	flag_action_approx, 
		// Some argument values are 
		// RED_NO_ACTION_APPROX, 
		// RED_ACTION_APPROX_NOXTIVE, and 
		// RED_ACTION_APPROX_UNTIMED.  
/*9*/	int	flag_reduction, 
		// Two argument values are 
		// RED_NO_REDUCTION_INACTIVE and 
		// RED_REDUCTION_INACTIVE.  
/*10*/	int	flag_state_approx, 
		// The argument values is the bit-wise 
		// of four flag values of the following form: 
		// fm | fs | fe | fg. 
		// fm is for abstraction of the model variables 
		// and can be of the following values:  
		// RED_NOAPPROX_MODL_GAME
		// RED_OAPPROX_MODL_GAME_DIAG_MAG
		// RED_OAPPROX_MODL_GAME_DIAGONAL
		// RED_OAPPROX_MODL_GAME_MAGNITUDE
		// RED_OAPPROX_MODL_GAME_UNTIMED
		// RED_OAPPROX_MODL_GAME_MODE_ONLY
		// RED_OAPPROX_MODL_GAME_NONE.  
		//
		// fs is for abstraction of the specification variables 
		// and can be of the following values: 
		// RED_NOAPPROX_SPEC_GAME
		// RED_OAPPROX_SPEC_GAME_DIAG_MAG
		// RED_OAPPROX_SPEC_GAME_DIAGONAL
		// RED_OAPPROX_SPEC_GAME_MAGNITUDE
		// RED_OAPPROX_SPEC_GAME_UNTIMED
		// RED_OAPPROX_SPEC_GAME_MODE_ONLY
		// RED_OAPPROX_SPEC_GAME_NONE
		//
		// fe is for abstraction of the environment variables 
		// and can be of the following values: 
		// RED_NOAPPROX_ENVR_GAME
		// RED_OAPPROX_ENVR_GAME_DIAG_MAG
		// RED_OAPPROX_ENVR_GAME_DIAGONAL
		// RED_OAPPROX_ENVR_GAME_MAGNITUDE
		// RED_OAPPROX_ENVR_GAME_UNTIMED
		// RED_OAPPROX_ENVR_GAME_MODE_ONLY
		// RED_OAPPROX_ENVR_GAME_NONE 
		//
		// fe is for abstraction of the global variables 
		// and can be of the following values: 
		// RED_NOAPPROX_GLOBAL_GAME
		// RED_OAPPROX_GLOBAL_GAME_DIAG_MAG
		// RED_OAPPROX_GLOBAL_GAME_DIAGONAL
		// RED_OAPPROX_GLOBAL_GAME_MAGNITUDE
		// RED_OAPPROX_GLOBAL_GAME_UNTIMED
		// RED_OAPPROX_GLOBAL_GAME_MODE_ONLY
		// RED_OAPPROX_GLOBAL_GAME_NONE  
/*11*/	int	flag_symmetry, 
		// Some argument values are 
		// RED_NO_SYMMETRY, 
		// RED_SYMMETRY_ZONE, 
		// RED_SYMMETRY_DISCRETE, 
		// RED_SYMMETRY_POINTER, and 
		// RED_SYMMETRY_STATE
/*12*/	int	flag_zeno, 
		// Some argument values are 
		// RED_PLAIN_NONZENO, 
		// RED_APPROX_NONZENO, and 
		// RED_ZENO_TRACES_OK.   
	int	flag_experiment, 
/*13*/	int	flag_print, 
		// Two argument values are 
		// RED_NO_PRINT and 
		// RED_PRINT. 
/*14*/	char	*role_spec, 
		// This string is of the form 
		//   "m1, m2, ..., mk; s1, s2, ..., sj;"
		// where m1, m2, ..., mk is a sequence of process indices 
		// for the model automata and 
		// s1, s2, ..., sj is a sequence of process indices 
		// for the specification automata. 
		// All the other process indices are for the environment
		// automata. 
/*15*/	... 
	);
  
  
  
extern struct sim_check_return_type	*red_bisim_check(
/*1*/	redgram dinit, 
/*2*/	redgram dinit_sim, 
/*3*/	int	flag_complete_greatest_fixpoint, 
		// Two argument values are
		// RED_NO_COMPLETE_GREATEST_FIXPOINT and
		// RED_COMPLETE_GREATEST_FIXPOINT.  
/*4*/	int	flag_fixpoint_iteration_bound, 
		// -1 means no bound. 
/*5*/	int	flag_counter_example, 
		// Two argument values are 
		// RED_COUNTER_EXAMPLE and 
		// RED_NO_COUNTER_EXAMPLE.  
/*6*/	int 	flag_time_progress, 
		// Two argument values are 
		// RED_NO_TIME_PROGRESS and 
		// RED_TIME_PROGRESS.  
/*7*/	int	flag_normality, 
		// Some argument values are 
		// RED_NORM_ZONE_NONE, 
		// RED_NORM_ZONE_MAGNITUDE_REDUCED, and 
		// RED_NORM_ZONE_CLOSURE. 
		// However, at this moment, this argument is 
		// neglected since REDLIB always use the closure form 
		// in simulation checking.  
		// We keep this argument for future research. 
/*8*/	int	flag_action_approx, 
		// Some argument values are 
		// RED_NO_ACTION_APPROX, 
		// RED_ACTION_APPROX_NOXTIVE, and 
		// RED_ACTION_APPROX_UNTIMED.  
/*9*/	int	flag_reduction, 
		// Two argument values are 
		// RED_NO_REDUCTION_INACTIVE and 
		// RED_REDUCTION_INACTIVE.  
/*10*/	int	flag_state_approx, 
		// The argument values is the bit-wise 
		// of four flag values of the following form: 
		// fm | fs | fe | fg. 
		// fm is for abstraction of the model variables 
		// and can be of the following values:  
		// RED_NOAPPROX_MODL_GAME
		// RED_OAPPROX_MODL_GAME_DIAG_MAG
		// RED_OAPPROX_MODL_GAME_DIAGONAL
		// RED_OAPPROX_MODL_GAME_MAGNITUDE
		// RED_OAPPROX_MODL_GAME_UNTIMED
		// RED_OAPPROX_MODL_GAME_MODE_ONLY
		// RED_OAPPROX_MODL_GAME_NONE.  
		//
		// fs is for abstraction of the specification variables 
		// and can be of the following values: 
		// RED_NOAPPROX_SPEC_GAME
		// RED_OAPPROX_SPEC_GAME_DIAG_MAG
		// RED_OAPPROX_SPEC_GAME_DIAGONAL
		// RED_OAPPROX_SPEC_GAME_MAGNITUDE
		// RED_OAPPROX_SPEC_GAME_UNTIMED
		// RED_OAPPROX_SPEC_GAME_MODE_ONLY
		// RED_OAPPROX_SPEC_GAME_NONE
		//
		// fe is for abstraction of the environment variables 
		// and can be of the following values: 
		// RED_NOAPPROX_ENVR_GAME
		// RED_OAPPROX_ENVR_GAME_DIAG_MAG
		// RED_OAPPROX_ENVR_GAME_DIAGONAL
		// RED_OAPPROX_ENVR_GAME_MAGNITUDE
		// RED_OAPPROX_ENVR_GAME_UNTIMED
		// RED_OAPPROX_ENVR_GAME_MODE_ONLY
		// RED_OAPPROX_ENVR_GAME_NONE 
		//
		// fe is for abstraction of the global variables 
		// and can be of the following values: 
		// RED_NOAPPROX_GLOBAL_GAME
		// RED_OAPPROX_GLOBAL_GAME_DIAG_MAG
		// RED_OAPPROX_GLOBAL_GAME_DIAGONAL
		// RED_OAPPROX_GLOBAL_GAME_MAGNITUDE
		// RED_OAPPROX_GLOBAL_GAME_UNTIMED
		// RED_OAPPROX_GLOBAL_GAME_MODE_ONLY
		// RED_OAPPROX_GLOBAL_GAME_NONE  
/*11*/	int	flag_symmetry, 
		// Some argument values are 
		// RED_NO_SYMMETRY, 
		// RED_SYMMETRY_ZONE, 
		// RED_SYMMETRY_DISCRETE, 
		// RED_SYMMETRY_POINTER, and 
		// RED_SYMMETRY_STATE
/*12*/	int	flag_zeno, 
		// Some argument values are 
		// RED_PLAIN_NONZENO, 
		// RED_APPROX_NONZENO, and 
		// RED_ZENO_TRACES_OK.   
/*13*/	int	flag_experiment, 
/*14*/	int	flag_print, 
		// Two argument values are 
		// RED_NO_PRINT and 
		// RED_PRINT. 
/*15*/	char	*role_spec, 
		// This string is of the form 
		//   "m1, m2, ..., mk; s1, s2, ..., sj;"
		// where m1, m2, ..., mk is a sequence of process indices 
		// for the model automata and 
		// s1, s2, ..., sj is a sequence of process indices 
		// for the specification automata. 
		// All the other process indices are for the environment
		// automata. 
/*16*/	... 
	);
  
  
  
extern void 
  red_input_roles(
	char	*role_spec, 
		// This string is of the form 
		//   "m1, m2, ..., mk; s1, s2, ..., sj;"
		// where m1, m2, ..., mk is a sequence of process indices 
		// for the model automata and 
		// s1, s2, ..., sj is a sequence of process indices 
		// for the specification automata. 
		// All the other process indices are for the environment
		// automata. 
	... 
  ); 

extern char
  *red_query_role_string(); 

extern int
  red_query_process_game_role(int pi); 

extern double  
  red_query_system_time(), 
  red_query_user_time(); 
  
extern void red_print_time(); 

extern int
  red_query_memory(); 
  
  
extern redgram  
  red_query_diagram_initial(), 
  red_query_diagram_spec_risk(), 
  red_query_diagram_enhanced_global_invariance(), 
  red_query_diagram_global_invariance(), 
  red_query_diagram_deadlock(), 
  red_query_diagram_zeno(); 
  
extern char	
  *red_query_string_initial(); 

extern redgram red_norm(
	redgram  d, 
	int	op
	);
  
  
extern redgram red_abstract(
	redgram	d, 
	int	flag_state_oapprox, 
		// The argument values is the bit-wise 
		// of four flag values of the following form: 
		// fm | fs | fe | fg. 
		// fm is for abstraction of the model variables 
		// and can be of the following values:  
		// RED_NOAPPROX_MODL_GAME
		// RED_OAPPROX_MODL_GAME_DIAG_MAG
		// RED_OAPPROX_MODL_GAME_DIAGONAL
		// RED_OAPPROX_MODL_GAME_MAGNITUDE
		// RED_OAPPROX_MODL_GAME_UNTIMED
		// RED_OAPPROX_MODL_GAME_MODE_ONLY
		// RED_OAPPROX_MODL_GAME_NONE.  
		//
		// fs is for abstraction of the specification variables 
		// and can be of the following values: 
		// RED_NOAPPROX_SPEC_GAME
		// RED_OAPPROX_SPEC_GAME_DIAG_MAG
		// RED_OAPPROX_SPEC_GAME_DIAGONAL
		// RED_OAPPROX_SPEC_GAME_MAGNITUDE
		// RED_OAPPROX_SPEC_GAME_UNTIMED
		// RED_OAPPROX_SPEC_GAME_MODE_ONLY
		// RED_OAPPROX_SPEC_GAME_NONE
		//
		// fe is for abstraction of the environment variables 
		// and can be of the following values: 
		// RED_NOAPPROX_ENVR_GAME
		// RED_OAPPROX_ENVR_GAME_DIAG_MAG
		// RED_OAPPROX_ENVR_GAME_DIAGONAL
		// RED_OAPPROX_ENVR_GAME_MAGNITUDE
		// RED_OAPPROX_ENVR_GAME_UNTIMED
		// RED_OAPPROX_ENVR_GAME_MODE_ONLY
		// RED_OAPPROX_ENVR_GAME_NONE 
		//
		// fe is for abstraction of the global variables 
		// and can be of the following values: 
		// RED_NOAPPROX_GLOBAL_GAME
		// RED_OAPPROX_GLOBAL_GAME_DIAG_MAG
		// RED_OAPPROX_GLOBAL_GAME_DIAGONAL
		// RED_OAPPROX_GLOBAL_GAME_MAGNITUDE
		// RED_OAPPROX_GLOBAL_GAME_UNTIMED
		// RED_OAPPROX_GLOBAL_GAME_MODE_ONLY
		// RED_OAPPROX_GLOBAL_GAME_NONE  
	char	*role_spec, 
		// This string is of the form 
		//   "m1, m2, ..., mk; s1, s2, ..., sj;"
		// where m1, m2, ..., mk is a sequence of process indices 
		// for the model automata and 
		// s1, s2, ..., sj is a sequence of process indices 
		// for the specification automata. 
		// All the other process indices are for the environment
		// automata. 
	... 
	);
  
  
extern redgram red_reduce_symmetry(
	redgram	d, 
	int	flag_symmetry
	);
  
  
redgram red_reduce_inactive(
	redgram	d
	);
  
  
/* The following procedures are for garbage-collection control. 
 */ 
extern int 	red_garbage_collect();
extern int 	red_push(redgram d);
extern redgram  red_pop(int i);
extern redgram  red_stack(int i);
extern void	red_set_stack(int i, redgram d); 
extern void	red_protect_from_gc(redgram d);  
extern void	red_unprotect_all_from_gc();  
extern void	red_protect_aux_from_gc(redgram d);  
extern void	red_unprotect_all_aux_from_gc(); 
 
/* The following three procedures are for token-based protection 
 * from garbage-collection. 
 */ 
extern int	red_get_a_protection_token(); 
extern void	red_protect_with_token(redgram d, int t);  
extern void	red_release_token(int t);  

extern void    red_print_variables();
  
extern void    red_print_variable(int vi) ;
  
  
extern void	red_print_inline_declarations(); 

extern void	red_print_xtions(), red_print_xtions_details();
extern void	red_print_xtion(int xi, int pi), 
		red_print_xtion_details(int xi);


  
extern void	red_print_sync_xtions(), 
		red_print_sync_xtion_compositions();
  
  
extern void    red_print_sync_xtion(int sxi);
  
  
extern void    red_print_game_sync_xtions();
  
  
extern void    red_print_game_sync_xtion(int sxi);
  
  
extern void    red_print_modes();
  
  
  
extern void    red_print_mode(int mi);

  
  
  
extern void    red_print_summary();
  
  
extern void    red_print_diagram(redgram d);
  
  
extern void    red_print_diagram_formula(redgram d);
  
  
  
extern int 	red_diagram_root_var(redgram d);
  
  
 
  
  
/* The following procedures are for the control of transition firing. 
 */ 
extern int red_disable_xtion(int xi);
  
  
  
extern int 	red_enable_xtion(int xi);
  
  
extern int 	red_check_xtion_enabled(int xi);
  
  
  
  
extern int 	red_disable_proc(int pi); 
  
  
extern int 	red_enable_proc(int pi);
  
  
extern int red_check_proc_enabled(int pi);  
  
  
extern int red_disable_sync_xtion(int sxi);
  
  
  
extern int red_enable_sync_xtion(int sxi);
  
  
  
extern int red_check_sxi_enabled(int sxi);
  
  
  
extern int red_disable_proc_xtion(int xi, int pi);
  
  
  
  
extern int red_enable_proc_xtion(int xi, int pi); 


  
  
  
  
extern int red_check_enabled_specific(int xi, int pi);



extern redgram	red_first_cube(redgram d), red_next_cube(redgram d); 

// The following procedure also returns the variable index of vn in the 
// variable table.  
// Upon return, *lbp and *ubp respectively record the lower-bound 
// and upper-bound of vn in diagram d. 
// If vn is not found, -1 is returned. 
extern int 	red_get_cube_discrete_value(
  redgram d, char *vn, int *lbp, int *ubp
); 




/***********************************************************************
 *  The following routines are for the parsing of models and 
 *  extracting information out of it. 
 */ 
extern void	red_parse_model(char *mf); 

extern int	red_first_macro_const(), red_next_macro_const(); 

extern char	*red_query_string_current_macro_const(); 

extern int	red_query_window_width(), red_query_window_height(); 

extern void 
  red_set_window_width(int window_width), 
  red_set_window_height(int window_height);   


extern int 	red_next_var_declaration(), red_first_var_declaration(); 

extern char	*red_query_string_current_var_declaration(); 

extern int	red_first_inline_exp_declaration(), 
		red_next_inline_exp_declaration(); 

extern char	*red_query_string_current_inline_exp_declaration(); 

extern char	*red_query_string_declaration_header(); 

extern int	red_first_mode(), red_next_mode(); 

extern char	*red_query_current_mode_name(), 
		*red_query_string_current_mode_urgency(), 
		*red_query_string_current_mode_invariance();  
  
  
extern int	red_query_current_mode_position_x(), 
		red_query_current_mode_position_y(), 
		red_query_current_mode_shape(), 
		red_query_current_mode_rectangle_width(); 
  
#define	red_query_current_mode_oval_xradius	red_query_current_mode_rectangle_width
#define	red_query_current_mode_triangle_radius	red_query_current_mode_rectangle_width

extern int	red_query_current_mode_rectangle_height();  
  
#define	red_query_current_mode_oval_yradius	red_query_current_mode_rectangle_height

extern int	red_query_current_mode_triangle_direction(); 
   
extern int	red_query_current_mode_color(); 

extern int	red_query_current_mode_rate_spec_count(), 
		red_query_current_mode_xtion_count(); 

extern int	red_first_xtion(), red_next_xtion(); 
  
extern char	*red_query_string_current_xtion_syncs(), 
		*red_query_string_current_xtion_trigger(), 
		*red_query_string_current_xtion_statement(), 
		*red_query_current_xtion_dest_name(), 
		*red_query_string_current_xtion(); 
  
extern int 	red_first_xtion_point(), 
		red_next_xtion_point(), 
		red_current_xtion_point_x(), 
		red_current_xtion_point_y(); 
  
  

extern int	red_first_rate_spec(), red_next_rate_spec(); 

extern char	*red_query_string_current_rate_spec(); 
  



/************************************************************************
  The following routines are for supporting the query and selection of 
  processes and transitions that appear in a sync bulk diagram.  
 */
extern redgram	red_query_diagram_current_state();  

extern void	red_set_discrete_current_state(
  char	*, // for the variable name of the LHS
  int	   // for the value of the RHS
); 

extern redgram	red_begin_sync_xtion_bulk_restriction_fwd(
  redgram	dsrc
); 
extern redgram	red_begin_sync_xtion_bulk_restriction_bck(
  redgram	dsrc
); 

extern void	red_end_sync_xtion_bulk_restriction(); 


extern redgram	
  red_restrict_sync_bulk(
    int, // pi, 
    int // xi
  ), 
  red_restrict_stream_sync_bulk(
    int, // pi, 
    int, // xi, 
    int, // soi, 
    int  // value
  ),  
  red_malloc_exp_sync_bulk(
    int, // pi, 
    int, // xi, 
    int  //soi
  ),  
  red_free_exp_sync_bulk(
    int, // pi, 
    int, // xi, 
    int  // soi
  ); 

extern int 
  red_get_stream_output(
    int	, // pi, 
    int	, // xi, 
    int	  // soi
  ); 

extern redgram 
  red_query_malloc_space(); 

extern void 
  red_set_malloc_space(
    redgram 
  ); 

extern int
  red_first_pxpair_for_roles(int flag_game_roles), 
  red_next_pxpair_for_roles(int flag_game_roles), 
  red_query_current_pi_for_roles(int flag_game_roles), 
  red_query_current_xi_for_roles(int flag_game_roles);  

extern redgram	red_execute_sync_bulk_restriction(
  redgram 	dpath, 
  int		*last_time_progress_lb_ptr, 
  int		*last_time_progress_ub_ptr, 
  int		flag_game_roles, 
		// The argument consists of three flag values. 
		// Each flag may appear or not. 
		// The three flag values are 
		// RED_GAME_MODL, 
		// RED_GAME_SPEC, 
		// RED_GAME_ENVR. 
  int 		flag_time_progress, 
		// Two argument values are 
		// RED_NO_TIME_PROGRESS and 
		// RED_TIME_PROGRESS.  
  int		flag_normality, 
		// Some argument values are 
		// RED_NORM_ZONE_NONE, 
		// RED_NORM_ZONE_MAGNITUDE_REDUCED, and 
		// RED_NORM_ZONE_CLOSURE. 
  int		flag_action_approx, 
		// Some argument values are 
		// RED_NO_ACTION_APPROX, 
		// RED_ACTION_APPROX_NOXTIVE, and 
		// RED_ACTION_APPROX_UNTIMED.  
  int		flag_reduction, 
		// Two argument values are 
		// RED_NO_REDUCTION_INACTIVE and 
		// RED_REDUCTION_INACTIVE.  
  int		flag_state_approx, 
		// The argument values is the bit-wise 
		// of four flag values of the following form: 
		// fm | fs | fe | fg. 
		// fm is for abstraction of the model variables 
		// and can be of the following values:  
		// RED_NOAPPROX_MODL_GAME
		// RED_OAPPROX_MODL_GAME_DIAG_MAG
		// RED_OAPPROX_MODL_GAME_DIAGONAL
		// RED_OAPPROX_MODL_GAME_MAGNITUDE
		// RED_OAPPROX_MODL_GAME_UNTIMED
		// RED_OAPPROX_MODL_GAME_MODE_ONLY
		// RED_OAPPROX_MODL_GAME_NONE.  
		//
		// fs is for abstraction of the specification variables 
		// and can be of the following values: 
		// RED_NOAPPROX_SPEC_GAME
		// RED_OAPPROX_SPEC_GAME_DIAG_MAG
		// RED_OAPPROX_SPEC_GAME_DIAGONAL
		// RED_OAPPROX_SPEC_GAME_MAGNITUDE
		// RED_OAPPROX_SPEC_GAME_UNTIMED
		// RED_OAPPROX_SPEC_GAME_MODE_ONLY
		// RED_OAPPROX_SPEC_GAME_NONE
		//
		// fe is for abstraction of the environment variables 
		// and can be of the following values: 
		// RED_NOAPPROX_ENVR_GAME
		// RED_OAPPROX_ENVR_GAME_DIAG_MAG
		// RED_OAPPROX_ENVR_GAME_DIAGONAL
		// RED_OAPPROX_ENVR_GAME_MAGNITUDE
		// RED_OAPPROX_ENVR_GAME_UNTIMED
		// RED_OAPPROX_ENVR_GAME_MODE_ONLY
		// RED_OAPPROX_ENVR_GAME_NONE 
		//
		// fe is for abstraction of the global variables 
		// and can be of the following values: 
		// RED_NOAPPROX_GLOBAL_GAME
		// RED_OAPPROX_GLOBAL_GAME_DIAG_MAG
		// RED_OAPPROX_GLOBAL_GAME_DIAGONAL
		// RED_OAPPROX_GLOBAL_GAME_MAGNITUDE
		// RED_OAPPROX_GLOBAL_GAME_UNTIMED
		// RED_OAPPROX_GLOBAL_GAME_MODE_ONLY
		// RED_OAPPROX_GLOBAL_GAME_NONE  
  int		flag_symmetry 
		// Some possible argument values are
		// RED_NO_SYMMETRY, 
		// RED_SYMMETRY_ZONE, 
		// RED_SYMMETRY_DISCRETE, 
		// RED_SYMMETRY_POINTER, 
		// RED_SYMMETRY_STATE. 
);   


  

#endif 

#ifdef __cplusplus
} 
#endif



 
