MIL_3_Tfile_Hdr_ 171A 171A modeler 9 5511A253 556646BC 81 FTNK-B0013 s146147 0 0 none none 0 0 none BD0ED1BD 3513 0 0 0 0 0 0 2b74 2                                                                                                                                                                                                                                                                                                                                                                                            ��g�      D  4  8       7  2�  2�  2�  2�  3  3  3  '  3      	UpLinkCap   �������   ����       Ac�    ����              ����              ����           �Z             DownLinkCap   �������   ����       A�ׄ    ����              ����              ����           �Z                 	   begsim intrpt         
   ����   
   doc file            	nd_module      endsim intrpt             ����      failure intrpts            disabled      intrpt interval         ԲI�%��}����      priority              ����      recovery intrpts            disabled      subqueue                     count    ���   
   ����   
      list   	���   
          
      super priority             ����              int	\InternetINStream;       int	\InternetOUTStream;       int	\RXStream;       int	\TXStream;       double	\UpLinkCap;       Evhandle	\TrFinU;       int	\RXuTPStream;       int	\TXuTPStream;       int	\RXwebStream;       int	\TXwebStream;       double	\DownLinkCap;       Evhandle	\TrFinD;       Stathandle	\packets_drop_hndl;       double	\packets_drop;       int	\RXTCPStream;       int	\TXTCPStream;          Packet *pkptr;   Packet *ip_packet;      8#define PowerUp (op_intrpt_type() == OPC_INTRPT_BEGSIM)    ;#define PacketArrival (op_intrpt_type() == OPC_INTRPT_STRM)           #define BuffUL 0   #define BuffDL 1   8#define TransOver (op_intrpt_type() == OPC_INTRPT_SELF)        #define TCP_DATA 0   #define UTP_DATA 1   #define WEB_DATA 2                                                 Z   �          
   INIT   
       
   |   "/*ID of the connections Incoming*/   Objid MyID, StrmID, OtherID;    int index, InStreams,OutStreams;   char ModuleName[64];   MyID = op_id_self();       4op_ima_obj_attr_get (MyID, "UpLinkCap", &UpLinkCap);   8op_ima_obj_attr_get (MyID, "DownLinkCap", &DownLinkCap);   ^packets_drop_hndl   = op_stat_reg ("User.Packets drop",		OPC_STAT_INDEX_NONE, OPC_STAT_LOCAL);   packets_drop=0.0;   KInStreams = op_topo_assoc_count(MyID, OPC_TOPO_ASSOC_IN, OPC_OBJTYPE_STRM);   MOutStreams = op_topo_assoc_count(MyID, OPC_TOPO_ASSOC_OUT, OPC_OBJTYPE_STRM);   '/*Find the Packet and AppData Streams*/       +for (index = 0; index < InStreams; index++)   {   6	/*Find the stream index at the specific association*/   J	StrmID = op_topo_assoc(MyID, OPC_TOPO_ASSOC_IN, OPC_OBJTYPE_STRM, index);   	   I	if(op_topo_assoc_count(StrmID, OPC_TOPO_ASSOC_IN, OPC_OBJTYPE_QUEUE)!=0)   	{   8		/*Find the process index at the specific association*/   J		OtherID = op_topo_assoc(StrmID, OPC_TOPO_ASSOC_IN, OPC_OBJTYPE_QUEUE,0);   ;		/*Access the attribute name and find the proper streams*/   3		op_ima_obj_attr_get(OtherID, "name", ModuleName);   0		if (strcmp(ModuleName, "INTERNET Cloud") == 0)   			{   B				op_ima_obj_attr_get(StrmID, "dest stream", &InternetINStream);   6				printf("InternetINStream: %d\n",InternetINStream);   			}   	}   H	if(op_topo_assoc_count(StrmID, OPC_TOPO_ASSOC_IN, OPC_OBJTYPE_PTRX)!=0)   	{   8		/*Find the process index at the specific association*/   I		OtherID = op_topo_assoc(StrmID, OPC_TOPO_ASSOC_IN, OPC_OBJTYPE_PTRX,0);   ;		/*Access the attribute name and find the proper streams*/   3		op_ima_obj_attr_get(OtherID, "name", ModuleName);   (		if (strcmp(ModuleName, "RX_uTP") == 0)   			{   =				op_ima_obj_attr_get(StrmID, "dest stream", &RXuTPStream);   ,				printf("RXuTPStream: %d\n",RXuTPStream);   			}   	}   H	if(op_topo_assoc_count(StrmID, OPC_TOPO_ASSOC_IN, OPC_OBJTYPE_PTRX)!=0)   	{   8		/*Find the process index at the specific association*/   I		OtherID = op_topo_assoc(StrmID, OPC_TOPO_ASSOC_IN, OPC_OBJTYPE_PTRX,0);   ;		/*Access the attribute name and find the proper streams*/   3		op_ima_obj_attr_get(OtherID, "name", ModuleName);   (		if (strcmp(ModuleName, "RX_WEB") == 0)   			{   =				op_ima_obj_attr_get(StrmID, "dest stream", &RXwebStream);   ,				printf("RXwebStream: %d\n",RXwebStream);   			}   	}   H	if(op_topo_assoc_count(StrmID, OPC_TOPO_ASSOC_IN, OPC_OBJTYPE_PTRX)!=0)   	{   8		/*Find the process index at the specific association*/   I		OtherID = op_topo_assoc(StrmID, OPC_TOPO_ASSOC_IN, OPC_OBJTYPE_PTRX,0);   ;		/*Access the attribute name and find the proper streams*/   3		op_ima_obj_attr_get(OtherID, "name", ModuleName);   (		if (strcmp(ModuleName, "RX_TCP") == 0)   			{   =				op_ima_obj_attr_get(StrmID, "dest stream", &RXTCPStream);   ,				printf("RXTCPStream: %d\n",RXTCPStream);   			}   	}   }       ,for (index = 0; index < OutStreams; index++)   {   6	/*Find the stream index at the specific association*/   K	StrmID = op_topo_assoc(MyID, OPC_TOPO_ASSOC_OUT, OPC_OBJTYPE_STRM, index);   	   J	if(op_topo_assoc_count(StrmID, OPC_TOPO_ASSOC_OUT, OPC_OBJTYPE_QUEUE)!=0)   	{   8		/*Find the process index at the specific association*/   K		OtherID = op_topo_assoc(StrmID, OPC_TOPO_ASSOC_OUT, OPC_OBJTYPE_QUEUE,0);   ;		/*Access the attribute name and find the proper streams*/   3		op_ima_obj_attr_get(OtherID, "name", ModuleName);   0		if (strcmp(ModuleName, "INTERNET Cloud") == 0)   			{   B				op_ima_obj_attr_get(StrmID, "src stream", &InternetOUTStream);   8				printf("InternetOUTStream: %d\n",InternetOUTStream);   			}   	}   I	if(op_topo_assoc_count(StrmID, OPC_TOPO_ASSOC_OUT, OPC_OBJTYPE_PTTX)!=0)   	{   8		/*Find the process index at the specific association*/   J		OtherID = op_topo_assoc(StrmID, OPC_TOPO_ASSOC_OUT, OPC_OBJTYPE_PTTX,0);   ;		/*Access the attribute name and find the proper streams*/   3		op_ima_obj_attr_get(OtherID, "name", ModuleName);   (		if (strcmp(ModuleName, "TX_uTP") == 0)   			{   <				op_ima_obj_attr_get(StrmID, "src stream", &TXuTPStream);   ,				printf("TXuTPStream: %d\n",TXuTPStream);   			}   	}   I	if(op_topo_assoc_count(StrmID, OPC_TOPO_ASSOC_OUT, OPC_OBJTYPE_PTTX)!=0)   	{   8		/*Find the process index at the specific association*/   J		OtherID = op_topo_assoc(StrmID, OPC_TOPO_ASSOC_OUT, OPC_OBJTYPE_PTTX,0);   ;		/*Access the attribute name and find the proper streams*/   3		op_ima_obj_attr_get(OtherID, "name", ModuleName);   (		if (strcmp(ModuleName, "TX_WEB") == 0)   			{   <				op_ima_obj_attr_get(StrmID, "src stream", &TXwebStream);   ,				printf("TXwebStream: %d\n",TXwebStream);   			}   	}   I	if(op_topo_assoc_count(StrmID, OPC_TOPO_ASSOC_OUT, OPC_OBJTYPE_PTTX)!=0)   	{   8		/*Find the process index at the specific association*/   J		OtherID = op_topo_assoc(StrmID, OPC_TOPO_ASSOC_OUT, OPC_OBJTYPE_PTTX,0);   ;		/*Access the attribute name and find the proper streams*/   3		op_ima_obj_attr_get(OtherID, "name", ModuleName);   (		if (strcmp(ModuleName, "TX_TCP") == 0)   			{   <				op_ima_obj_attr_get(StrmID, "src stream", &TXTCPStream);   ,				printf("TXTCPStream: %d\n",TXTCPStream);   			}   	}   }   %op_stat_write (packets_drop_hndl, 0);   
                     
   ����   
          pr_state           �          
   IDLE   
       
          
       
      //printf("\n Packet arrival");   
           ����             pr_state        �  J          
   PKT_HND   
       J   Q   
