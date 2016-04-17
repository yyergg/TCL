#ifndef _REDLIB_H 
#define _REDLIB_H 

/*******************************************************************************************
 * This header file contains the constants, structure definitions, and the library procedure 
 * interfaces.  
 *
 * Note that usually we don't execute transitions on state images.  
 * This is because that there could be synchronizations among transitions.  
 * RED compiles a table called synchronous transition table in which 
 * each entry corresponds to a minimal combination of synchronizing transitions.  
 * RED also compiles another transition bulk CRD+MDD for the bulk evaluation of 
 * pre or post images for a set of complex synchronization of transitions.  
 * Thus unless for curioristy, users should use synchronous transition table 
 * to calculate the pre or post image.  
 * 
 * The synchronous transitions are stored in array SYNC_XTION[..] whose entry count 
 * is stored in SYNC_XTION_COUNT.  
 * If all your synchronizations are pairwise, then this should store all the synchronizous 
 * transitions you need, including those transitions without synchronization.  
 * The synchronous transitions of those complex synchronizations are stored in 
 * CRD+MDD RED_complex_sync_xtions().  
 * The relevant procedures for computing post and pre-images are as follows. 
 *   RED_sync_xtion_specific_fwd(d, sxi) : post image of a particular synchronous transiton indexed sxi
 *   RED_sync_xtion_specific_bck(d, sxi) : pre-image of a particular synchronous transiton indexed sxi
 *   RED_sync_xtion_iterative_fwd(d) : post image of all simple synchronous transitons stored in SYNC_XTION[]
 *   RED_sync_xtion_iterative_bck(d) : pre-image of all simple synchronous transitons stored in SYNC_XTION[]
 *   RED_sync_xtion_bulk_fwd(d) : post image of all complex synchronous transitons stored 
 *				  in RED_complex_sync_xtions().  
 *   RED_sync_xtion_bulk_bck(d) : pre-image of all complex synchronous transitons stored 
 *				  in RED_complex_sync_xtions(). 
 *   RED_sync_xtion_fwd(d) : post image of all synchronous transitions. 
 *   RED_sync_xtion_bck(d) : post image of all synchronous transitions. 
 */
/******************************************************************************
 * The plan for the interface.  
 *	All the constants are to be copied from the redbasics.h, ... 
 * Thus it is not OK to revise the constants in this file. 
 * You have to modify them in the corresponding files and then cut and paste here. 
 */

#define _REDLIB_H

#include <stdarg.h> 

typedef struct red_type *redgram; 

/****** redbasics.h ******************************************************************
 * The following constants are defined in redbasics.h 
 */ 


/* The following flags are used to set options in controlling the operation 
 * of the red engine. 
    The option can be set with the following procedure. 
    void	RED_option_set(int opt, int value)
    opt is a macro constant like RED_OPT_... declared in the following.  
    value is the value of the option to be set. 
    For example, RED_option_set(RED_OPT_ABSTRACTION, FLAG_OVER_APPROX) sets 
    the abstraction to over-approximation.  
    
    We can also set the flag with bit-values through the following procedure.   
    void	RED_flag_set(int opt)
    Here opt is the macro constant like RED_OPT_... used in RED_option_set(opt, ...).  
    
    We can also clear the flag with bit-values through the following procedure.   
    void	RED_flag_clear(int opt)
    Here opt is the macro constant like RED_OPT_... used in RED_option_set(opt, ...).  
    
    We can also check the flag value with the following procedure. 
    int	RED_flag_check(int opt) 
    Here opt is the macro constant like RED_OPT_... used in RED_option_set(opt, ...).  	
 */ 

/* MASK values for GLOBAL_STATUS[0]  */





/********************************************************************************
 * The following are constants used to interface redlib.a
 */
#define	RED_FLAG_SUCCESS 	1
#define	RED_FLAG_FAILURE	0 

#define	RED_FLAG_UNKNOWN	-1
#define	RED_MODE_OVAL		1 
#define	RED_MODE_RECTANGLE	2 
#define	RED_MODE_TRIANGLE	3 

#define	RED_TRIANGLE_LEFTWARD	1 
#define	RED_TRIANGLE_RIGHTWARD	2 
#define	RED_TRIANGLE_UPWARD	3 
#define	RED_TRIANGLE_DOWNWARD	4 


/* Some string used for easy construction of the model. 
 */ 
#define RED_MODE           "RED_MODE"
#define RED_CONDITION_TRUE "true"
#define RED_CONDITION_FALSE "false"
#define RED_OO             (INT_MAX)
#define RED_P              "P"
#define RED_Q              "Q"
#define RED_R              "R"
#define RED_S              "S"

/* 
typedef struct red_type RED_TYPE;  //struct red_type in redbop.h 
typedef struct xtion_type XTION_TYPE; //struct xtion_type in red.h 
typedef struct action_type ACTION_TYPE; //struct action_type in red.h 
*/ 

/* Type constants for the declaration of variables in model construction.  
 * Used for the type in RED_begin_variable_declaration(scope, type)  
 */
#define	RED_VAR_TYPE		1

