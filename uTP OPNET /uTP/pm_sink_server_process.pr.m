MIL_3_Tfile_Hdr_ 171A 171A modeler 9 55118A96 5537924D C FTNK-B0013 s146147 0 0 none none 0 0 none 5D9462EF E19 0 0 0 0 0 0 2b74 2                                                                                                                                                                                                                                                                                                                                                                                              ��g�      D   H   L      �  p  t  �          �  �           	   begsim intrpt         
   ����   
   doc file            	nd_module      endsim intrpt             ����      failure intrpts            disabled      intrpt interval         ԲI�%��}����      priority              ����      recovery intrpts            disabled      subqueue                     count    ���   
   ����   
      list   	���   
          
      super priority             ����             "Stathandle	\packets_received_hndl;       double	\packets_received;       Stathandle	\packets_delay_hndl;       double	\requests_sent;       double	\req_rate;       int	\own_id;       Stathandle	\requests_sent_hndl;          Packet*		pkptr;      /* Macros */   7#define PowerUp (op_intrpt_type() == OPC_INTRPT_BEGSIM)   <#define PacketArrival (op_intrpt_type() == OPC_INTRPT_STRM)                                                   �   �          
   INIT   
       
      D/* Register the statistics that will be maintained by this model.	*/       ppackets_received_hndl   = op_stat_reg ("Sink.Packets Received (packets)",		OPC_STAT_INDEX_NONE, OPC_STAT_LOCAL);   fpackets_delay_hndl   = op_stat_reg ("Sink.Packets Delay (sec)",		OPC_STAT_INDEX_NONE, OPC_STAT_LOCAL);       /* Generation.											*/       9/* Obtain the object id of the surrounding module.					*/   (own_id = op_id_self (); //state variable           
       
      packets_received = 0;   
       
   ����   
          pr_state        �   �          
   APP   
       
       
       J       J           ����             pr_state        �  J          
   SINK   
       
      "double		ete_delay, pk_create_time;   !/* Obtain the incoming packet.	*/   &pkptr = op_pk_get (op_intrpt_strm ());       '/* Calculate metrics to be updated.		*/   1pk_create_time = op_pk_creation_time_get (pkptr);   ,ete_delay = op_sim_time () - pk_create_time;       !/* Update local statistics.				*/       Nop_stat_write (packets_received_hndl, packets_received=packets_received +1.0);   0op_stat_write (packets_delay_hndl, 		ete_delay);   9//printf("\n NODE %d sinked data packet", op_id_self ());       "/* Destroy the received packet.	*/   op_pk_destroy (pkptr);   
                     
   ����   
          pr_state                          �      �   �  p   �          
   tr_0   
       
   PowerUp   
       ����          
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
          ����                       pr_transition                   Packets Received (packets)           Sink��������        ԲI�%��}   Packets Delay (sec)           Sink��������        ԲI�%��}                            