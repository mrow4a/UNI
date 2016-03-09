import os
import string
import sqlite3
import time, datetime
import threading, thread
import bluetooth._bluetooth as bluez
import requests
import json
from ble_thread import *
import collections
import socket
import sys

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
        BLOCK_THREADS = True
            
def create_database():
    os.remove("Car_database.db")
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
        sys.stdout.write("ERROR initiating session start.." + str(e)+"\n")
        BLOCK_THREADS = "Release"
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
        return response.json()  
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
            break
            
def error_to_server(session): 
    global ERROR_REPORT, BLOCK_THREADS  
    if(ERROR_REPORT=="INIT"):
        ERROR_REPORT = "INIT [OK]"
    
    elif (ERROR_REPORT!=False and ERROR_REPORT!="INIT"):
        sys.stdout.write(str(datetime.datetime.utcnow()) + " " + ERROR_REPORT +"\n")
    
    response = send_post_to_server(ERROR_REPORT,"error_report", session)
    if (response and response['HTTPRESPONSE']=="1"):
        if (BLOCK_THREADS=="Locked"):
            if (response['BLOCK_THREADS']=="Release"):
                BLOCK_THREADS = "Release"
        ERROR_REPORT = False 
    else:
        BLOCK_THREADS = "Release"             

def check_semaphore():
    try:
        semaphore.release() 
    except:
        pass
              
def init_main_threads():
    global ERROR_REPORT, server_ip
    ERROR_REPORT="INIT"
    BLOCK_THREADS = False
    
    session = start_session(server_ip)
    if session:
        print str(session)
        sys.stdout.write(init_database()+'\n')
        sys.stdout.write(init_connection_thread(session)+'\n')
        sys.stdout.write(init_BLE_thread()+'\n')
    return session
    


def pre_init():
    ##MAIN##
    global CHANGES_TO_DATABASE, semaphore, ERROR_REPORT, BLOCK_THREADS,tags_list,TIME_TO_DELETE_CAR, session
    global server_ip
    CHANGES_TO_DATABASE = 0
    semaphore = threading.BoundedSemaphore(value=1)
    ERROR_REPORT="INIT"
    BLOCK_THREADS = False
    tags_list = []
    TIME_TO_DELETE_CAR = 60
    # Very important to check the ID of the device with hciconfig dev!!! Sometimes it changes from 0 to 1 but no exception is dropped...
    arguments = sys.argv
    get_lock('Car_discovery')
    if len(arguments)>1:
        server_ip = str(arguments[1])
    else:
        server_ip = str("127.0.0.1:8000")
    session = init_main_threads()
    
    while (True):
        error_to_server(session)
        if (BLOCK_THREADS == True):
            BLOCK_THREADS = "Locked"
        elif(BLOCK_THREADS == "Release"):
            BLOCK_THREADS = False
            session = init_main_threads()
            print "session: " + str(session)
        time.sleep(10)  


#INITIALIZE MAIN THREADS
pre_init()


    
