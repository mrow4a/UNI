MIL_3_Tfile_Hdr_ 171A 171A modeler 9 5510913E 552C03CD 13 FTNK-B0013 s146147 0 0 none none 0 0 none 84F7754A 1991 0 0 0 0 0 0 2b74 2                                                                                                                                                                                                                                                                                                                                                                                            ��g�      D   H   L      #  m  q  u  y  �  �  �               	   begsim intrpt         
   ����   
   doc file            	nd_module      endsim intrpt             ����      failure intrpts            disabled      intrpt interval         ԲI�%��}����      priority              ����      recovery intrpts            disabled      subqueue                     count    ���   
   ����   
      list   	���   
          
      super priority             ����          
   int	\PckStream;       int	\SinkStream;       int	\TransStream;       int	\RecNextPckSeq;       Boolean	\DataPktArr;          int seq, type, src, dst;   Packet* pkptr;   Packet* ackptr;   Packet* tcp_packet_ptr;   Packet* ip_packet_ptr;      8#define PowerUp (op_intrpt_type() == OPC_INTRPT_BEGSIM)    [#define PacketArrival (op_intrpt_type() == OPC_INTRPT_STRM && op_intrpt_strm()==PckStream)    C#define DataPacketArrival (op_pk_total_size_get(tcp_packet_ptr)!=1)                                                 Z   �          
   INIT   
       
   >   "/*ID of the connections Incoming*/   Objid MyID, StrmID, OtherID;    int index, InStreams,OutStreams;   char ModuleName[64];   MyID = op_id_self();           KInStreams = op_topo_assoc_count(MyID, OPC_TOPO_ASSOC_IN, OPC_OBJTYPE_STRM);   MOutStreams = op_topo_assoc_count(MyID, OPC_TOPO_ASSOC_OUT, OPC_OBJTYPE_STRM);   '/*Find the Packet and AppData Streams*/       +for (index = 0; index < InStreams; index++)   {   6	/*Find the stream index at the specific association*/   J	StrmID = op_topo_assoc(MyID, OPC_TOPO_ASSOC_IN, OPC_OBJTYPE_STRM, index);   	   H	if(op_topo_assoc_count(StrmID, OPC_TOPO_ASSOC_IN, OPC_OBJTYPE_PTRX)!=0)   	{   8		/*Find the process index at the specific association*/   I		OtherID = op_topo_assoc(StrmID, OPC_TOPO_ASSOC_IN, OPC_OBJTYPE_PTRX,0);   ;		/*Access the attribute name and find the proper streams*/   3		op_ima_obj_attr_get(OtherID, "name", ModuleName);   $		if (strcmp(ModuleName, "RX") == 0)   			{   ;				op_ima_obj_attr_get(StrmID, "dest stream", &PckStream);   			}   	}   }       ,for (index = 0; index < OutStreams; index++)   {   6	/*Find the stream index at the specific association*/   K	StrmID = op_topo_assoc(MyID, OPC_TOPO_ASSOC_OUT, OPC_OBJTYPE_STRM, index);   	   I	if(op_topo_assoc_count(StrmID, OPC_TOPO_ASSOC_OUT, OPC_OBJTYPE_PROC)!=0)   	{   8		/*Find the process index at the specific association*/   J		OtherID = op_topo_assoc(StrmID, OPC_TOPO_ASSOC_OUT, OPC_OBJTYPE_PROC,0);   ;		/*Access the attribute name and find the proper streams*/   3		op_ima_obj_attr_get(OtherID, "name", ModuleName);   .		if (strcmp(ModuleName, "Traffic Sink") == 0)   			{   ;				op_ima_obj_attr_get(StrmID, "src stream", &SinkStream);   *				printf("SinkStream: %d\n",SinkStream);   			}   	}   N	else if(op_topo_assoc_count(StrmID, OPC_TOPO_ASSOC_OUT, OPC_OBJTYPE_PTTX)!=0)   	{   8		/*Find the process index at the specific association*/   J		OtherID = op_topo_assoc(StrmID, OPC_TOPO_ASSOC_OUT, OPC_OBJTYPE_PTTX,0);   ;		/*Access the attribute name and find the proper streams*/   3		op_ima_obj_attr_get(OtherID, "name", ModuleName);   $		if (strcmp(ModuleName, "TX") == 0)   			{   <				op_ima_obj_attr_get(StrmID, "src stream", &TransStream);   ,				printf("TransStream: %d\n",TransStream);   			}   	}   }       /*Initialization*/   RecNextPckSeq=0;   
                     
   ����   
          pr_state           �          
   IDLE   
                     
       
           ����             pr_state        �  J          
   REQ_ARR   
       J   0   if ((PacketArrival))    	{   &	ip_packet_ptr = op_pk_get(PckStream);   G	op_pk_nfd_get(ip_packet_ptr, "type", &type); //type 0 means tcp packet   +	op_pk_nfd_get(ip_packet_ptr, "src", &src);   +	op_pk_nfd_get(ip_packet_ptr, "dst", &dst);   B	op_pk_nfd_get_pkt (ip_packet_ptr, "data_field", &tcp_packet_ptr);   ,	if (DataPacketArrival && type==0 && dst==0)   		{   			/*Data packet arrival*/    			DataPktArr=DataPacketArrival;   /			op_pk_nfd_get (tcp_packet_ptr, "seq", &seq);   			   �			//printf("\n NODE %d IDLE DataPktArr of size=%d, seq=%d, Exp seq=%d", op_topo_parent(op_id_self()),op_pk_total_size_get(tcp_packet_ptr),seq,RecNextPckSeq);   			if(seq==RecNextPckSeq)   				{   =				op_pk_nfd_get_pkt (tcp_packet_ptr, "data_field", &pkptr);   "				op_pk_send(pkptr, SinkStream);   				if(RecNextPckSeq==0)   					RecNextPckSeq=1;   				else   					RecNextPckSeq=0;   				   				}   			else   #				op_pk_destroy (tcp_packet_ptr);   				   3			ackptr = op_pk_create_fmt ("pm_tcp_ack_packet");   0			op_pk_nfd_set (ackptr, "seq", RecNextPckSeq);   
			type=0;   			dst=src;   				src=0;   ;			op_pk_nfd_set_pkt (ip_packet_ptr, "data_field", ackptr);   .			op_pk_nfd_set(ip_packet_ptr, "type", type);   ,			op_pk_nfd_set(ip_packet_ptr, "src", src);   ,			op_pk_nfd_set(ip_packet_ptr, "dst", dst);   *			op_pk_send(ip_packet_ptr, TransStream);   		}   	else if(type==0)   		{   			/*ACK packet arrival*/   D			op_pk_nfd_get_pkt (ip_packet_ptr, "data_field", &tcp_packet_ptr);   /			op_pk_nfd_get (tcp_packet_ptr, "seq", &seq);   �			printf("\n NODE %d IDLE AckArr - LOGICAL ERROR! of size=%d seq=%d",op_topo_parent(op_id_self()),op_pk_total_size_get (tcp_packet_ptr), seq);   			   		}   	   	}   J                     
   ����   
          pr_state                      �   �      v   �   �   �          
   tr_0   
       
   PowerUp   
       ����          
    ����   
          ����                       pr_transition      
        H          �  |  E          
   tr_10   
       ����          ����          
    ����   
          ����                       pr_transition              X       �  ;  !   �          
   tr_11   
       ����          ����          
    ����   
          ����                       pr_transition                                             