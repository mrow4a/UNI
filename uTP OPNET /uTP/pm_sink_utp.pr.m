MIL_3_Tfile_Hdr_ 171A 171A modeler 9 555B056D 555B05B8 3 FTNK-B0013 s146147 0 0 none none 0 0 none E9907A55 1563 0 0 0 0 0 0 2b74 2                                                                                                                                                                                                                                                                                                                                                                                             ��g�      D   �   �  �  �  Q  r  v  G  K  W  [  _  A  M      ReqRate   �������   ����       @.      ����              ����              ����           �Z                 	   begsim intrpt         
   ����   
   doc file            	nd_module      endsim intrpt             ����      failure intrpts            disabled      intrpt interval         ԲI�%��}����      priority              ����      recovery intrpts            disabled      subqueue                     count    ���   
   ����   
      list   	���   
          
      super priority             ����             "Stathandle	\packets_received_hndl;       double	\packets_received;       Stathandle	\packets_delay_hndl;       double	\requests_sent;       double	\req_rate;       int	\own_id;       Stathandle	\requests_sent_hndl;          Packet*		pkptr;      /* Macros */   8#define PowerUp (op_intrpt_type() == OPC_INTRPT_BEGSIM)    <#define PacketArrival (op_intrpt_type() == OPC_INTRPT_STRM)    \#define	PckGen	(op_intrpt_code () == SSC_GENERATE && (op_intrpt_type() == OPC_INTRPT_SELF) )       /* Interrupt code values.			*/   #define		SSC_GENERATE			0   #define		SSC_POWER_UP			1           #/* Node configuration constants.	*/   #define		SSC_STRM_TO_LOW			0                                                  �   �          
   INIT   
       
      D/* Register the statistics that will be maintained by this model.	*/       ppackets_received_hndl   = op_stat_reg ("Sink.Packets Received (packets)",		OPC_STAT_INDEX_NONE, OPC_STAT_LOCAL);   fpackets_delay_hndl   = op_stat_reg ("Sink.Packets Delay (sec)",		OPC_STAT_INDEX_NONE, OPC_STAT_LOCAL);       /* Generation.											*/       9/* Obtain the object id of the surrounding module.					*/   (own_id = op_id_self (); //state variable       9/* Read the values of the packet generation parameters	*/   3op_ima_obj_attr_get (own_id, "ReqRate", &req_rate);       D/* Register the statistics that will be maintained by this model.	*/       orequests_sent_hndl   = op_stat_reg ("Generator.Requests Sent (packets)",		OPC_STAT_INDEX_NONE, OPC_STAT_LOCAL);               /*Interupt for generation*/   if(req_rate>0)   X	op_intrpt_schedule_self(op_sim_time() + op_dist_exponential(1/req_rate), SSC_GENERATE);           
       
      packets_received = 0;   requests_sent=0;   
       
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
      0/* Create a packet with the specified format.	*/   Packet				*pkptr;   int pck_size;       pck_size= 1400;   "pkptr = op_pk_create (pck_size*8);   ?//printf("\n NODE %d generated WEB GET packet", op_id_self ());       $op_pk_send (pkptr, SSC_STRM_TO_LOW);   Fop_stat_write (requests_sent_hndl, requests_sent=requests_sent + 1.0);       
       
      Wop_intrpt_schedule_self(op_sim_time() + op_dist_exponential(1/req_rate), SSC_GENERATE);   
       
   ����   
          pr_state        �  J          
   SINK   
       
      "double		ete_delay, pk_create_time;   !/* Obtain the incoming packet.	*/   &pkptr = op_pk_get (op_intrpt_strm ());       '/* Calculate metrics to be updated.		*/   1pk_create_time = op_pk_creation_time_get (pkptr);   ,ete_delay = op_sim_time () - pk_create_time;       !/* Update local statistics.				*/       Nop_stat_write (packets_received_hndl, packets_received=packets_received +1.0);   0op_stat_write (packets_delay_hndl, 		ete_delay);   9//printf("\n NODE %d sinked data packet", op_id_self ());       "/* Destroy the received packet.	*/   op_pk_destroy (pkptr);   
                     
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
          ����                       pr_transition                   Packets Received (packets)           Sink��������        ԲI�%��}   Packets Delay (sec)           Sink��������        ԲI�%��}   Requests Sent (packets)           	Generator��������        ԲI�%��}                            