int src=0;   
int dst=0;   int type=0;   int seq_num,ack_num;   Packet* est_pkt;   %	/*DOWNLINK, so PASS to the network*/   )if(op_intrpt_strm () == InternetINStream)   	{   "	/* Obtain the incoming packet.	*/   *	ip_packet = op_pk_get (InternetINStream);   	est_pkt=op_pk_copy(ip_packet);   	   F	if(op_subq_pk_insert(BuffDL, ip_packet, OPC_QPOS_TAIL)!= OPC_QINS_OK)   		{   		op_pk_destroy (ip_packet);   +		//printf("\n RXStream packet Destroyed");   		}   4	else if(op_subq_stat (BuffDL, OPC_QSTAT_PKSIZE)==1)   		{   d		TrFinD=op_intrpt_schedule_self(op_sim_time() + op_pk_total_size_get(est_pkt)/DownLinkCap, BuffDL);       		}   	op_pk_destroy (est_pkt);   	}       	/*UPLINK*/   else    	{   "	/* Obtain the incoming packet.	*/   	RXStream=op_intrpt_strm ();   	pkptr = op_pk_get (RXStream);   	   	est_pkt=op_pk_copy(pkptr);   /	ip_packet = op_pk_create_fmt ("pm_ip_packet");   	if(RXStream==RXwebStream)   		{   		type=WEB_DATA;   		dst=WEB_DATA;   		}   	else if(RXStream==RXuTPStream)   		{   		type=UTP_DATA;   		dst=UTP_DATA;   		}   	else if(RXStream==RXTCPStream)   		{   		type=TCP_DATA;   		dst=TCP_DATA;   		}   +	op_pk_nfd_set (ip_packet, "src",RXStream);   &	op_pk_nfd_set (ip_packet, "dst",dst);   )	op_pk_nfd_set (ip_packet, "type",type);	   4	op_pk_nfd_set_pkt (ip_packet, "data_field", pkptr);   	   �	//printf("\n USER IN RXSTREAM, BUFF: %f, size: %d, type=%d, timestamp:%f", op_subq_stat (BuffUL, OPC_QSTAT_PKSIZE), op_pk_total_size_get(pkptr)/8,type,op_sim_time());   	   	   F	if(op_subq_pk_insert(BuffUL, ip_packet, OPC_QPOS_TAIL)!= OPC_QINS_OK)   		{   		op_pk_destroy (ip_packet);   E		op_stat_write (packets_drop_hndl, packets_drop=packets_drop + 1.0);   .		op_pk_nfd_get (est_pkt, "ack_num",&ack_num);   .		op_pk_nfd_get (est_pkt, "seq_num",&seq_num);   		if(type==WEB_DATA)   			{   M			//printf("\n RXStream WEB_DATA packet Destroyed s%d,a%d",seq_num,ack_num);   			}   		else    			{   D			//printf("\n RXStream packet Destroyed s%d,a%d",seq_num,ack_num);   			}   		   		}   4	else if(op_subq_stat (BuffUL, OPC_QSTAT_PKSIZE)==1)   		{   b		TrFinU=op_intrpt_schedule_self(op_sim_time() + op_pk_total_size_get(est_pkt)/UpLinkCap, BuffUL);       		}       	op_pk_destroy (est_pkt);   	}   J                     
   ����   
          pr_state        �   �          
   WAIT_TRA   
       
      //printf("\n In WAIT_TRA");   
                         ����             pr_state        �   Z          
   SEND   
       
   /   int type=0,dst;   Packet* est_pkt;   if(op_intrpt_code ()==BuffUL)   	{    	/* Send the incoming packet.	*/   3	pkptr = op_subq_pk_remove (BuffUL, OPC_QPOS_HEAD);   '	op_pk_send (pkptr, InternetOUTStream);   -	//printf("\n Packet finished transmission");   /	if(op_subq_stat (BuffUL, OPC_QSTAT_PKSIZE)!=0)   		{   6		est_pkt = op_subq_pk_access (BuffUL, OPC_QPOS_HEAD);   b		TrFinU=op_intrpt_schedule_self(op_sim_time() + op_pk_total_size_get(est_pkt)/UpLinkCap, BuffUL);   a		//printf("\n SEND InternetOUTStream BUFFER SIZE = %f",op_subq_stat (BuffUL, OPC_QSTAT_PKSIZE));   		}   	   	}   "else if(op_intrpt_code ()==BuffDL)   	{   6	ip_packet= op_subq_pk_remove (BuffDL, OPC_QPOS_HEAD);    	/* Send the incoming packet.	*/   5	op_pk_nfd_get_pkt (ip_packet, "data_field", &pkptr);   ,	op_pk_nfd_access(ip_packet, "type", &type);   	if(type==WEB_DATA)   		{   		dst=TXwebStream;   		}   	else if(type==UTP_DATA)   		{   		dst=TXuTPStream;   		}   	else if(type==TCP_DATA)   		{   		dst=TXTCPStream;   		}   	   /	//printf("\n USER INTinStream, dst: %d", dst);   	op_pk_send (pkptr, dst);   	op_pk_destroy (ip_packet);   	   /	if(op_subq_stat (BuffDL, OPC_QSTAT_PKSIZE)!=0)   		{   6		est_pkt = op_subq_pk_access (BuffDL, OPC_QPOS_HEAD);   d		TrFinD=op_intrpt_schedule_self(op_sim_time() + op_pk_total_size_get(est_pkt)/DownLinkCap, BuffDL);   6		//printf("\n SERV Next Packet starts transmission");   		}   	   	}   
                     
   ����   
          pr_state                      �   �      v   �   �   �          
   tr_0   
       
   PowerUp   
       ����          
    ����   
          ����                       pr_transition      
        B  &        �  �  S          
   tr_10   
       
   PacketArrival   
       ����          
    ����   
          ����                       pr_transition              �       �  5  �   �          
   tr_12   
       
   Q (op_subq_stat (0, OPC_QSTAT_PKSIZE)!=0 || op_subq_stat (1, OPC_QSTAT_PKSIZE)!=0)   
       ����          
    ����   
          ����                       pr_transition              M       �  6     �          
   tr_13   
       
   Nop_subq_stat (0, OPC_QSTAT_PKSIZE)==0 && op_subq_stat (1, OPC_QSTAT_PKSIZE)==0   
       ����          
    ����   
          ����                       pr_transition              �  6     	   �  �  N          
   tr_15   
       
   PacketArrival   
       ����          
    ����   
          ����                       pr_transition              �   �     �   �  �   d          
   tr_16   
       
   	TransOver   
       ����          
    ����   
          ����                       pr_transition               �   �     �   k     �          
   tr_17   
       
   Nop_subq_stat (0, OPC_QSTAT_PKSIZE)==0 && op_subq_stat (1, OPC_QSTAT_PKSIZE)==0   
       ����          
    ����   
          ����                       pr_transition              �   �     �   j  �   �          
   tr_18   
       
   Nop_subq_stat (0, OPC_QSTAT_PKSIZE)!=0 || op_subq_stat (1, OPC_QSTAT_PKSIZE)!=0   
       ����          
    ����   
          ����                       pr_transition                   Packets drop           User   normal����        ԲI�%��}                            