MIL_3_Tfile_Hdr_ 171A 171A modeler 9 55488D58 555D2A29 1B FTNK-B0013 s146147 0 0 none none 0 0 none B5E6F93 1287 0 0 0 0 0 0 2b74 2                                                                                                                                                                                                                                                                                                                                                                                             ��g�      D   H   L      �  �  �  k  o  {    �  �  �           	   begsim intrpt         
   ����   
   doc file            	nd_module      endsim intrpt             ����      failure intrpts            disabled      intrpt interval         ԲI�%��}����      priority              ����      recovery intrpts            disabled      subqueue                     count    ���   
   ����   
      list   	���   
          
      super priority             ����             "Stathandle	\packets_received_hndl;       double	\packets_received;       Stathandle	\packets_delay_hndl;       double	\req_rate;       int	\own_id;       Stathandle	\response_sent_hndl;       double	\response_sent;       int	\range;       int	\length;          Packet*		pkptr;      /* Macros */   7#define PowerUp (op_intrpt_type() == OPC_INTRPT_BEGSIM)   <#define PacketArrival (op_intrpt_type() == OPC_INTRPT_STRM)                                                   �   �          
   INIT   
       
      D/* Register the statistics that will be maintained by this model.	*/       ppackets_received_hndl   = op_stat_reg ("Sink.Packets Received (packets)",		OPC_STAT_INDEX_NONE, OPC_STAT_LOCAL);   fpackets_delay_hndl   = op_stat_reg ("Sink.Packets Delay (sec)",		OPC_STAT_INDEX_NONE, OPC_STAT_LOCAL);   hresponse_sent_hndl = op_stat_reg ("Sink.Response send (packets)",		OPC_STAT_INDEX_NONE, OPC_STAT_LOCAL);   /* Generation.											*/       9/* Obtain the object id of the surrounding module.					*/   (own_id = op_id_self (); //state variable       	length=0;   
       
      packets_received = 0;   
       
   ����   
          pr_state        �   �          
   APP   
       
       
       
       
           ����             pr_state        �  J          
   SINK   
       
      "double		ete_delay, pk_create_time;   !/* Obtain the incoming packet.	*/   &pkptr = op_pk_get (op_intrpt_strm ());       '/* Calculate metrics to be updated.		*/   1pk_create_time = op_pk_creation_time_get (pkptr);   ,ete_delay = op_sim_time () - pk_create_time;       !/* Update local statistics.				*/       Nop_stat_write (packets_received_hndl, packets_received=packets_received +1.0);   0op_stat_write (packets_delay_hndl, 		ete_delay);   9//printf("\n NODE %d sinked data packet", op_id_self ());       "/* Destroy the received packet.	*/   op_pk_destroy (pkptr);   
       J      0/* Create a packet with the specified format.	*/   Packet* pkptr;   Packet* http_packet_ptr;   int pck_size;       do   	{   %	pck_size= op_dist_exponential(1000);   '	}while(pck_size>1400 || pck_size<300);       Y//printf("\n NODE %d generated WEB REQUEST Packets of size: %d", op_id_self (),pck_size);   	range=19;       6http_packet_ptr = op_pk_create_fmt ("pm_http_packet");   	   "pkptr = op_pk_create (pck_size*8);   9op_pk_nfd_set_pkt (http_packet_ptr, "data_field", pkptr);   	   1op_pk_nfd_set(http_packet_ptr, "length", length);   /op_pk_nfd_set(http_packet_ptr, "range", range);    op_pk_send (http_packet_ptr, 0);   a//printf("\n NODE %d send WEB RESPONSE Packets length %d, range %d", op_id_self (),length,range);   if(length==range)   
	length=0;   else    
	length++;   op_prg_odb_bkpt("STOP");       Top_stat_write (response_sent_hndl, response_sent=response_sent+(double)range + 1.0);   J       
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
          ����                       pr_transition                   Packets Received (packets)           Sink��������        ԲI�%��}   Packets Delay (sec)           Sink��������        ԲI�%��}   Response send (packets)           Sink��������        ԲI�%��}                            