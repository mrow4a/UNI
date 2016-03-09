import os
import string
import sqlite3
import time, datetime
import threading, thread
import bluetooth._bluetooth as bluez
import requests
import json
import collections
import socket
import sys
import struct
import threading, thread
#VER 4
# bluez functions, contstants can be found in bluez C source code in different files (lib/hci.h, lib/hci.c)
# bluetooth._bluetooth = bluetooth.so shared object in: /usr/lib/python2.7/dist-packages/bluetooth/_bluetooth

# Check Bluetooth specification (v4.2) Page 1271, Vol 2, Part E, section 7.8 LE Controller Commands
OGF_LE_CTL=0x08 # LE Controller Commands Group Code
OCF_LE_SET_SCAN_PARAMETERS=0x000B  # LE Set Scan Parameters Command -> Page 1286, Vol 2, Part E, Section: 7.8.10
OCF_LE_SET_SCAN_ENABLE=0x000C      # LE Set Scan Enable Command

#Check Bluetooth specification (v4.2) Page 1241, Vol 2, Part E, section 7.7.65.2 LE Advertising Report Event
LE_META_EVENT = 0x3E # event type for all LE event -> encapsulate all LE Controller speicific events
EVT_LE_ADVERTISING_REPORT=0x02 # subevent of LE_META_EVENT
# Advertisment event types
ADV_IND=0x00
ADV_DIRECT_IND=0x01
ADV_SCAN_IND=0x02
ADV_NONCONN_IND=0x03
ADV_SCAN_RSP=0x04

#Generated Event format (LE Advertising Report Event) by the controller for the host after an incoming Advertising packet
#Check Bluetooth Specification (v4.2), Vol 2, Part E, 5.4.4 HCI Event Packet (Page 786) + LE Advertising Report Event parameters (Page 1241)
#In case of 1 generated report/responses:
#1.byte : Packet type = HCI Event Packet = 0x04
#2.byte : Event Code = 0x3E - event type for all LE event -> encapsulate all LE Controller speicific events
#3.byte : Parameter Total Length
#4.byte : Subevent Code = 0x02
#5.byte : Number of responses = 0x01-0x19
#6.byte : Event Type: Non Connectable unidirected advertising = 0x03
#7.byte : Address Type
#8-13   : Address
#14.byte: Length of data (per reports)
#15-45  : Maximum 31 bytes of Data
#46.byte: RSSI in dBm

def process_BLE_packet(packet):
    Check_Sum_Genarated = 0
    j = 0
    
    Packet_type,Event_Code,Parameter_Total_Length,Subevent_Code = struct.unpack("BBBB",packet[0:4])
          
    if((Packet_type == 0x04) & (Event_Code == 0x3E) & (Subevent_Code == 0x02)): 

        try:
            Code = ''.join(struct.unpack("sss",packet[14:17]))  # string.join concatenates elements where the seperator will be the calling string between the elements
                
        except:
            # This is needed because if a BLE device advertises itself, but does not have data part, the previos command drops an exception
            return 0
        
        if(Code == "SUP"): #Also need to check the checksum from the message
                
            try:          
                Check_Sum_from_Packet,RSSI = struct.unpack("Bb",packet[24:26])          
                Plate_Number = ''.join(struct.unpack("sssssss",packet[17:24]))
            except:
                return 0
                
            # Calculate the "special" checksum generated by BLE TAG for further security 
            for i in range(0,7):
                Check_Sum_Genarated ^= ord(Plate_Number[i]) ^ ord(Code[j]) 
                if(j == 2): 
                    j = 0    
                else:
                    j = j+1
                        
            Code = ''               
            if(Check_Sum_Genarated == Check_Sum_from_Packet):  #Check the generated checksum
                # The averaging here is good only if 1 BLE TAG sending information
                # The formula used for distance calculation is not reliable, need to find a better one
                #print "RSSI: " + str(RSSI) + "counter: " + str(counter)
                
                #print "RSSI average in dBm= ",RSSI_average
                distance = float(10**((-68-float(RSSI)) / (10 * 1.6)))  # Needs to be rethink!!!!
                #print "TAG: " + str(Plate_Number) + " FOUND: " + str(float(RSSI)) + " At distance: " + str(distance)  
                return (None,str(Plate_Number),distance) #don't forget to change the distance value!!!!  
    return 0

def update_detected_tags(entry):
    #block doubled access to the database
    database = sqlite3.connect("Car_database.db")
    cursor = database.cursor()
    cursor.executemany("INSERT into Detected_tags VALUES(?,?,?)",entry )
    database.commit()   
    database.close()
    
