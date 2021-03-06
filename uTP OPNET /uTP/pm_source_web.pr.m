MIL_3_Tfile_Hdr_ 171A 171A modeler 9 55118CA7 5533AE0C 10 FTNK-B0013 s146147 0 0 none none 0 0 none E220D433 F12 0 0 0 0 0 0 2b74 2                                                                                                                                                                                                                                                                                                                                                                                             ��g�      D  �  �  Z  ^  \  �  �  �  �    
    L  X      ReqRate   �������   ����       @.      ����              ����              ����           �Z             ReqType    �������    ����           ����          ����          ����           �Z         
       0       -       web,       1       -       music       stream,       2       -       upload       	   begsim intrpt         
   ����   
   doc file            	nd_module      endsim intrpt             ����      failure intrpts            disabled      intrpt interval         ԲI�%��}����      priority              ����      recovery intrpts            disabled      subqueue                     count    ���   
   ����   
      list   	���   
          
      super priority             ����             Objid	\own_id;       Stathandle	\requests_sent_hndl;       double	\req_rate;       double	\requests_sent;          +/* Variables used in state transitions.		*/   int			intrpt_code;      /* Macros */   7#define PowerUp (op_intrpt_type() == OPC_INTRPT_BEGSIM)   ,#define	PckGen	(intrpt_code == SSC_GENERATE)       /* Interrupt code values.			*/   #define		SSC_GENERATE			0           #/* Node configuration constants.	*/   #define		SSC_STRM_TO_LOW			0                                                      �   �          
   INIT   
       
      /* Generation.											*/       9/* Obtain the object id of the surrounding module.					*/   (own_id = op_id_self (); //state variable       9/* Read the values of the packet generation parameters	*/   3op_ima_obj_attr_get (own_id, "ReqRate", &req_rate);       D/* Register the statistics that will be maintained by this model.	*/       orequests_sent_hndl   = op_stat_reg ("Generator.Requests Sent (packets)",		OPC_STAT_INDEX_NONE, OPC_STAT_LOCAL);       
       
      requests_sent=0;   
       
   ����   
          pr_state        �   �          
   GENERATE   
       
      /*Interupt for generation*/   if (req_rate <0)   	{   	req_rate = 15.0;   	}       Uop_intrpt_schedule_self(op_sim_time() + op_dist_exponential(req_rate), SSC_GENERATE);   
       J      0/* Create a packet with the specified format.	*/   Packet				*pkptr;   int pck_size;   do   	{   $	pck_size= op_dist_exponential(600);   '	}while(pck_size>1400 || pck_size<300);   "pkptr = op_pk_create (pck_size*8);   ?//printf("\n NODE %d generated WEB GET packet", op_id_self ());       $op_pk_send (pkptr, SSC_STRM_TO_LOW);   Fop_stat_write (requests_sent_hndl, requests_sent=requests_sent + 1.0);       F/* Determine the code of the interrupt, which is used in evaluating	*/   +/* state transition conditions.										*/    intrpt_code = op_intrpt_code ();       J           ����             pr_state                          �      �   �  p   �          
   tr_0   
       
   PowerUp   
       ����          
    ����   
          ����                       pr_transition              �   �     �   �      �    .  �   �          
   tr_1   
       
   PckGen   
       ����          
    ����   
          ����                       pr_transition                   Requests Sent (packets)           	Generator   normal����        ԲI�%��}                            