#define RED_TYPE_SYNCHRONIZER   	1002
#define RED_TYPE_DISCRETE       	1003
#define RED_TYPE_DENSE          	1004
#define RED_TYPE_CLOCK          	1005
#define RED_TYPE_POINTER        	1006
#define RED_TYPE_PARAMETER      	1007
#define RED_TYPE_MTERMINAL		1008
#define RED_TYPE_BOOLEAN		1009
#define RED_TYPE_CRD	1010

#define	RED_VAR_SCOPE				2

#define RED_SCOPE_GLOBAL        		1010
#define RED_SCOPE_LOCAL         		1011
 
/*************************************************************
 *  The following argument values are for 
 *  red_input_model(), 
 *  red_input_rules(), 
 *  red_end_declaration().  
 */
 
#define	RED_PARSING_ONLY			1200
#define	RED_NO_REFINED_GLOBAL_INVARIANCE	1201
#define	RED_REFINE_GLOBAL_INVARIANCE		1202


/*************************************************************
 *  The following argument values are for red_reach_bck(), 
 *  red_reach_fwd(), red_model_check(), red_bisim_check(), 
 *  and red_sim_check().  
 */
#define	RED_TASK_TYPE				1020

#define RED_TASK_REDLIB_SESSION			(0x1)
#define RED_TASK_SAFETY				(0x2)
#define RED_TASK_RISK				(0x3) 
#define RED_TASK_GOAL				(0x4) 
#define RED_TASK_ZENO				(0x5) 
#define RED_TASK_DEADLOCK			(0x6) 
#define RED_TASK_MODEL_CHECK			(0x7) 
#define RED_TASK_SIMULATE			(0x8) 
#define RED_TASK_BRANCH_SIM_CHECK		(0x9) 
#define RED_TASK_BRANCH_BISIM_CHECK		(0xA) 

#define RED_PARAMETRIC_ANALYSIS		(0x010) 
#define	RED_NO_PARAMETRIC_ANALYSIS	(0x020) 

#define	RED_ALL_ROLES		(0x0700) 
#define	RED_GAME_ROLES		(0x0700) 
#define	RED_GAME_MODL		(0x0200) 
#define	RED_GAME_SPEC		(0x0100) 
#define RED_GAME_ENVR		(0x0400) 

//We use the following two for reachability analysis. 
#define RED_FULL_REACHABILITY		(0x030) 
#define	RED_NO_FULL_REACHABILITY	(0x040) 

//We then use the following two for greatest fixpoint analysis. 
#define RED_COMPLETE_GREATEST_FIXPOINT	(0x30) 
#define RED_NO_COMPLETE_GREATEST_FIXPOINT (0x40) 

#define RED_NO_REACHABILITY_DEPTH_BOUND	-1 

#define RED_COUNTER_EXAMPLE		(0x050) 
#define	RED_NO_COUNTER_EXAMPLE		(0x060) 

#define	RED_NO_TIME_PROGRESS		(0x070) 
#define RED_TIME_PROGRESS		(0x080) 

#define	RED_NORM_ZONE_NONE			(0x01000)
#define RED_NORM_ZONE_MAGNITUDE_REDUCED		(0x02000)
#define	RED_NORM_ZONE_CLOSURE			(0x03000)
#define RED_NORM_ZONE_MAGNITUDE_ONE_RIPPLE	(0x04000) 

#define RED_NORM_HYBRID_2REDUNDANCY_ELIMINATION_DOWNWARD	(0x00010000)
#define RED_NORM_HYBRID_3REDUNDANCY_ELIMINATION_DOWNWARD	(0x00020000)
#define RED_NORM_HYBRID_4REDUNDANCY_ELIMINATION_DOWNWARD	(0x00040000)
#define RED_NORM_HYBRID_2REDUNDANCY_ELIMINATION_LOOKAHEAD	(0x00080000)
#define RED_NORM_HYBRID_PROOF_OBLIGATIONS			(0x00100000)

#define	RED_NO_ACTION_APPROX		(0x090)
#define	RED_ACTION_APPROX_NOXTIVE	(0x0A0)
#define	RED_ACTION_APPROX_UNTIMED	(0x0B0)

#define	RED_NO_REDUCTION_INACTIVE	(0x0C0)
#define	RED_REDUCTION_INACTIVE_NOXTIVE	(0x0D0)
#define	RED_REDUCTION_INACTIVE		(0x0E0)

#define	RED_MASK_APPROX			(0xd0000000)
#define	RED_NOAPPROX			(0x00000000)
#define	RED_UAPPROX			(0x80000000) 
#define	RED_OAPPROX			(0x40000000)

#define RED_MASK_OAPPROX_STRATEGY	(0xF0000) 

#define	RED_OAPPROX_STRATEGY_GAME	(0x10000)

#define RED_MASK_OAPPROX_SPEC_GAME	(0xF000F) 
#define	RED_NOAPPROX_SPEC_GAME		(0x00000) 
#define RED_OAPPROX_SPEC_GAME_DIAG_MAG	(0x10001)
#define RED_OAPPROX_SPEC_GAME_DIAGONAL	(0x10002)
#define RED_OAPPROX_SPEC_GAME_MAGNITUDE	(0x10003)
#define RED_OAPPROX_SPEC_GAME_UNTIMED	(0x10004)
#define RED_OAPPROX_SPEC_GAME_MODE_ONLY	(0x10005)
#define RED_OAPPROX_SPEC_GAME_NONE	(0x10006)

