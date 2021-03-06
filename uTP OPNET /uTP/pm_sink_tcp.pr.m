MIL_3_Tfile_Hdr_ 171A 171A modeler 9 55118C92 555DDC5A 33 FTNK-B0013 s146147 0 0 none none 0 0 none 3C85EC81 1B48 0 0 0 0 0 0 2b74 2                                                                                                                                                                                                                                                                                                                                                                                            ��g�      D  #  '  �  �  �  W  [  ,  0  <  @  D  �  �      ReqRate   �������   ����       @.      ����              ����              ����           �Z             gen_intr    �������    ����            ����            ����            ����            �Z                 	   begsim intrpt         
   ����   
   doc file            	nd_module      endsim intrpt             ����      failure intrpts            disabled      intrpt interval         ԲI�%��}����      priority              ����      recovery intrpts            disabled      subqueue                     count    ���   
   ����   
      list   	���   
          
      super priority             ����             "Stathandle	\packets_received_hndl;       double	\packets_received;       Stathandle	\packets_delay_hndl;       double	\requests_sent;       double	\req_rate;       int	\own_id;       Stathandle	\requests_sent_hndl;       int	\Generate_Flag;       int	\Gen_Intrpt;          Packet*		pkptr;      /* Macros */   8#define PowerUp (op_intrpt_type() == OPC_INTRPT_BEGSIM)    9#define PowerUpIntrpt (op_intrpt_code () == SSC_POWER_UP)   <#define PacketArrival (op_intrpt_type() == OPC_INTRPT_STRM)    2#define	PckGen	(op_intrpt_code () == SSC_GENERATE)       /* Interrupt code values.			*/   #define		SSC_GENERATE			0   #define		SSC_POWER_UP			1           #/* Node configuration constants.	*/   #define		SSC_STRM_TO_LOW			0   
       void generate()   	{   	FIN(generate());   	/*Interupt for generation*/   	if(req_rate>0)   Y		op_intrpt_schedule_self(op_sim_time() + op_dist_exponential(1/req_rate), SSC_GENERATE);   	   	FOUT;   	}                                              �   �          
   INIT   
       J   !   double power_up_time;   5op_ima_sim_attr_get_dbl ("duration", &power_up_time);   D/* Register the statistics that will be maintained by this model.	*/       ppackets_received_hndl   = op_stat_reg ("Sink.Packets Received (packets)",		OPC_STAT_INDEX_NONE, OPC_STAT_LOCAL);   fpackets_delay_hndl   = op_stat_reg ("Sink.Packets Delay (sec)",		OPC_STAT_INDEX_NONE, OPC_STAT_LOCAL);       /* Generation.											*/       9/* Obtain the object id of the surrounding module.					*/   (own_id = op_id_self (); //state variable       6op_ima_obj_attr_get (own_id, "gen_intr", &Gen_Intrpt);   if(Gen_Intrpt==1)   H	op_intrpt_schedule_self(op_sim_time() + power_up_time/2, SSC_POWER_UP);   else if(Gen_Intrpt==0)   <	op_intrpt_schedule_self(op_sim_time() + 0.1, SSC_POWER_UP);       Cprintf("\n NODE %d POWER UP TIME:%f", op_id_self (),power_up_time);   9/* Read the values of the packet generation parameters	*/   3op_ima_obj_attr_get (own_id, "ReqRate", &req_rate);       D/* Register the statistics that will be maintained by this model.	*/       orequests_sent_hndl   = op_stat_reg ("Generator.Requests Sent (packets)",		OPC_STAT_INDEX_NONE, OPC_STAT_LOCAL);           /*Interupt for generation*/   if(req_rate>0)   X	op_intrpt_schedule_self(op_sim_time() + op_dist_exponential(1/req_rate), SSC_GENERATE);       Generate_Flag=0;       J       
      packets_received = 0;   requests_sent=0;   
       
   ����   
          pr_state        �   �          
   APP   
       
          
       
          
           ����             pr_state        �   Z          
   GENERATE   
       
   
   0/* Create a packet with the specified format.	*/   Packet				*pkptr;   int pck_size;       pck_size= 1400;   "pkptr = op_pk_create (pck_size*8);       $op_pk_send (pkptr, SSC_STRM_TO_LOW);   Fop_stat_write (requests_sent_hndl, requests_sent=requests_sent + 1.0);       
       
       
       
   ����   
          pr_state        �  J          
   SINK   
       
      "double		ete_delay, pk_create_time;   !/* Obtain the incoming packet.	*/   &pkptr = op_pk_get (op_intrpt_strm ());       '/* Calculate metrics to be updated.		*/   1pk_create_time = op_pk_creation_time_get (pkptr);   ,ete_delay = op_sim_time () - pk_create_time;       !/* Update local statistics.				*/       Nop_stat_write (packets_received_hndl, packets_received=packets_received +1.0);   0op_stat_write (packets_delay_hndl, 		ete_delay);   9//printf("\n NODE %d sinked data packet", op_id_self ());       "/* Destroy the received packet.	*/   op_pk_destroy (pkptr);   
                     
   ����   
          pr_state        J   Z          
   PowerUp   
       
      Generate_Flag=1;       V//printf("\n NODE %d START GENERATING Generate_Flag=%d", op_id_self (),Generate_Flag);   
                     
   ����   
          pr_state                          �      �   �  p   �          
   tr_0   
       
   PowerUp   
       ����          
    ����   
          ����                       pr_transition              �   �     �   �  �   j          
   tr_2   
       
   PckGen && Generate_Flag==1   
       ����          
    ����   
          ����                       pr_transition              �   �     �   o  �   �  �   �          
   tr_3   
       ����          
   
generate()   
       
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
          ����                       pr_transition              O   �     �   �  K   \          
   tr_8   
       
   PowerUpIntrpt   
       ����          
    ����   
          ����                       pr_transition      	        v   �     ^   h  �   �          
   tr_9   
       ����          ����          
    ����   
          ����                       pr_transition              4   �     �   �  ,   �  &   �  �   �          
   tr_14   
       
   PckGen && Generate_Flag==0   
       
   
generate()   
       
    ����   
          ����                       pr_transition                   Packets Received (packets)           Sink��������        ԲI�%��}   Packets Delay (sec)           Sink��������        ԲI�%��}   Requests Sent (packets)           	Generator��������        ԲI�%��}                            