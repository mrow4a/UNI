/* Process model C form file: pm_sink_server_process.pr.c */
/* Portions of this file copyright 1986-2012 by OPNET Technologies, Inc. */


/*
 =========================== NOTE ==========================
 This file is automatically generated from pm_sink_server_process.pr.m
 during a process model compilation.

 Do NOT manually edit this file.
 Manual edits will be lost during the next compilation.
 =========================== NOTE ==========================
*/



/* This variable carries the header into the object file */
const char pm_sink_server_process_pr_c [] = "MIL_3_Tfile_Hdr_ 171A 30A op_runsim_dev 7 555CEE63 555CEE63 1 FTNK-B0013 s146147 0 0 none none 0 0 none 0 0 0 0 0 0 0 0 2b74 2                                                                                                                                                                                                                                                                                                                                                                                                 ";
#include <string.h>



/* OPNET system definitions */
#include <opnet.h>



/* Header Block */

/* Macros */
#define PowerUp (op_intrpt_type() == OPC_INTRPT_BEGSIM)
#define PacketArrival (op_intrpt_type() == OPC_INTRPT_STRM) 

/* End of Header Block */

#if !defined (VOSD_NO_FIN)
#undef	BIN
#undef	BOUT
#define	BIN		FIN_LOCAL_FIELD(_op_last_line_passed) = __LINE__ - _op_block_origin;
#define	BOUT	BIN
#define	BINIT	FIN_LOCAL_FIELD(_op_last_line_passed) = 0; _op_block_origin = __LINE__;
#else
#define	BINIT
#endif /* #if !defined (VOSD_NO_FIN) */



/* State variable definitions */
typedef struct
	{
	/* Internal state tracking for FSM */
	FSM_SYS_STATE
	/* State Variables */
	Stathandle	             		packets_received_hndl                           ;
	double	                 		packets_received                                ;
	Stathandle	             		packets_delay_hndl                              ;
	double	                 		requests_sent                                   ;
	double	                 		req_rate                                        ;
	int	                    		own_id                                          ;
	Stathandle	             		requests_sent_hndl                              ;
	} pm_sink_server_process_state;

#define packets_received_hndl   		op_sv_ptr->packets_received_hndl
#define packets_received        		op_sv_ptr->packets_received
#define packets_delay_hndl      		op_sv_ptr->packets_delay_hndl
#define requests_sent           		op_sv_ptr->requests_sent
#define req_rate                		op_sv_ptr->req_rate
#define own_id                  		op_sv_ptr->own_id
#define requests_sent_hndl      		op_sv_ptr->requests_sent_hndl

/* These macro definitions will define a local variable called	*/
/* "op_sv_ptr" in each function containing a FIN statement.	*/
/* This variable points to the state variable data structure,	*/
/* and can be used from a C debugger to display their values.	*/
#undef FIN_PREAMBLE_DEC
#undef FIN_PREAMBLE_CODE
#define FIN_PREAMBLE_DEC	pm_sink_server_process_state *op_sv_ptr;
#define FIN_PREAMBLE_CODE	\
		op_sv_ptr = ((pm_sink_server_process_state *)(OP_SIM_CONTEXT_PTR->_op_mod_state_ptr));


/* No Function Block */

#if !defined (VOSD_NO_FIN)
enum { _op_block_origin = __LINE__ };
#endif

/* Undefine optional tracing in FIN/FOUT/FRET */
/* The FSM has its own tracing code and the other */
/* functions should not have any tracing.		  */
#undef FIN_TRACING
#define FIN_TRACING

#undef FOUTRET_TRACING
#define FOUTRET_TRACING

#if defined (__cplusplus)
extern "C" {
#endif
	void pm_sink_server_process (OP_SIM_CONTEXT_ARG_OPT);
	VosT_Obtype _op_pm_sink_server_process_init (int * init_block_ptr);
	void _op_pm_sink_server_process_diag (OP_SIM_CONTEXT_ARG_OPT);
	void _op_pm_sink_server_process_terminate (OP_SIM_CONTEXT_ARG_OPT);
	VosT_Address _op_pm_sink_server_process_alloc (VosT_Obtype, int);
	void _op_pm_sink_server_process_svar (void *, const char *, void **);


#if defined (__cplusplus)
} /* end of 'extern "C"' */
#endif