#define RED_MASK_OAPPROX_MODL_GAME	(0xF00F0) 
#define	RED_NOAPPROX_MODL_GAME		(0x00000) 
#define RED_OAPPROX_MODL_GAME_DIAG_MAG	(0x10010)
#define RED_OAPPROX_MODL_GAME_DIAGONAL	(0x10020)
#define RED_OAPPROX_MODL_GAME_MAGNITUDE	(0x10030)
#define RED_OAPPROX_MODL_GAME_UNTIMED	(0x10040)
#define RED_OAPPROX_MODL_GAME_MODE_ONLY	(0x10050)
#define RED_OAPPROX_MODL_GAME_NONE	(0x10060)

#define RED_MASK_OAPPROX_ENVR_GAME	(0xF0F00) 
#define	RED_NOAPPROX_ENVR_GAME		(0x00000) 
#define RED_OAPPROX_ENVR_GAME_DIAG_MAG	(0x10100)
#define RED_OAPPROX_ENVR_GAME_DIAGONAL	(0x10200)
#define RED_OAPPROX_ENVR_GAME_MAGNITUDE	(0x10300)
#define RED_OAPPROX_ENVR_GAME_UNTIMED	(0x10400)
#define RED_OAPPROX_ENVR_GAME_MODE_ONLY	(0x10500)
#define RED_OAPPROX_ENVR_GAME_NONE	(0x10600)

#define RED_MASK_OAPPROX_GLOBAL_GAME		(0xFF000) 
#define RED_NOAPPROX_GLOBAL_GAME		(0x00000)
#define RED_OAPPROX_GLOBAL_GAME_DIAG_MAG	(0x11000)
#define RED_OAPPROX_GLOBAL_GAME_DIAGONAL	(0x12000)
#define RED_OAPPROX_GLOBAL_GAME_MAGNITUDE	(0x13000)
#define RED_OAPPROX_GLOBAL_GAME_UNTIMED		(0x14000)
#define RED_OAPPROX_GLOBAL_GAME_NONE		(0x16000)

#define RED_OAPPROX_GAME_DIAG_MAG	(  RED_OAPPROX \
                                         | RED_OAPPROX_SPEC_GAME_DIAG_MAG \
					 | RED_OAPPROX_MODL_GAME_DIAG_MAG \
					 | RED_OAPPROX_ENVR_GAME_DIAG_MAG \
					 | RED_OAPPROX_GLOBAL_GAME_DIAG_MAG \
					 )
#define RED_OAPPROX_GAME_DIAGONAL	(  RED_OAPPROX \
                                         | RED_OAPPROX_SPEC_GAME_DIAGONAL \
					 | RED_OAPPROX_MODL_GAME_DIAGONAL \
					 | RED_OAPPROX_ENVR_GAME_DIAGONAL \
					 | RED_OAPPROX_GLOBAL_GAME_DIAGONAL \
					 )
#define RED_OAPPROX_GAME_MAGNITUDE	(  RED_OAPPROX \
                                         | RED_OAPPROX_SPEC_GAME_MAGNITUDE \
					 | RED_OAPPROX_MODL_GAME_MAGNITUDE \
					 | RED_OAPPROX_ENVR_GAME_MAGNITUDE \
					 | RED_OAPPROX_GLOBAL_GAME_MAGNITUDE \
					 )
#define RED_OAPPROX_GAME_LOCAL_MAGNITUDE (  RED_OAPPROX \
                                         | RED_OAPPROX_SPEC_GAME_MAGNITUDE \
					 | RED_OAPPROX_MODL_GAME_MAGNITUDE \
					 | RED_OAPPROX_ENVR_GAME_MAGNITUDE \
					 | RED_NOAPPROX_GLOBAL_GAME \
					 )
#define RED_OAPPROX_GAME_UNTIMED	(  RED_OAPPROX \
                                         | RED_OAPPROX_SPEC_GAME_UNTIMED \
					 | RED_OAPPROX_MODL_GAME_UNTIMED \
					 | RED_OAPPROX_ENVR_GAME_UNTIMED \
					 | RED_OAPPROX_GLOBAL_GAME_UNTIMED \
					 )


/*****************************************
 * options for process-oriented symmetry reduction. 
 */
#define RED_NO_SYMMETRY		(0x1000000)
#define RED_SYMMETRY_ZONE	(0x2000000)
#define RED_SYMMETRY_DISCRETE	(0x3000000)
#define	RED_SYMMETRY_POINTER	(0x4000000)
#define RED_SYMMETRY_STATE	(0x5000000)

#define	RED_MASK_ZENO		(0x0F000000) 
#define	RED_PLAIN_NONZENO	(0x06000000)
#define RED_APPROX_NONZENO	(0x07000000)
#define RED_ZENO_TRACES_OK	(0x08000000) 

#define RED_NO_PRINT		0

/* end of argument values for red_reach_fwd(), .... 
 */ 



#define	RED_VAR_PRIMED		1030 
#define	RED_VAR_PRIMED		1030 
#define RED_VAR_PROC		1040 
#define RED_VAR_LB		1050 
#define RED_VAR_UB		1060 
#define RED_VAR_CLOCK1		1070 
#define RED_VAR_CLOCK2		1080 
#define RED_VAR_SYSGEN 		1090
#define	RED_VAR_STREAM_ORDERED	1100


