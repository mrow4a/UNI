MIL_3_Tfile_Hdr_ 171A 171A modeler 9 55109090 5568066B 7A FTNK-B0013 s146147 0 0 none none 0 0 none 6999C094 39B4 0 0 0 0 0 0 2b74 2                                                                                                                                                                                                                                                                                                                                                                                            ��g�      D  �  �  j  n  �  7�  7�  7�  7�  7�  7�  7�  �  �      IntDelay   �������   ����               ����              ����              ����           �Z             IntWebDelay   �������   ����               ����              ����              ����           �Z             jitter_mode    �������    ����           ����          ����          ����           �Z                 	   begsim intrpt         
   ����   
   doc file            	nd_module      endsim intrpt             ����      failure intrpts            disabled      intrpt interval         ԲI�%��}����      priority              ����      recovery intrpts            disabled      subqueue                     count    ���   
   ����   
      list   	���   
          
      super priority             ����             int	\UserINStream;       int	\UserOUTStream;       int	\RXservWEBStream;       int	\TXservWEBStream;       Evhandle	\TrFinD;       double	\IntDelay;       Evhandle	\TrFinU;       int	\RXservUTPStream;       int	\TXservUTPStream;       int	\RXservTCPStream;       int	\TXservTCPStream;       double	\IntWebDelay;       int	\jitter_mode;          Packet *pkptr;      8#define PowerUp (op_intrpt_type() == OPC_INTRPT_BEGSIM)    ;#define PacketArrival (op_intrpt_type() == OPC_INTRPT_STRM)       #define BuffDLWEB 0   #define BuffDL 1   #define BuffULWEB 2   #define BuffUL 3   8#define TransOver (op_intrpt_type() == OPC_INTRPT_SELF)    �#define EmptyQueue (op_subq_stat (0, OPC_QSTAT_PKSIZE)==0 && op_subq_stat (1, OPC_QSTAT_PKSIZE)==0 && op_subq_stat (2, OPC_QSTAT_PKSIZE)==0 && op_subq_stat (3, OPC_QSTAT_PKSIZE)==0)   �#define NotEmptyQueue ( op_subq_stat (0, OPC_QSTAT_PKSIZE)!=0 || op_subq_stat (1, OPC_QSTAT_PKSIZE)!=0 || op_subq_stat (2, OPC_QSTAT_PKSIZE)!=0 || op_subq_stat (3, OPC_QSTAT_PKSIZE)!=0)       #define TCP_DATA 0   #define UTP_DATA 1   #define WEB_DATA 2                                                 Z   �          
   INIT   
       
   {   "/*ID of the connections Incoming*/   Objid MyID, StrmID, OtherID;    int index, InStreams,OutStreams;   char ModuleName[64];   MyID = op_id_self();       2op_ima_obj_attr_get (MyID, "IntDelay", &IntDelay);   8op_ima_obj_attr_get (MyID, "jitter_mode", &jitter_mode);   8op_ima_obj_attr_get (MyID, "IntWebDelay", &IntWebDelay);   KInStreams = op_topo_assoc_count(MyID, OPC_TOPO_ASSOC_IN, OPC_OBJTYPE_STRM);   MOutStreams = op_topo_assoc_count(MyID, OPC_TOPO_ASSOC_OUT, OPC_OBJTYPE_STRM);   '/*Find the Packet and AppData Streams*/       +for (index = 0; index < InStreams; index++)   {   6	/*Find the stream index at the specific association*/   J	StrmID = op_topo_assoc(MyID, OPC_TOPO_ASSOC_IN, OPC_OBJTYPE_STRM, index);   	   I	if(op_topo_assoc_count(StrmID, OPC_TOPO_ASSOC_IN, OPC_OBJTYPE_QUEUE)!=0)   	{   8		/*Find the process index at the specific association*/   J		OtherID = op_topo_assoc(StrmID, OPC_TOPO_ASSOC_IN, OPC_OBJTYPE_QUEUE,0);   ;		/*Access the attribute name and find the proper streams*/   3		op_ima_obj_attr_get(OtherID, "name", ModuleName);   &		if (strcmp(ModuleName, "USER") == 0)   			{   >				op_ima_obj_attr_get(StrmID, "dest stream", &UserINStream);   .				printf("UserINStream: %d\n",UserINStream);   			}   	}   H	if(op_topo_assoc_count(StrmID, OPC_TOPO_ASSOC_IN, OPC_OBJTYPE_PTRX)!=0)   	{   8		/*Find the process index at the specific association*/   I		OtherID = op_topo_assoc(StrmID, OPC_TOPO_ASSOC_IN, OPC_OBJTYPE_PTRX,0);   ;		/*Access the attribute name and find the proper streams*/   3		op_ima_obj_attr_get(OtherID, "name", ModuleName);   -		if (strcmp(ModuleName, "RX_SERV_WEB") == 0)   			{   A				op_ima_obj_attr_get(StrmID, "dest stream", &RXservWEBStream);   9				printf("SERV RXservWEBStream: %d\n",RXservWEBStream);   			}   	}   H	if(op_topo_assoc_count(StrmID, OPC_TOPO_ASSOC_IN, OPC_OBJTYPE_PTRX)!=0)   	{   8		/*Find the process index at the specific association*/   I		OtherID = op_topo_assoc(StrmID, OPC_TOPO_ASSOC_IN, OPC_OBJTYPE_PTRX,0);   ;		/*Access the attribute name and find the proper streams*/   3		op_ima_obj_attr_get(OtherID, "name", ModuleName);   -		if (strcmp(ModuleName, "RX_SERV_UTP") == 0)   			{   A				op_ima_obj_attr_get(StrmID, "dest stream", &RXservUTPStream);   9				printf("SERV RXservUTPStream: %d\n",RXservUTPStream);   			}   	}   H	if(op_topo_assoc_count(StrmID, OPC_TOPO_ASSOC_IN, OPC_OBJTYPE_PTRX)!=0)   	{   8		/*Find the process index at the specific association*/   I		OtherID = op_topo_assoc(StrmID, OPC_TOPO_ASSOC_IN, OPC_OBJTYPE_PTRX,0);   ;		/*Access the attribute name and find the proper streams*/   3		op_ima_obj_attr_get(OtherID, "name", ModuleName);   -		if (strcmp(ModuleName, "RX_SERV_TCP") == 0)   			{   A				op_ima_obj_attr_get(StrmID, "dest stream", &RXservTCPStream);   9				printf("SERV RXservTCPStream: %d\n",RXservTCPStream);   			}   	}   }       ,for (index = 0; index < OutStreams; index++)   {   6	/*Find the stream index at the specific association*/   K	StrmID = op_topo_assoc(MyID, OPC_TOPO_ASSOC_OUT, OPC_OBJTYPE_STRM, index);   	   J	if(op_topo_assoc_count(StrmID, OPC_TOPO_ASSOC_OUT, OPC_OBJTYPE_QUEUE)!=0)   	{   8		/*Find the process index at the specific association*/   K		OtherID = op_topo_assoc(StrmID, OPC_TOPO_ASSOC_OUT, OPC_OBJTYPE_QUEUE,0);   ;		/*Access the attribute name and find the proper streams*/   3		op_ima_obj_attr_get(OtherID, "name", ModuleName);   &		if (strcmp(ModuleName, "USER") == 0)   			{   >				op_ima_obj_attr_get(StrmID, "src stream", &UserOUTStream);   0				printf("UserOUTStream: %d\n",UserOUTStream);   			}   	}   I	if(op_topo_assoc_count(StrmID, OPC_TOPO_ASSOC_OUT, OPC_OBJTYPE_PTTX)!=0)   	{   8		/*Find the process index at the specific association*/   J		OtherID = op_topo_assoc(StrmID, OPC_TOPO_ASSOC_OUT, OPC_OBJTYPE_PTTX,0);   ;		/*Access the attribute name and find the proper streams*/   3		op_ima_obj_attr_get(OtherID, "name", ModuleName);   -		if (strcmp(ModuleName, "TX_SERV_WEB") == 0)   			{   @				op_ima_obj_attr_get(StrmID, "src stream", &TXservWEBStream);   9				printf("SERV TXservWEBStream: %d\n",TXservWEBStream);   			}   	}   I	if(op_topo_assoc_count(StrmID, OPC_TOPO_ASSOC_OUT, OPC_OBJTYPE_PTTX)!=0)   	{   8		/*Find the process index at the specific association*/   J		OtherID = op_topo_assoc(StrmID, OPC_TOPO_ASSOC_OUT, OPC_OBJTYPE_PTTX,0);   ;		/*Access the attribute name and find the proper streams*/   3		op_ima_obj_attr_get(OtherID, "name", ModuleName);   -		if (strcmp(ModuleName, "TX_SERV_UTP") == 0)   			{   @				op_ima_obj_attr_get(StrmID, "src stream", &TXservUTPStream);   9				printf("SERV TXservUTPStream: %d\n",TXservUTPStream);   			}   	}   I	if(op_topo_assoc_count(StrmID, OPC_TOPO_ASSOC_OUT, OPC_OBJTYPE_PTTX)!=0)   	{   8		/*Find the process index at the specific association*/   J		OtherID = op_topo_assoc(StrmID, OPC_TOPO_ASSOC_OUT, OPC_OBJTYPE_PTTX,0);   ;		/*Access the attribute name and find the proper streams*/   3		op_ima_obj_attr_get(OtherID, "name", ModuleName);   -		if (strcmp(ModuleName, "TX_SERV_TCP") == 0)   			{   @				op_ima_obj_attr_get(StrmID, "src stream", &TXservTCPStream);   9				printf("SERV TXservTCPStream: %d\n",TXservTCPStream);   			}   	}   }       
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
       J   `   Packet* ip_packet;   int type,BUFF_TYPE;   double internet_delay;   $/*DOWNLINK, so PASS to the network*/   if(jitter_mode==0)   	internet_delay=IntDelay;   else if(jitter_mode==1)   	{   :	internet_delay=IntDelay+op_dist_exponential(0.005)-0.005;   	   	}       %if(op_intrpt_strm () == UserINStream)   	{    	//printf("\n IN UserINStream");   "	/* Obtain the incoming packet.	*/   &	ip_packet = op_pk_get (UserINStream);   �	//printf("\n IN UserINStream, BUFF: %f, size: %d, timestamp:%f", op_subq_stat (BuffUL, OPC_QSTAT_PKSIZE),op_pk_total_size_get(ip_packet)/8,op_sim_time());   ,	op_pk_nfd_access (ip_packet, "type",&type);   	BUFF_TYPE=(int)BuffUL;   	if(type==TCP_DATA)   		{   3		op_pk_nfd_set (ip_packet, "dst",TXservTCPStream);   		}   	else if(type==UTP_DATA)   		{   3		op_pk_nfd_set (ip_packet, "dst",TXservUTPStream);   		}   	else if(type==WEB_DATA)   		{   3		op_pk_nfd_set (ip_packet, "dst",TXservWEBStream);   		   		internet_delay=IntWebDelay;   		   		BUFF_TYPE=(int)BuffULWEB;   		}   	   7	if(op_subq_stat (BUFF_TYPE, OPC_QSTAT_FREE_PKSIZE)==0)   		{   		op_pk_destroy (ip_packet);   )		printf("\n RXStream packet Destroyed");   		}   N	else if(op_subq_pk_insert(BUFF_TYPE, ip_packet, OPC_QPOS_TAIL)!= OPC_QINS_OK)   !		printf("\n Packet not queued");   6	else if(op_subq_stat (BUFF_TYPE, OPC_QSTAT_PKSIZE)>0)   		{   E		op_intrpt_schedule_self(op_sim_time() + internet_delay, BUFF_TYPE);   :		//printf("\n SERV RXStream packet starts transmission");   		}   	   	   	}   else    	{   	//printf("\n SERV RXStream ");   "	/* Obtain the incoming packet.	*/   '	pkptr = op_pk_get (op_intrpt_strm ());   /	ip_packet = op_pk_create_fmt ("pm_ip_packet");   4	op_pk_nfd_set (ip_packet, "src",op_intrpt_strm ());   	BUFF_TYPE=(int)BuffDL;   )	if(op_intrpt_strm () == RXservUTPStream)   		{   -		op_pk_nfd_set (ip_packet, "type",UTP_DATA);   ,		op_pk_nfd_set (ip_packet, "dst",UTP_DATA);   		}   .	else if(op_intrpt_strm () == RXservWEBStream)   		{   -		op_pk_nfd_set (ip_packet, "type",WEB_DATA);   ,		op_pk_nfd_set (ip_packet, "dst",WEB_DATA);   		   		internet_delay=IntWebDelay;   		   		BUFF_TYPE=(int)BuffDLWEB;   		}   .	else if(op_intrpt_strm () == RXservTCPStream)   		{   .		op_pk_nfd_set (ip_packet, "type",TCP_DATA);	   ,		op_pk_nfd_set (ip_packet, "dst",TCP_DATA);   		}   	   4	op_pk_nfd_set_pkt (ip_packet, "data_field", pkptr);   	   S	//printf("\n IN RXservStream, BUFF: %f", op_subq_stat (BuffDL, OPC_QSTAT_PKSIZE));   7	if(op_subq_stat (BUFF_TYPE, OPC_QSTAT_FREE_PKSIZE)==0)   		{   		op_pk_destroy (ip_packet);   )		printf("\n RXStream packet Destroyed");   		}   N	else if(op_subq_pk_insert(BUFF_TYPE, ip_packet, OPC_QPOS_TAIL)!= OPC_QINS_OK)   !		printf("\n Packet not queued");   6	else if(op_subq_stat (BUFF_TYPE, OPC_QSTAT_PKSIZE)>0)   		{   E		op_intrpt_schedule_self(op_sim_time() + internet_delay, BUFF_TYPE);   		}       	}   J                     
   ����   
          pr_state        �   �          
   	WAIT_INRT   
       
      //printf("\n In WAIT_TRA");   
                         ����             pr_state        �   Z          
   SEND   
       
   C   int dst,type;   Packet* ip_packet;   /* Send the incoming packet.	*/   if(op_intrpt_code ()==BuffDL)   	{   3	pkptr = op_subq_pk_remove (BuffDL, OPC_QPOS_HEAD);   #	op_pk_send (pkptr, UserOUTStream);   A	//printf("\n CLOUD UserOUTStream Packet finished transmission");       	   	}   %else if(op_intrpt_code ()==BuffDLWEB)   	{   6	pkptr = op_subq_pk_remove (BuffDLWEB, OPC_QPOS_HEAD);   #	op_pk_send (pkptr, UserOUTStream);   A	//printf("\n CLOUD UserOUTStream Packet finished transmission");       	   	}   "else if(op_intrpt_code ()==BuffUL)   	{   7	ip_packet = op_subq_pk_remove (BuffUL, OPC_QPOS_HEAD);   -	op_pk_nfd_access (ip_packet, "type", &type);   F	//printf("\n SERV BuffUL, dst: %d Packet finished transmission",dst);   	if(type==TCP_DATA)   		{   		dst=TXservTCPStream;   		}   	else if(type==UTP_DATA)   		{   		dst=TXservUTPStream;   		}   	else if(type==WEB_DATA)   		{   		dst=TXservWEBStream;   		}   	   F	//printf("\n SERV BuffUL, dst: %d Packet finished transmission",dst);   5	//printf("\n CLOUD TXserv, Packet send to: %d",dst);   5	op_pk_nfd_get_pkt (ip_packet, "data_field", &pkptr);   	op_pk_send (pkptr, dst);   	op_pk_destroy (ip_packet);   	}   %else if(op_intrpt_code ()==BuffULWEB)   	{   :	ip_packet = op_subq_pk_remove (BuffULWEB, OPC_QPOS_HEAD);   -	op_pk_nfd_access (ip_packet, "type", &type);   F	//printf("\n SERV BuffUL, dst: %d Packet finished transmission",dst);   	if(type==TCP_DATA)   		{   		dst=TXservTCPStream;   		}   	else if(type==UTP_DATA)   		{   		dst=TXservUTPStream;   		}   	else if(type==WEB_DATA)   		{   		dst=TXservWEBStream;   		}   	   F	//printf("\n SERV BuffUL, dst: %d Packet finished transmission",dst);   5	//printf("\n CLOUD TXserv, Packet send to: %d",dst);   5	op_pk_nfd_get_pkt (ip_packet, "data_field", &pkptr);   	op_pk_send (pkptr, dst);   	op_pk_destroy (ip_packet);   	}   
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
          ����                       pr_transition              �  	     �  5  �   �          
   tr_12   
       
   NotEmptyQueue   
       ����          
    ����   
          ����                       pr_transition              R   �     �  6     �          
   tr_13   
       
   
EmptyQueue   
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
          ����                       pr_transition              5   �     �   k     �          
   tr_17   
       
   
EmptyQueue   
       ����          
    ����   
          ����                       pr_transition              �   �     �   j  �   �          
   tr_18   
       
   NotEmptyQueue   
       ����          
    ����   
          ����                       pr_transition                                             