#Check Bluetooth specification (4.2) page 1289, Vol 2, Part E, Section: 7.8.11 - LE Set Scan Enable Command
def hci_enable_le_scan(sock):
    cmd_pkt = struct.pack("<BB", 0x01, 0x00) # Last parameter: duplicate filtering: 0x00 - disabled
    bluez.hci_send_cmd(sock, OGF_LE_CTL, OCF_LE_SET_SCAN_ENABLE, cmd_pkt)
    print "...sent scan enable"
    

def hci_le_set_scan_parameters(sock):
    print "...setting up scan"  
    # Check Bluetooth specification (4.2) Page 1286, Vol 2, Part E, Section: 7.8.10 - LE Set Scan Parameters Command
    OWN_ADDR_TYPE = 0x00 #Own Address type: what to put in scan request packet 0x00 = Public Device Address, 0x01 - Random device Address
    SCAN_TYPE = 0x00 #0x00 = Passive scanning, 0x01 = Active scanning
    INTERVAL = 0x10 # Time between 2 scanning: value * 0.625 ms
    WINDOW = 0x10 # Duration of LE scan : value * 0.625 ms
    FILTER = 0x00 # all advertisements, not just whitelisted devices
    # interval and window are uint_16, so we pad them with 0x0
    cmd_pkt = struct.pack("<BBBBBBB", SCAN_TYPE, 0x0, INTERVAL, 0x0, WINDOW, OWN_ADDR_TYPE, FILTER)
    #print "packed up: ", cmd_pkt
    bluez.hci_send_cmd(sock, OGF_LE_CTL, OCF_LE_SET_SCAN_PARAMETERS, cmd_pkt)
    print "...sent scan parameters command"


def get_lock(process_name):
    global lock_socket   # Without this our lock gets garbage collected
    lock_socket = socket.socket(socket.AF_UNIX, socket.SOCK_DGRAM)
    try:
        lock_socket.bind('\0' + process_name)
        sys.stdout.write('I got the lock'+'\n')
    except socket.error,e:
        sys.stdout.write('lock exists:'+str(e) +'\n')
        sys.exit()

def error_reporting(state):
    global ERROR_REPORT
    if(state!=False):
        if(ERROR_REPORT==False or ERROR_REPORT=="INIT"):
            ERROR_REPORT = state
        elif(ERROR_REPORT!=state):
            ERROR_REPORT += ', '+state
            
def create_database():
    os.remove("Car_database.db") if os.path.exists("Car_database.db") else None
    database = sqlite3.connect("Car_database.db")
    cursor = database.cursor()
    cursor.execute("CREATE table if not EXISTS Detected_tags (KEY INTEGER Primary KEY,Plate_Number TEXT, Distance REAL)")
    cursor.execute("CREATE table if not EXISTS Detected_cars (KEY INTEGER Primary KEY,Plate_Number TEXT, Distance REAL, Time TEXT)")
    database.commit()
    #cursor.execute("CREATE table if not EXISTS State_lamp (KEY INTEGER Primary KEY,Eth0_Address TEXT, Time TEXT, BLE_thread_init TEXT, INT_connection_init TEXT, Database_thread_init TEXT)")
            
def delete_cars_left(database):
    global TIME_TO_DELETE_CAR
    try: 
        timestamp = datetime.datetime.utcnow() - datetime.timedelta(seconds = TIME_TO_DELETE_CAR)       
        cursor = database.cursor() 
        cursor.execute("DELETE from Detected_cars WHERE Time<?",(timestamp,))
        database.commit()
    except Exception,e:
        error_reporting(("ERROR - Database_Check..." + str(e)))    

def detected_tags_to_cars(database):
    cursor = database.cursor()
    cursor.execute("""SELECT DISTINCT Plate_Number from Detected_tags""")
    rows = cursor.fetchall()
    cars_list = []
    columns = ('Plate_Number', 'Distance')
    for plate_number in rows:
        plate_number = plate_number[0]
        
        cursor.execute("SELECT AVG(Distance) from Detected_tags WHERE Plate_Number=?",(plate_number,))
        distance_tags = cursor.fetchone()
            #cursor.execute("UPDATE Detected_cars SET Distance=?, Time=? WHERE Plate_Number=?",(float(distance),datetime.datetime.utcnow(),plate_number ))
        
        if (distance_tags!=None):
            distance = distance_tags[0]
            cursor.execute("INSERT into Detected_cars VALUES(?,?,?,?)",(None,plate_number,float(distance), datetime.datetime.utcnow()) )
            database.commit()
            cursor.execute("SELECT AVG(Distance) from Detected_cars WHERE Plate_Number=?",(plate_number,))
            distance = cursor.fetchone()[0]
            cars_list.append(dict(zip(columns, (plate_number,float(distance)) )))
            
            sys.stdout.write("DETECTED CAR: " + str(plate_number) + " at: " + str(distance)+"\n")
                    
    cursor.execute("DELETE from Detected_tags")
    
    database.commit()
    return cars_list
    