/* Used for the op in RED_formulus_ineq(lhs_name, op, rhs_name)
 */
#define RED_OP_LESS             ARITH_LESS
#define RED_OP_LEQ              ARITH_LEQ
#define RED_OP_EQ               ARITH_EQ
#define RED_OP_NEQ              ARITH_NEQ
#define RED_OP_GEQ              ARITH_GEQ
#define RED_OP_GREATER          ARITH_GREATER


#define RED_XTION_SYNC_COUNT		01 
#define RED_XTION_SRC_MODE		02 
#define RED_XTION_DST_MODE		03 
#define RED_XTION_PROCESS_COUNT		04 
#define RED_XTION_STREAM_OP_COUNT	05 

#define	RED_XTION_STREAM_OPEN_INPUT	11 
#define	RED_XTION_STREAM_OPEN_OUTPUT	12 
#define	RED_XTION_STREAM_CLOSE		13 
#define	RED_XTION_STREAM_INPUT		14 
#define	RED_XTION_STREAM_OUTPUT		15 
#define	RED_XTION_MALLOC		16 
#define	RED_XTION_FREE			17 



#define RED_XTION_SYNC_DIRECTION	0330  
#define RED_XTION_SYNC_XMIT		0331
#define RED_XTION_SYNC_RECV		0332

#define RED_XTION_SYNC_QUANTIFIED_ADDRESS 		0340
#define	RED_XTION_SYNC_NO_CORRESPONDENCE_REQUIREMENT	0341 
#define RED_XTION_SYNC_QUANTIFIED_CORRESPONDENCE_VAR	0342
#define	RED_XTION_SYNC_CORRESPONDENCE_EXPRESSION	0343

#define	RED_XTION_SYNC_VAR_INDEX			0350 
#define	RED_XTION_SYNC_QFD_CORRESPONDENCE_VAR_INDEX	0360 


#define RED_MODE_XTION_COUNT	100 
#define RED_MODE_URGENT		101 
#define RED_MODE_PROCESS_COUNT	102 
#define RED_MODE_RATE_LB_NUM	200
#define RED_MODE_RATE_LB_DEN	201 
#define RED_MODE_RATE_UB_NUM	202 
#define RED_MODE_RATE_UB_DEN	203 
 

#define RED_SYSTEM_UNTIMED	100
#define RED_SYSTEM_TIMED	101
#define RED_SYSTEM_HYBRID	102
#define RED_SYSTEM_UNKNOWN	103
 
#define RED_ARITH_ADD      	ARITH_ADD
#define RED_ARITH_MINUS    	ARITH_MINUS
#define RED_ARITH_MULTIPLY 	ARITH_MULTIPLY
#define RED_ARITH_DIVIDE   	ARITH_DIVIDE
#define	RED_ARITH_MODULO	ARITH_MODULO
#define RED_ARITH_MIN   	ARITH_MIN
#define RED_ARITH_MAX   	ARITH_MAX



#define RED_NO_COVERAGE		0
#define	RED_COVERAGE_ACM	(0x01)
#define	RED_COVERAGE_DCM	(0x02)
#define RED_COVERAGE_TCM	(0x04)
#define	RED_COVERAGE_RCM	(0x08) 
#define	RED_COVERAGE_DTCM	(0x10)




#define	RED_GARBAGE_SILENT	0
#define	RED_GARBAGE_REPORT	1



/* The following option flags are all temporary for experiment.
 * They are not meant to be open to the public for long. 
 * They are also not meant to be supported by REDLIB for long. 
 * Once we have in the same word as the option for 
 * RED_ZENO_TRACES_OK. 
 * They are for experiment and not meant for permanent support. 
 */ 
#define	RED_MASK_EXPERIMENT_TOKEN				(0xFF) 

// The following experiment token must match 
// the internal token hold in REDLIB for the following 
// experiment flags to be effective.  
// 
#define	RED_TOKEN_NO_EXPERIMENT					(0x00) 

#define	RED_MASK_TCTCTL_CHECKING				(0x00100)
#define	RED_TCTCTL_CHECKING					(0x00100)
#define	RED_NO_TCTCTL_CHECKING					(0x00000) 

#define	RED_TIME_EXISTS_ALWAYS_SEGMENTED_EVALUATION		(0x00200) 
#define	RED_TIME_EXISTS_ALWAYS_NO_SEGMENTED_EVALUATION		(0x00000) 

#define	RED_TIME_TCONVEXITY_SHARED_PARTITIONS			(0x00400) 
#define	RED_TIME_TCONVEXITY_NO_SHARED_PARTITIONS		(0x00000) 

#define	RED_MASK_TIME_PROGRESS_OPTIONS				(0x0F000) 
#define	RED_TIME_PROGRESS_NONE					(0x00000) 
#define RED_TIME_PROGRESS_ASSUMED_CONVEXITY			(0x01000) 

#define RED_TIME_PROGRESS_FULL_FORMULATION			(0x04000) 
#define	RED_TIME_PROGRESS_SHARED_CONCAVITY			(0x05000)
#define	RED_TIME_PROGRESS_ADAPTIVE_SHARED_CONCAVITY		(0x06000)