/* Process model interrupt handling procedure */


void
pm_sink_server_process (OP_SIM_CONTEXT_ARG_OPT)
	{
#if !defined (VOSD_NO_FIN)
	int _op_block_origin = 0;
#endif
	FIN_MT (pm_sink_server_process ());

		{
		/* Temporary Variables */
		Packet*		pkptr;
		/* End of Temporary Variables */


		FSM_ENTER_NO_VARS ("pm_sink_server_process")

		FSM_BLOCK_SWITCH
			{
			/*---------------------------------------------------------*/
			/** state (INIT) enter executives **/
			FSM_STATE_ENTER_FORCED_NOLABEL (0, "INIT", "pm_sink_server_process [INIT enter execs]")
				FSM_PROFILE_SECTION_IN ("pm_sink_server_process [INIT enter execs]", state0_enter_exec)
				{
				/* Register the statistics that will be maintained by this model.	*/
				
				packets_received_hndl   = op_stat_reg ("Sink.Packets Received (packets)",		OPC_STAT_INDEX_NONE, OPC_STAT_LOCAL);
				packets_delay_hndl   = op_stat_reg ("Sink.Packets Delay (sec)",		OPC_STAT_INDEX_NONE, OPC_STAT_LOCAL);
				
				/* Generation.											*/
				
				/* Obtain the object id of the surrounding module.					*/
				own_id = op_id_self (); //state variable
				
				
				}
				FSM_PROFILE_SECTION_OUT (state0_enter_exec)

			/** state (INIT) exit executives **/
			FSM_STATE_EXIT_FORCED (0, "INIT", "pm_sink_server_process [INIT exit execs]")
				FSM_PROFILE_SECTION_IN ("pm_sink_server_process [INIT exit execs]", state0_exit_exec)
				{
				packets_received = 0;
				}
				FSM_PROFILE_SECTION_OUT (state0_exit_exec)


			/** state (INIT) transition processing **/
			FSM_TRANSIT_ONLY ((PowerUp), 1, state1_enter_exec, ;, INIT, "PowerUp", "", "INIT", "APP", "tr_0", "pm_sink_server_process [INIT -> APP : PowerUp / ]")
				/*---------------------------------------------------------*/



			/** state (APP) enter executives **/
			FSM_STATE_ENTER_UNFORCED (1, "APP", state1_enter_exec, "pm_sink_server_process [APP enter execs]")

			/** blocking after enter executives of unforced state. **/
			FSM_EXIT (3,"pm_sink_server_process")


			/** state (APP) exit executives **/
			FSM_STATE_EXIT_UNFORCED (1, "APP", "pm_sink_server_process [APP exit execs]")


			/** state (APP) transition processing **/
			FSM_TRANSIT_ONLY ((PacketArrival), 2, state2_enter_exec, ;, APP, "PacketArrival", "", "APP", "SINK", "tr_4", "pm_sink_server_process [APP -> SINK : PacketArrival / ]")
				/*---------------------------------------------------------*/



			/** state (SINK) enter executives **/
			FSM_STATE_ENTER_FORCED (2, "SINK", state2_enter_exec, "pm_sink_server_process [SINK enter execs]")
				FSM_PROFILE_SECTION_IN ("pm_sink_server_process [SINK enter execs]", state2_enter_exec)
				{
				double		ete_delay, pk_create_time;
				/* Obtain the incoming packet.	*/
				pkptr = op_pk_get (op_intrpt_strm ());
				
				/* Calculate metrics to be updated.		*/
				pk_create_time = op_pk_creation_time_get (pkptr);
				ete_delay = op_sim_time () - pk_create_time;
				
				/* Update local statistics.				*/
				
				op_stat_write (packets_received_hndl, packets_received=packets_received +1.0);
				op_stat_write (packets_delay_hndl, 		ete_delay);
				//printf("\n NODE %d sinked data packet", op_id_self ());
				
				/* Destroy the received packet.	*/
				op_pk_destroy (pkptr);
				}
				FSM_PROFILE_SECTION_OUT (state2_enter_exec)

			/** state (SINK) exit executives **/
			FSM_STATE_EXIT_FORCED (2, "SINK", "pm_sink_server_process [SINK exit execs]")


			/** state (SINK) transition processing **/
			FSM_TRANSIT_FORCE (1, state1_enter_exec, ;, "default", "", "SINK", "APP", "tr_5", "pm_sink_server_process [SINK -> APP : default / ]")
				/*---------------------------------------------------------*/



			}


		FSM_EXIT (0,"pm_sink_server_process")
		}
	}