def start_session(server_ip):
    global URL
    global BLOCK_THREADS
    try:
        URL = "http://"+server_ip+"/lamp_register_get/"
        client = requests.session()
        client.get(URL)
        eth_address = open('/sys/class/net/eth0/address').readline()
        eth_address = eth_address[0:17]
        client_tuple = (client,eth_address)
        sys.stdout.write("#### CAR DISCOVERY ####\n")
        sys.stdout.write("...session starts\n")
        return client_tuple
    except Exception,e:
        error_reporting("ERROR initiating session start.." + str(e)+"\n")
        return 0

def send_post_to_server(content, content_type, client_tuple):
    try:
        if(client_tuple==0):
            raise Exception, 'Cannot obtain session - server offline or refuses'
        else:
            client = client_tuple[0]
            eth0 = client_tuple[1]
            csrftoken = client.cookies['csrftoken']
        data = { "content_type": content_type, "content": (eth0, content) }
        headers = {'Content-type': 'application/json',  "X-CSRFToken":csrftoken}
        cookies = dict(client.cookies)
        response = requests.post(URL, data=json.dumps(data), headers=headers,cookies=cookies)
        response.raise_for_status()
        if content_type != "get_update":
            response = response.json()
        return response 
    except Exception,e:
        error_reporting(("ERROR - Server_Connection..." + str(e)))  
        return 0

def init_connection_thread(client_tuple):
    # Return the MAC address of interface
    try:
        response = send_post_to_server([],"set_queue", client_tuple)
        if (response and response['HTTPRESPONSE']=="1" and response['Eth0']):
            INT_connection_init = ("OK - obtaining identifier for microcontroller... eth0: [" + response['Eth0'] + "]")
            ## NEW THREAD ##
            response2 = send_post_to_server([],"get_update", client_tuple)
            if (response2):
                info = response2.headers.get('Content-Disposition', '')
                filename = "Car_discovery.py"
                os.remove(filename) if os.path.exists(filename) else None
                with open(filename, 'w') as f:
                    for chunk in response2:
                        os.chmod(filename, 0o777)
                        f.write(chunk)
                print 'getting updated...' + str(info)
                
            thread.start_new_thread( Server_Connection, (client_tuple,) )
        else:
            INT_connection_init = ("ERROR - obtaining identifier for microcontroller..." + response['HTTPRESPONSE'])
            raise Exception, INT_connection_init
            
    except Exception,e:
        INT_connection_init = ("ERROR - initializing server connection..." + str(e))
        error_reporting(INT_connection_init)
    return INT_connection_init

def init_database():
    global semaphore
    try:
        try:
            semaphore.release()
        except:
            pass
        semaphore.acquire()
        create_database()
        semaphore.release() 
        Database_thread_init = "OK - starting database..."
    except Exception,e:
        Database_thread_init = ("ERROR - starting database..." + str(e))
        error_reporting(Database_thread_init)
    
    return Database_thread_init
  
def init_BLE_thread():
    dev_id = 0
    global sock
    try:
        sock = bluez.hci_open_dev(dev_id)
        hci_le_set_scan_parameters(sock)
        hci_enable_le_scan(sock)
        ## NEW THREAD ##
        thread.start_new_thread( Catch_and_Store_advertisement_meassages, (sock,) )
        BLE_thread_init = "OK - accessing bluetooth device..."
    except Exception,e:
        BLE_thread_init = ("ERROR - accessing bluetooth device and catch thread..." + str(e))
        error_reporting(BLE_thread_init)
    return BLE_thread_init
   