#define	RED_TIME_PROGRESS_SPLIT_CONVEXITIES			(0x08000) 
#define	RED_TIME_PROGRESS_SHARED_SPLIT_CONVEXITIES		(0x09000)	
#define RED_TIME_PROGRESS_ADAPTIVE_SHARED_SPLIT_CONVEXITIES 	(0x0A000)
#define RED_TIME_PROGRESS_ON_THE_FLY_SHARED_SPLIT_CONVEXITIES 	(0x0B000)

#define RED_TIME_PROGRESS_EASY_CONCAVITY			(0x0C000) 
#define RED_TIME_PROGRESS_SHARED_EASY_CONCAVITY			(0x0D000) 
#define RED_TIME_PROGRESS_ADAPTIVE_SHARED_EASY_CONCAVITY	(0x0E000) 

#define RED_MASK_TIME_PROGRESS_ANALYSIS				(0xF0000)
#define RED_TIME_PROGRESS_ANALYSIS_NONE				(0x00000)
#define RED_TIME_PROGRESS_ANALYSIS_TCTCTL			(0x10000)
#define RED_TIME_PROGRESS_ANALYSIS_ADVANCED			(0x20000) 

#define	RED_GFP_ON_THE_FLY					(0x100000) 
#define	RED_GFP_COMBINATONAL					(0x000000) 

#define RED_GFP_EASY_STRONG_FAIRNESS				(0x200000)

/*  The following flags are obsolete. 
#define RED_MASK_FAIRNESS_ASSUMPTIONS_EVAL 			(0x600000) 
#define RED_FAIRNESS_ASSUMPTIONS_EVAL_CONJ 			(0x000000) 
#define	RED_FAIRNESS_ASSUMPTIONS_EVAL_CONCAT			(0x200000)
#define RED_FAIRNESS_ASSUMPTIONS_EVAL_OCC_VAR			(0x400000) 
*/
#define RED_MASK_GFP_PATH		 			(0x800000) 
#define RED_GFP_PATH_INVARIANCE 				(0x000000) 
#define	RED_GFP_PATH_FXP					(0x800000) 

#define RED_MASK_LUB_EXTRAPOLATION				(0xF000000) 
#define RED_GLUB_EXTRAPOLATION					(0x1000000) 
#define RED_LUB_EXTRAPOLATION					(0x2000000) 

/* All BDD+CRD+HRD not in the user stack will be recycled. 
 * The following routine push a BDD+CRD+HRD into the user stack to
 * protect it from garbage collection.  
 * The returned index can be used to access the BDD+CRD+HRD with 
 * RED_user().  
 */ 
/* The stack to save CRD+MTMDD+HRD from garbage collection.  
 */ 

/* The following parameters are for the argument to procedure 
 * red_begin_declaration(int flag_declare)
 */
#define RED_MASK_DECLARE_VARIABLES	2
#define RED_RENEW_VARIABLES		2
#define RED_ADD_VARIABLES		1
#define RED_CHECK_VARAIABLES		0 

#define RED_MASK_DECLARE_RULES		4
#define RED_RENEW_RULES			4
#define RED_ADD_RULES			0


/***************************************************************
 *  The following two constants are used for choosing from 
 *  the synchronous transition table and 
 *  the game synchronous transition table in the execution of 
 *  red_sync_xtion_bck() and related procedures.  
 */
#define	RED_USE_GAME_SYNC_XTION		1
#define	RED_USE_DECLARED_SYNC_XTION	0


#define	RED_DEPTH_ENUMERATIVE_DEFAULT	-1 
#define	RED_DEPTH_ENUMERATIVE_ALL	-2


#ifndef _REDLIB_REACH_RETURN_STRUCTURES 
#define _REDLIB_REACH_RETURN_STRUCTURES 




/***************************************************************
 *  The following structures are for the return value of reachability analysis. 
 *  Unlike the return types for model-checking and sim-checking, 
 *  this return type is actually used in red.  
 *  The other two types are only used in redlib for the users. 
 */
struct counter_example_party_type { 
  int	proc, xtion; 	
}; 

struct counter_example_node_type { 
  int					exit_sync_xtion_party_count; 
  struct counter_example_party_type	*exit_sync_xtion_party; 
  char					*exit_sync_xtion_string; 
  struct red_type 			*prestate; 
  struct counter_example_node_type	*next_counter_example_node; 
}; 



struct reachable_return_type { 
  int                                   status, 

#define MASK_REACHABLE_RETURN                   (0xF) 
#define FLAG_RESULT_EARLY_REACHED               1
#define FLAG_RESULT_FULL_FIXPOINT               2 
#define FLAG_RESULT_DEPTH_BOUND_FINISHED        4 

#define FLAG_COUNTER_EXAMPLE_GENERATED          (0x10)
#define FLAG_COUNTER_EXAMPLE_NOT_GENERATED      (0x00) 

#define FLAG_RESULT_PARAMETRIC_ANALYSIS         (0x20) 
#define FLAG_RESULT_NO_PARAMETRIC_ANALYSIS      (0x00) 

#define MASK_REACHABILITY_RESULT                (0xF00) 
#define FLAG_REACHABILITY_UNDECIDED             0
#define FLAG_NOT_REACHABLE                      (0x100)
#define FLAG_REACHABILITY_INCONCLUSIVE          (0x200)
#define FLAG_REACHABILITY_DETECTED              (0x300)

#define MASK_LFP_TASK_TYPE                      (0xF000)
#define MASK_LFP_TASK_RISK                      (0x1000)
#define MASK_LFP_TASK_GOAL                      (0x2000)
#define MASK_LFP_TASK_SAFETY                    (0x3000)
#define MASK_LFP_TASK_ZENO                      (0x5000)
#define MASK_LFP_TASK_DEADLOCK                  (0x6000)