void
_op_pm_sink_server_process_diag (OP_SIM_CONTEXT_ARG_OPT)
	{
	/* No Diagnostic Block */
	}




void
_op_pm_sink_server_process_terminate (OP_SIM_CONTEXT_ARG_OPT)
	{

	FIN_MT (_op_pm_sink_server_process_terminate ())


	/* No Termination Block */

	Vos_Poolmem_Dealloc (op_sv_ptr);

	FOUT
	}


/* Undefine shortcuts to state variables to avoid */
/* syntax error in direct access to fields of */
/* local variable prs_ptr in _op_pm_sink_server_process_svar function. */
#undef packets_received_hndl
#undef packets_received
#undef packets_delay_hndl
#undef requests_sent
#undef req_rate
#undef own_id
#undef requests_sent_hndl

#undef FIN_PREAMBLE_DEC
#undef FIN_PREAMBLE_CODE

#define FIN_PREAMBLE_DEC
#define FIN_PREAMBLE_CODE

VosT_Obtype
_op_pm_sink_server_process_init (int * init_block_ptr)
	{
	VosT_Obtype obtype = OPC_NIL;
	FIN_MT (_op_pm_sink_server_process_init (init_block_ptr))

	obtype = Vos_Define_Object_Prstate ("proc state vars (pm_sink_server_process)",
		sizeof (pm_sink_server_process_state));
	*init_block_ptr = 0;

	FRET (obtype)
	}

VosT_Address
_op_pm_sink_server_process_alloc (VosT_Obtype obtype, int init_block)
	{
#if !defined (VOSD_NO_FIN)
	int _op_block_origin = 0;
#endif
	pm_sink_server_process_state * ptr;
	FIN_MT (_op_pm_sink_server_process_alloc (obtype))

	ptr = (pm_sink_server_process_state *)Vos_Alloc_Object (obtype);
	if (ptr != OPC_NIL)
		{
		ptr->_op_current_block = init_block;
#if defined (OPD_ALLOW_ODB)
		ptr->_op_current_state = "pm_sink_server_process [INIT enter execs]";
#endif
		}
	FRET ((VosT_Address)ptr)
	}



void
_op_pm_sink_server_process_svar (void * gen_ptr, const char * var_name, void ** var_p_ptr)
	{
	pm_sink_server_process_state		*prs_ptr;

	FIN_MT (_op_pm_sink_server_process_svar (gen_ptr, var_name, var_p_ptr))

	if (var_name == OPC_NIL)
		{
		*var_p_ptr = (void *)OPC_NIL;
		FOUT
		}
	prs_ptr = (pm_sink_server_process_state *)gen_ptr;

	if (strcmp ("packets_received_hndl" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->packets_received_hndl);
		FOUT
		}
	if (strcmp ("packets_received" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->packets_received);
		FOUT
		}
	if (strcmp ("packets_delay_hndl" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->packets_delay_hndl);
		FOUT
		}
	if (strcmp ("requests_sent" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->requests_sent);
		FOUT
		}
	if (strcmp ("req_rate" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->req_rate);
		FOUT
		}
	if (strcmp ("own_id" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->own_id);
		FOUT
		}
	if (strcmp ("requests_sent_hndl" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->requests_sent_hndl);
		FOUT
		}
	*var_p_ptr = (void *)OPC_NIL;

	FOUT
	}