def Catch_and_Store_advertisement_meassages(sock):

    global tags_list 
    global semaphore
    global CHANGES_TO_DATABASE
    
    #Setup filter to receive only the events what we need
    flt = bluez.hci_filter_new()
    bluez.hci_filter_all_events(flt) # Enable all events, then mask it with HCI_EVENT_PKT = 0x04 check bluez source/lib/hci.h
    bluez.hci_filter_set_ptype(flt, bluez.HCI_EVENT_PKT) #HCI Packet types: Bluetooth specification (4.2) page 2406, Vol 4, Part A, Section: 2 - Protocol
    sock.setsockopt( bluez.SOL_HCI, bluez.HCI_FILTER, flt ) #(socket level)SOL_HCI = 0 check bluez source/lib/bluetooth.h
    
    while(True):
        if(BLOCK_THREADS == False):
            try:
                packet = sock.recv(46)
                if packet:
                    entry = process_BLE_packet(packet)
                    if entry:
                        tags_list.append(entry)
                        if (len(tags_list)>20):
                            semaphore.acquire()
                            status = update_detected_tags(tags_list)
                            tags_list =[]
                            CHANGES_TO_DATABASE = 1
                            semaphore.release()
            except Exception,e:
                error_reporting(("ERROR - Catch_and_Store_advertisement_meassages..." + str(e)))
        else:
            print "close ble thread..."
            break

def Server_Connection(client_tuple):
    global semaphore
    global ERROR_REPORT
    global CHANGES_TO_DATABASE
    global BLOCK_THREADS
    while(True):
        if(BLOCK_THREADS == False):
            try: 
                #block doubled access to the database
                semaphore.acquire()
                database = sqlite3.connect("Car_database.db")
                delete_cars_left(database)
                
                #print str(database_tuple[1]) + " " + str(database_tuple[2])
                if(CHANGES_TO_DATABASE==1):
                    cars_list = detected_tags_to_cars(database)
                    if (cars_list and cars_list != []):
                        response = send_post_to_server(json.dumps(cars_list, indent=2),"detected_cars", client_tuple)
                        if (response and response['HTTPRESPONSE']=="1"):
                            CHANGES_TO_DATABASE = 0
                        else:
                            raise Exception, "HTTPRESPONSE!=0 or response is null"
                            
                database.close()
                semaphore.release()
            except Exception,e:
                error_reporting(("ERROR - Server_Connection..." + str(e)))
                
            else:
                time.sleep(2)
        else:
            print "close connection thread..."
            break

def check_error_status():
    global ERROR_REPORT,BLOCK_THREADS
    if(ERROR_REPORT=="INIT"):
        error = "INIT [OK]"
        BLOCK_THREADS = error_to_server(session,False,error)
        ERROR_REPORT = False
    elif (ERROR_REPORT!=False and ERROR_REPORT!="INIT"):
        sys.stdout.write(str(datetime.datetime.utcnow()) + " " + ERROR_REPORT +"\n")
        BLOCK_THREADS = True
    return ERROR_REPORT, BLOCK_THREADS

def update_lamp_software():
    global BLOCK_THREADS
    BLOCK_THREADS = "Update"
           
def error_to_server(session,block,ERROR_REPORT): 
    global sock
    response = send_post_to_server(ERROR_REPORT,"error_report", session)
    if (response and response['HTTPRESPONSE']=="1"):
        if (block==True):
            if (response['BLOCK_THREADS']=="Release"):
                if sock:
                    sock.close()
                block = "Release"
            elif (response['BLOCK_THREADS']=="Update"):
                if sock:
                    sock.close()
                print "prepare for update..."
                block = "Release"
                update_lamp_software()
                time.sleep(10)
    else:
        block = "Release"   
    return block

def check_semaphore():
    try:
        semaphore.release() 
    except:
        pass
              
def init_main_threads():
    global ERROR_REPORT, server_ip
    ERROR_REPORT="INIT"
    
    session = start_session(server_ip)
    if session:
        print str(session)
        sys.stdout.write(init_database()+'\n')
        sys.stdout.write(init_connection_thread(session)+'\n')
        sys.stdout.write(init_BLE_thread()+'\n')
    return session
    

def pre_init(args):
    ##MAIN##
    global CHANGES_TO_DATABASE, semaphore, ERROR_REPORT, BLOCK_THREADS,tags_list,TIME_TO_DELETE_CAR, session
    global server_ip, sock
        
    CHANGES_TO_DATABASE = 0
    semaphore = threading.BoundedSemaphore(value=1)
    ERROR_REPORT="INIT"
    BLOCK_THREADS = False
    tags_list = []
    TIME_TO_DELETE_CAR = 60
    # Very important to check the ID of the device with hciconfig dev!!! Sometimes it changes from 0 to 1 but no exception is dropped...
    arguments = args
    if len(arguments)>1:
        server_ip = str(arguments[1])
    else:
        server_ip = str("127.0.0.1:8000")
    session = init_main_threads()
    return session


#INITIALIZE MAIN THREADS
#pre_init(sys.argv)


    