                                        iteration_count, 
                                        counter_example_length; 	
  struct counter_example_node_type      *counter_example; 
  struct red_type                       *reachability, 
                                        *risk_parameter; 
}; 

#endif 



#ifndef _REDLIB_PS_EXP_STRUCTURES 
#define _REDLIB_PS_EXP_STRUCTURES 


/* Formula structure definition used in TCTL model-checking. 
 * This is to be implemented later. 
 */




struct parse_indirect_type {
  struct parse_variable_type	*pointer; 
  struct parse_indirect_type	*next_parse_indirect; 
}; 

#define PROC_QUANTIFIED_SYNC	-4
#define PROC_GLOBAL_EVENT	-3
#define	PROC_LOCAL		-2
#define	PROC_QFD		-1
#define PROC_GLOBAL		0

struct parse_atom_type {
  int				var_index,
				indirect_count, *indirect_vi,  
				status;
  struct ps_exp_type		**indirect_exp, 
				*exp_base_proc_index; /* -2: P, -1: qfd, 0: global, [1,oo): local */
  struct parse_variable_type	*var; 		/* only used when arith_op == TYPE_DISCRETE,
						 or TYPE_CLOCK, TYPE_POINTER;
				      		*/
  char				*var_name;	/* only used when 
  						 arith_op == TYPE_DISCRETE,
						 or TYPE_CLOCK, TYPE_POINTER;
						 Note, qname is now moved to 
						 exp_base_proc_index on 2004/1/2.
				      		*/
};

struct parse_mode_index_type { 
  int				value; 
  struct parse_mode_type	*parse_mode; 
  char				*mode_name; 	
}; 

struct parse_qsync_holder_type { 
  int				place_index; 
  char				*qsync_var_name; 
  struct parse_variable_type	*qsync_var, *place_holder_var; 	
}; 



#define	INTERVAL_LEFT_UNBOUNDED		8
#define	INTERVAL_LEFT_OPEN		4
#define	INTERVAL_LEFT_CLOSED		0
#define	INTERVAL_RIGHT_UNBOUNDED	2
#define	INTERVAL_RIGHT_OPEN		1
#define	INTERVAL_RIGHT_CLOSED		0


struct parse_interval_type {
  int			status; 
  struct ps_exp_type	*lbound_exp, /* NULL means negative infinity. */ 
			*rbound_exp; /* NULL means positive infinity. */ 
};

/* The following constants are used for the specific types of inequalities. 
 */ 
#define	MASK_EXP_TYPE			(0xFF) 
#define FLAG_EXP_STATIC			(0x01)
#define FLAG_EXP_DISCRETE_CONSTANT	(0x02)	/* x ~ c */
#define	FLAG_EXP_DISCRETE_LINEAR	(0x03)	/* a1 x1 + ... + an xn ~ c */
#define	FLAG_EXP_DISCRETE_POLYNOMIAL	(0x04)	/* polynomial ~ c */
#define	FLAG_EXP_CLOCK_CONSTANT		(0x05)	/* x ~ c */
#define	FLAG_EXP_CLOCK_DIFFERENCE	(0x06)	/* x - y ~ c */
#define FLAG_EXP_CLOCK_MIXED		(0x07)	/* a1 x1 + ... + an xn ~ c */
#define FLAG_EXP_CLOCK_DIFFERENCE_MIXED	(0x08)	/* a1 x1 + ... + an xn ~ c */
#define FLAG_EXP_DENSE_CONSTANT		(0x09)	/* x ~ c */
#define FLAG_EXP_DENSE_LINEAR		(0x0A)	/* a1 x1 + ... + an xn ~ c */
#define FLAG_EXP_DENSE_MIXED		(0x0B)	/* a1 x1 + ... + an xn ~ c */
#define FLAG_EXP_DENSE_POLYNOMIAL	(0x0C)

struct parse_arith_type {
  int			status; 
  struct ps_exp_type	*lhs_exp, *rhs_exp;
};


struct parse_term_type {
  struct ps_exp_type	*coeff, *operand;
};

struct parse_ineq_type {
  int				type, term_count;
  struct parse_term_type	*term;
  struct ps_exp_type		*offset;
};


struct parse_conditional_arith_type {
  int			type; 
  struct ps_exp_type	*cond, *if_exp, *else_exp;
};


struct ps_bunit_type {
  struct ps_exp_type 	*subexp;
  struct ps_bunit_type 	*bnext;
};


struct ps_bexp_type {
  int 			len;
  struct ps_bunit_type 	*blist;
};



struct ps_rexp_type {
  char				*clock_name;
  int				clock_index;
  struct parse_variable_type	*var;  
  struct ps_exp_type		*child;
};


struct ps_qexp_type {
  char			*quantifier_name;
  int			value;
  struct ps_exp_type	*child, *lb_exp, *ub_exp;
};


