MIL_3_Tfile_Hdr_ 171A 171A modeler 9 5548884C 556517F7 22 FTNK-B0013 s146147 0 0 none none 0 0 none CBC41AF3 1E21 0 0 0 0 0 0 2b74 2                                                                                                                                                                                                                                                                                                                                                                                            ��g�      D   �   �  �  �  �  �  �    	        �  �      ReqRate   �������   ����       @.      ����              ����              ����           �Z                 	   begsim intrpt         
   ����   
   doc file            	nd_module      endsim intrpt             ����      failure intrpts            disabled      intrpt interval         ԲI�%��}����      priority              ����      recovery intrpts            disabled      subqueue                     count    ���   
   ����   
      list   	���   
          
      super priority             ����             "Stathandle	\packets_received_hndl;       double	\packets_received;       Stathandle	\packets_delay_hndl;       double	\requests_sent;       double	\req_rate;       int	\own_id;       Stathandle	\requests_sent_hndl;       Stathandle	\web_delay_hndl;       double	\request_time;       List*	\WebList;       #Stathandle	\web_delay_counter_hndl;       int	\web_delay_counter;          Packet*		pkptr;      /* Macros */   7#define PowerUp (op_intrpt_type() == OPC_INTRPT_BEGSIM)   <#define PacketArrival (op_intrpt_type() == OPC_INTRPT_STRM)    Y#define	PckGen	(op_intrpt_code () == SSC_GENERATE && op_intrpt_type() == OPC_INTRPT_SELF)       /* Interrupt code values.			*/   #define		SSC_GENERATE			0           #/* Node configuration constants.	*/   #define		SSC_STRM_TO_LOW			0                                                  �   �          
   INIT   
       J      D/* Register the statistics that will be maintained by this model.	*/       ppackets_received_hndl   = op_stat_reg ("Sink.Packets Received (packets)",		OPC_STAT_INDEX_NONE, OPC_STAT_LOCAL);   fpackets_delay_hndl   = op_stat_reg ("Sink.Packets Delay (sec)",		OPC_STAT_INDEX_NONE, OPC_STAT_LOCAL);   ^web_delay_hndl   = op_stat_reg ("Sink.Web Delay (sec)",		OPC_STAT_INDEX_NONE, OPC_STAT_LOCAL);   mweb_delay_counter_hndl   = op_stat_reg ("Sink.Web Delay Over 1s(sec)",		OPC_STAT_INDEX_NONE, OPC_STAT_LOCAL);       /* Generation.											*/       9/* Obtain the object id of the surrounding module.					*/   (own_id = op_id_self (); //state variable       9/* Read the values of the packet generation parameters	*/   3op_ima_obj_attr_get (own_id, "ReqRate", &req_rate);       D/* Register the statistics that will be maintained by this model.	*/       orequests_sent_hndl   = op_stat_reg ("Generator.Requests Sent (packets)",		OPC_STAT_INDEX_NONE, OPC_STAT_LOCAL);       /*Interupt for generation*/       if(req_rate>0)   X	op_intrpt_schedule_self(op_sim_time() + op_dist_exponential(1/req_rate), SSC_GENERATE);           WebList= op_prg_list_create();   op_prg_list_init(WebList);   J       J      packets_received = 0;   requests_sent=0;   web_delay_counter=0;   J       
   ����   
          pr_state        �   �          
   APP   
       
       
       
          
           ����             pr_state        �   Z          
   GENERATE   
       
      0/* Create a packet with the specified format.	*/   Packet				*pkptr;   double *double_ptr;   int pck_size,i;   for (i = 0; i < 4; i++)       {             		do   				{   (					pck_size= op_dist_exponential(550);   *				}while(pck_size>1400 || pck_size<300);   &				pkptr = op_pk_create (pck_size*8);   				   (				op_pk_send (pkptr, SSC_STRM_TO_LOW);   J				op_stat_write (requests_sent_hndl, requests_sent=requests_sent + 1.0);        		   	}   M//printf("\n NODE %d generated WEB GET packet",op_topo_parent(op_id_self()));   request_time=op_sim_time();   9double_ptr = (double *)op_prg_mem_alloc(sizeof(double));    *double_ptr = request_time;   :op_prg_list_insert(WebList, double_ptr, OPC_LISTPOS_TAIL);       
       
      Wop_intrpt_schedule_self(op_sim_time() + op_dist_exponential(1/req_rate), SSC_GENERATE);   
       
   ����   
          pr_state        �  J          
   SINK   
       J   3   -double		ete_delay, pk_create_time, web_delay;   double *double_ptr;   int length, range,pck_size,i;   Packet* app_data;   !/* Obtain the incoming packet.	*/   &pkptr = op_pk_get (op_intrpt_strm ());       (op_pk_nfd_get(pkptr, "length", &length);   &op_pk_nfd_get(pkptr, "range", &range);   3op_pk_nfd_get_pkt (pkptr, "data_field", &app_data);   '/* Calculate metrics to be updated.		*/   1pk_create_time = op_pk_creation_time_get (pkptr);   ,ete_delay = op_sim_time () - pk_create_time;       !/* Update local statistics.				*/       Nop_stat_write (packets_received_hndl, packets_received=packets_received +1.0);   0op_stat_write (packets_delay_hndl, 		ete_delay);       ]//printf("\n NODE %d sinked response packet length=%d,range=%d", op_id_self (),length,range);       op_pk_destroy (pkptr);   if(range==length )   	{   F	double_ptr = (double *)op_prg_list_access(WebList, OPC_LISTPOS_HEAD);   	request_time=*double_ptr;   '	web_delay=op_sim_time ()-request_time;   @	op_prg_mem_free(op_prg_list_remove(WebList, OPC_LISTPOS_HEAD));   	if(web_delay>=1)   P		op_stat_write (web_delay_counter_hndl, web_delay_counter=web_delay_counter+1);   	   +	op_stat_write (web_delay_hndl, web_delay);   F	printf("\n NODE %d DELIVERED WEBSITE", op_topo_parent(op_id_self()));   	op_prg_odb_bkpt("DEL_WEB");   	}   ;else if(length==3 || length==7 || length==11 || length==15)   	{   	for (i = 0; i < 4; i++)        		{             		do   				{   (					pck_size= op_dist_exponential(550);   *				}while(pck_size>1400 || pck_size<300);   &				pkptr = op_pk_create (pck_size*8);   Q				//printf("\n NODE %d generated WEB GET packet",op_topo_parent(op_id_self()));   (				op_pk_send (pkptr, SSC_STRM_TO_LOW);   J				op_stat_write (requests_sent_hndl, requests_sent=requests_sent + 1.0);        		}   	}   "/* Destroy the received packet.	*/   op_pk_destroy (app_data);   J                     
   ����   
          pr_state                          �      �   �  p   �          
   tr_0   
       
   PowerUp   
       ����          
    ����   
          ����                       pr_transition              �   �     �   �  �   j          
   tr_2   
       
   PckGen   
       ����          
    ����   
          ����                       pr_transition              �   �     �   o  �   �  �   �          
   tr_3   
       ����          ����          
    ����   
          ����                       pr_transition              �       �   �  �  C          
   tr_4   
       
   PacketArrival   
       ����          
    ����   
          ����                       pr_transition              �  	     �  6  �   �          
   tr_5   
       ����          ����          
    ����   
          ����                       pr_transition                   Packets Received (packets)           Sink��������        ԲI�%��}   Packets Delay (sec)           Sink��������        ԲI�%��}   Requests Sent (packets)           	Generator��������        ԲI�%��}   Web Delay (sec)           Sink��������        ԲI�%��}   Web Delay Over 1s(sec)           Sink��������        ԲI�%��}                            