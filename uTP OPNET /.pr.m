MIL_3_Tfile_Hdr_ 171A 171A modeler 9 55118B16 55118B16 1 FTNK-B0013 s146147 0 0 none none 0 0 none 32C01196 A0C 0 0 0 0 0 0 2b74 2                                                                                                                                                                                                                                                                                                                                                                                              ��g�      D   H   L      �  �  �  �  �         �  �           	   begsim intrpt         
   ����   
   doc file            	nd_module      endsim intrpt             ����      failure intrpts            disabled      intrpt interval         ԲI�%��}����      priority              ����      recovery intrpts            disabled      subqueue                     count    ���   
   ����   
      list   	���   
          
      super priority             ����             #Stathandle	\requests_received_hndl;       double	\requests_received;          Packet*		pkptr;      /* Macros */   7#define PowerUp (op_intrpt_type() == OPC_INTRPT_BEGSIM)                                                      �   �          
   INIT   
       
      D/* Register the statistics that will be maintained by this model.	*/       rrequests_received_hndl   = op_stat_reg ("Sink.Requests Received (packets)",		OPC_STAT_INDEX_NONE, OPC_STAT_LOCAL);       
       
      requests_received = 0;   
       
   ����   
          pr_state        �   �          
   SINK   
       
       
       
   	   !/* Obtain the incoming packet.	*/   &pkptr = op_pk_get (op_intrpt_strm ());       !/* Update local statistics.				*/   Qop_stat_write (requests_received_hndl, requests_received=requests_received +1.0);   >printf("\n NODE %d received request, sinking", op_id_self ());       "/* Destroy the received packet.	*/   op_pk_destroy (pkptr);   
           ����             pr_state                          �      �   �  p   �          
   tr_0   
       
   PowerUp   
       ����          
    ����   
          ����                       pr_transition              �   �     �   �  '   �  #  !  �   �          
   tr_1   
       
����   
       ����          
    ����   
          ����                       pr_transition                   Requests Received (packets)           Sink   normal����        ԲI�%��}                            