struct ps_project_type {
  int			variable_index;
  struct ps_exp_type	*child;
};
#define	vsim_type_offset	variable_index 
#define	clock_offset		variable_index 
#define var_type		variable_index 
#define var_proc		variable_index 



struct ps_fairness_link_type	{
  int				status; 
  struct ps_exp_type		*fairness;
  struct red_type		*red_fairness; 
  struct ps_fairness_link_type	*next_ps_fairness_link;
};



struct ps_mexp_type {
  int				time_lb, time_ub,
  				strong_fairness_count, weak_fairness_count;
  struct ps_exp_type		*path_child, *dest_child,  
				*time_restriction; 
  struct red_type		*red_early_decision_maker, 
				*red_time_restriction; 
  struct ps_fairness_link_type	*strong_fairness_list, *weak_fairness_list;
};



struct ps_eexp_type {
  struct ps_exp_type		*postcond_exp, 
				*precond_exp, *event_exp; 
  struct red_type		*red_early_decision_maker, 
				*red_events, 
				*red_postcond, *red_precond; 
};


struct red_predicate_type { 
  struct red_type	*red, *original_red; 
}; 

#define	MASK_INLINE_TYPE	(0xF)
#define	FLAG_INLINE_CONSTANT	1
#define FLAG_INLINE_DISCRETE 	2
#define FLAG_INLINE_BOOLEAN 	3 

struct ps_inline_declare_type { 
  char			*inline_exp_name; 
  int			status, formal_argument_count; 
  struct name_link_type	*formal_argument_list; 
  struct ps_exp_type	*declare_exp; 
}; 


struct ps_inline_call_type { 
  char			*inline_exp_name; // This is to be shared with 
                                          // the corresponding inline_declare. 
                                          // So don't release here. 
  struct ps_exp_type	*inline_declare, *instantiated_exp; 
  struct ps_bunit_type	*actual_argument_list; 
}; 



union	ps_union {
  int					value; 
  char					*argument; 
  struct ps_exp_type			*exp; 
  struct parse_atom_type		atom;
  struct parse_mode_index_type		mode_index; 
  struct parse_qsync_holder_type	qsholder;  
  struct parse_arith_type		arith;
  struct parse_conditional_arith_type   arith_cond; 
  struct ps_bexp_type			bexp;

  struct ps_inline_declare_type		inline_declare; 
  struct ps_inline_call_type		inline_call; 
  
  struct parse_interval_type		interval; 
  struct parse_ineq_type		ineq;
  struct red_predicate_type		rpred;
  struct ps_rexp_type			reset;
  struct ps_qexp_type			qexp;
  struct ps_mexp_type			mexp;
  struct ps_eexp_type			eexp; 
  struct ps_project_type		project; 
};

/* The following constants are used for status. */ 

/* The following two flags overlap with the FLAG_STATEMENT_ELSE and FLAG_STATEMENT_COMPOUND 
 * in redbasics.h.  
 * They are only used in simple statements and inequalities while 
 * FLAG_STATEMENT_ELSE and FLAG_STATEMENT_COMPOUND are only used for compound statements.  
 */
 
/* var_status 
 * The same as the status bits of VAR. 
 */ 

/* exp_status */ 
#define	FLAG_CONSTANT			(0x00000001) // This means 
						     // the expression 
/* This flag value is used only in type constant. */ 
#define MASK_MODE_INDEX			(0x00000002)
#define	FLAG_LOCAL_IDENTIFIER		(0x00000004)

#define	FLAG_HYBRID			(0x00000008)

#define FLAG_EXP_VAR_PRIMED		(0x00000001)

#define FLAG_VAR_FILLED_IN		(0x00000002)

#define FLAG_ONE_POS_CLOCK		(0x00000004) // These two flags are used to check if  
#define	FLAG_ONE_NEG_CLOCK		(0x00000008) // more clocks are not in difference form.
                                                     // This flag is only used in redgram.y  
                                                     // for assignment and 
                                                     // inequality analysis. 
                                                     // So we let it share 
                                                     // with flags fairness 
                                                     // occurrence and strengths. 



						     // evaluates to a constant. 
#define	FLAG_VAR_BOUNDS_DELAYED_EVAL	(0x00000010) //** used in VAR[]
#define MASK_FAIRNESS_STRENGTH		(0x00000020) // used for modal expresions 
#define FLAG_FAIRNESS_STRONG		(0x00000020) // used for modal expresions
#define FLAG_FAIRNESS_WEAK		(0x00000000) // used for modal expresions

#define FLAG_PATH_INSIDE		(0x00000040)
#define	FLAG_RESET_INSIDE		(0x00000080) 
#define FLAG_NEGATION_INSIDE		(0x00000100) 
#define	FLAG_DISJUNCTION_INSIDE		(0x00000200)
#define	FLAG_CONJUNCTION_INSIDE		(0x00000400) // we use this to detect time-convexity instead.
#define	FLAG_EXP_MODAL_INSIDE		(0x00000800)
#define FLAG_TCTCTL_SUBFORM		(0x00001000) 
#define FLAG_NOT_TCTCTL_SUBFORM		(0x00000000) 

#define FLAG_GFP_EARLY_DECISION		(0x00002000) 

#define	FLAG_TAIL_CONVEXITY_EVALUATION	(0x00004000) // used for modal operators
// The following constant is now replaced by FLAG_TCTCTL_SUBFORM
// #define	FLAG_PATH_CONVEXITY_EVALUATION	(0x20000000) // used for modal operators

#define	FLAG_INDIRECTION		(0x00008000)



/*********************************
 * The following constants are used for exp_status. 
 */ 

struct	ps_exp_type	{
  int 			type, count, 
			var_status, 
			exp_status, 
			lineno; 
  union ps_union	u; 
  struct red_type	*diagram_label; 
  struct a23tree_type	*a23tree; 
};


#ifndef _REDLIB_MCHECK_RETURN_STRUCTURES 
#define _REDLIB_MCHECK_RETURN_STRUCTURES 


/*****************************************************************
 *  
 */

struct model_check_return_type { 
  int			status; 
#define	FLAG_MODEL_CHECK_SATISFIED	1
#define	FLAG_MODEL_CHECK_UNSATISFIED	0

  redgram		initial_state_diagram, failed_state_diagram; 
  struct ps_exp_type	*neg_formula; 
}; 
  /* model_check_return_type */ 

#endif 

#endif 




#ifndef _REDLIB_BISIM_RETURN_STRUCTURES 
#define _REDLIB_BISIM_RETURN_STRUCTURES 

/*****************************************************************
 *  This part is reserved for the future research. 
 *  We have no clue how to do it now. 
 */
 
struct counter_example_sync_party_type { 
  int	proc, xi; 
}; 

struct matching_sync_xtion_type { 
  int						matching_sync_xtion_party_count; 
  struct counter_example_sync_party_type	*matching_sync_xtion_party; 
  struct counter_example_tree_node_type		*poststate; 
}; 

struct counter_example_tree_node_type { 
  int						iteration_index, 
						status;  
  struct red_type 				*prestate; 
/*  Flag values FLAG_EC_SPEC and FLAG_EC_MODL specify that 
 *  whose this is a transition. 
 */ 
  int                                       	sync_xtion_party_count; 
  struct counter_example_sync_party_type	*sync_xtion_party; 
  int                                      	matching_sync_xtion_count; 
  struct matching_sync_xtion_type		*matching_sync_xtion; 
}; 

struct sim_check_return_type { 
  int					status, 
#define MASK_SIM_CHECK_RETURN                 (0xF) 
#define FLAG_RESULT_EARLY_REACHED             1
#define FLAG_RESULT_FULL_FIXPOINT             2 
#define FLAG_RESULT_DEPTH_BOUND_FINISHED      4 

#define FLAG_COUNTER_EXAMPLE_GENERATED        (0x10)
#define FLAG_COUNTER_EXAMPLE_NOT_GENERATED    (0x00) 

#define MASK_SIMULATION_RESULT		      (0xF00) 
#define MASK_BISIMULATION_RESULT	      (0xF00) 
#define	FLAG_NO_SIMULATION                    0
#define	FLAG_NO_BISIMULATION                  0

#define	FLAG_SIMULATION_INCONCLUSIVE          (0x100)
#define	FLAG_BISIMULATION_INCONCLUSIVE        (0x100)

#define FLAG_SIMULATION_EXISTS                (0x200)
#define	FLAG_BISIMULATION_EXISTS              (0x200) 

#define MASK_SIM_ZENO_CYCLE                   (0xF00000)
#define	FLAG_SIM_USE_PLAIN_NONZENO            (0x000000)
#define FLAG_SIM_USE_APPROX_NONZENO           (0x100000)
#define FLAG_SIM_ZENO_CYCLE_OK                (0x200000)

#define MASK_BISIM_ZENO_CYCLE                 (0xF00000)
#define	FLAG_BISIM_USE_PLAIN_NONZENO          (0x000000)
#define FLAG_BISIM_USE_APPROX_NONZENO         (0x100000)
#define FLAG_BISIM_ZENO_CYCLE_OK              (0x200000)

  					iteration_count; 
  redgram				initial_state_pair_diagram, 
					final_sim_relation_diagram; 

#define	bisim_relation_diagram	sim_relation_diagram 

  redgram				*iteratively_removed_diagram; 
  struct counter_example_tree_node_type	*counter_example_tree; 
}; 
  /* bisim_check_return_type */ 


#endif 

#endif 


#ifndef string_var 

extern int	leng_string_var( 
  char		*head,  
  char		*tail, 
  char		*f, 
  va_list	args 
); 
extern char	*string_var_given_length(
  int		real_len,  
  char		*head, 
  char		*tail, 
  char		*f,  
  va_list	args 
);  

// Note that result is used as a name, instead of as an address. 
#define	string_var(result, head, tail, f, args)\
{ \
  va_start(args, f); \
  result = string_var_given_length( \
    leng_string_var(head, tail, f, args), \
    head, tail, f, args \
  ); \
  va_end(args); \
} \
  /*string_var() */ \


// Note that result1 and result2 is used as names, instead of as addresses. 
#define	string_2var(result1, result2, f1, f2, args)\
{ \
  va_start(args, f2); \
  result1 = string_var_given_length( \
    leng_string_var("", "", f1, args), "", "", f1, args \
  ); \
  result2 = string_var_given_length( \
    leng_string_var("", "", f2, args), "", "", f2, args \
  ); \
  va_end(args); \
} \
  /*string_2var() */ \


#endif 










