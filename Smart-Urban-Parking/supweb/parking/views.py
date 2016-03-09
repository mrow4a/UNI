from django.shortcuts import render,get_object_or_404
from parking.models import Area, Lamp, Parking,Lamp_Register_Queue, Lamp_Error, BLE_Tag
import json
from django.http import HttpResponse
from django.contrib.gis.geos import Point,Polygon
from django.template.loader import render_to_string
from django.core.serializers.json import DjangoJSONEncoder
import datetime 
from django.utils import timezone
from django.db.models import Q
from rest_framework import status
from rest_framework.decorators import api_view
from rest_framework.response import Response
from parking.serializers import ParkingSerializer


def context_render(custom_script=None):
    if custom_script==None:
        custom_script='parking/map_handle_script.js'
        
    area="World"
    p = get_object_or_404(Area, name=area)
    lon,lat = p.geom.centroid.x, p.geom.centroid.y
    area_type = p.area_type
    lamp_points = Lamp.objects.all()  # @UndefinedVariable
    parking_points = Parking.objects.all()  # @UndefinedVariable
    area_list = Area.objects.raw('SELECT name as name FROM parking_area')  # @UndefinedVariable
    context = {'map_handle_script': render_to_string(custom_script, 
                                               {'area_type': area_type,
                                                'area': area,
                                                'lon': lon,
                                                'lat': lat,
                                                'parking_points':parking_points,
                                                'lamp_points':lamp_points,
                                                'area_list': area_list,
                                                }),
               } 
    return context

def if_online(time_delta=None):
    curdate = timezone.now()
    if time_delta:
        delta = curdate - datetime.timedelta(seconds=time_delta)
    else:
        delta = curdate - datetime.timedelta(seconds=60)
    return delta

def index(request):
    context=context_render('parking/client_map_handle_script.js')
    context.update({'area_map': render_to_string('parking/area_map.html',context)})
    return render(request, 'parking/index.html', context)

def area_map(request):
    return render(request, 'parking/area_map.html', context_render())

def area_get(request):
    try:
        data = json.loads(request.GET['content'])
        if data:
            p = get_object_or_404(Area, name=data)
            lon,lat = p.geom.centroid.x, p.geom.centroid.y
            area_type = p.area_type
            json_data_response = json.dumps({'HTTPRESPONSE': '1'
                                         , 'lon': lon
                                         , 'lat': lat
                                         , 'area_type': area_type})
            return HttpResponse(json_data_response)
    
    except Exception,e:
        json_data_response = json.dumps({'HTTPRESPONSE': str(e)})
        return HttpResponse(json_data_response)
    
def parking_get(request):
    try:
        data = json.loads(request.GET['content'])
        if data:
            occupancy = Parking.parking_objects.occupancy(data)
            json_data_response = json.dumps({'HTTPRESPONSE': '1', 'parking': occupancy })
            return HttpResponse(json_data_response)
        
    except Exception,e:
        json_data_response = json.dumps({'HTTPRESPONSE': str(e)})
        return HttpResponse(json_data_response)


def lamp_online_get(request):
    try:
        data = json.loads(request.GET['content'])
        if data:
            l = get_object_or_404(Lamp, id=data)
            
            if (l.online_timestamp <= if_online()):
                online = False
            else:
                online = True
            json_data_response = json.dumps({'HTTPRESPONSE': '1', 'online': online })
            return HttpResponse(json_data_response)
        
    except Exception,e:
        json_data_response = json.dumps({'HTTPRESPONSE': str(e)})
        return HttpResponse(json_data_response)
    
def delete_reg(data):      
    eth0 = data
    lamp_waits = (Lamp_Register_Queue.objects.filter(lamp_number__iexact=eth0).exists())
    lamp_exists = (Lamp.objects.filter(lamp_number__iexact=eth0).exists())
    if (lamp_exists and lamp_waits):
        Lamp_Register_Queue.objects.filter(lamp_number__iexact=eth0).delete()  

def car_detection(Plate_Number,Distance,lamp):
    tag_exists = (BLE_Tag.objects.filter(Q(lamp_location=lamp.id) & Q(plate_number=Plate_Number)))
    if not tag_exists.exists():
        BLE_Tag.objects.create(timestamp = timezone.now(), lamp_location = lamp, plate_number = Plate_Number, distance = Distance)  # @UndefinedVariable
    else:
        tag_exists.update(timestamp = timezone.now(), distance = Distance)
    
def add_reg(eth0, error, detected_cars, reset_sig):
    if error:
        lamp = get_object_or_404(Lamp, lamp_number=eth0)
        Lamp_Error.objects.create(lamp_location=lamp, error_status=error)  # @UndefinedVariable

    elif detected_cars:
        Plate_Number = detected_cars['Plate_Number']
        Distance = detected_cars['Distance']  
        lamp = get_object_or_404(Lamp, lamp_number=eth0)
        car_detection(Plate_Number,Distance,lamp)
    elif reset_sig:
        lamp = get_object_or_404(Lamp, lamp_number=eth0)
        Lamp_Error.objects.create(lamp_location=lamp, reset_signal=reset_sig)  # @UndefinedVariable
    else:
        lamp_exists = (Lamp.objects.filter(lamp_number__iexact=eth0).exists())
        lamp_waits = (Lamp_Register_Queue.objects.filter(lamp_number__iexact=eth0).exists())
        if not (lamp_exists or lamp_waits):
            Lamp_Register_Queue.objects.create(lamp_number=eth0)  # @UndefinedVariable
            return 0
        elif lamp_waits:
            return 0
        else:
            return eth0
   
def lamp_register_get(request):
    if request.method == 'GET':
        try:
            data = json.loads(request.GET["content"]) 
            if data =="show":
                lamp_register_points = Lamp_Register_Queue.objects.all().values_list('lamp_number')  # @UndefinedVariable
                json_data_response = json.dumps({'HTTPRESPONSE': '1', 'lamp_register_points': json.dumps(list(lamp_register_points), cls=DjangoJSONEncoder) })
                return HttpResponse(json_data_response)
            elif data =="count":
                lamp_register_wait_count = len(Lamp_Register_Queue.objects.all())  # @UndefinedVariable
                lamps = Lamp.objects.all()
                lamp_register_error_count = 0
                lamp_error_dict = {}
                for lamp in lamps:
                    lamp_error = Lamp_Error.objects.filter(Q(lamp_location = lamp.id) & Q(lamp_location__online_timestamp__gt = if_online())).order_by('-timestamp')
                    if(lamp_error):
                        lamp_error = lamp_error[0]
                        if (lamp_error.error_status != "INIT [OK]"):
                            lamp_error_dict[str(lamp_error.lamp_location)]=str(lamp_error.error_status)
                            lamp_register_error_count+=1  
                        else:
                            lamp_error_dict[str(lamp_error.lamp_location)]="ONLINE"
                    if(lamp.online_timestamp <= if_online()):
                        lamp_error_dict[str(lamp.id)]="OFFLINE"   
                lamp_register_online_count = len(Lamp.objects.filter(online_timestamp__lte = if_online())) # @UndefinedVariable
                
                json_data_response = json.dumps({'HTTPRESPONSE': '1', 'lamp_register_wait_count': lamp_register_wait_count, 'lamp_register_online_count': lamp_register_online_count, 'lamp_register_error_count':lamp_register_error_count,'lamp_error_dict':lamp_error_dict })
                return HttpResponse(json_data_response)
            
            
        except Exception,e:
            json_data_response = json.dumps({'HTTPRESPONSE': str(e)})
            return HttpResponse(json_data_response)
    elif request.method == 'POST':
        try:
            content_type = json.loads(request.body)['content_type']
            content_tuple = json.loads(request.body)['content']
            eth0 = content_tuple[0]
            content = content_tuple[1]
            if content_type == "set_queue":
                eth0 = add_reg(eth0,0,0,0)
                json_data_response = json.dumps({'HTTPRESPONSE': "1", 'Eth0': eth0})
                return HttpResponse(json_data_response)
            elif content_type == "detected_cars":
                add_reg(eth0,0,json.loads(content)[0],0)
                json_data_response = json.dumps({'HTTPRESPONSE': "1"})
                return HttpResponse(json_data_response)
            elif content_type == "error_report":
                content = content_tuple[1]
                lamp = get_object_or_404(Lamp, lamp_number=eth0)
                lamp.online_timestamp = timezone.now()
                lamp.save()
                if (content!=False):
                    add_reg(eth0, content,0,0)
                lamp_error = Lamp_Error.objects.filter(Q(reset_signal = True) & Q(lamp_location = lamp.id) & Q(lamp_location__online_timestamp__gt = if_online())).exists()
                if (lamp_error):
                    BLOCK_THREADS = "Release"
                    Lamp_Error.objects.filter(Q(reset_signal = True) & Q(lamp_location = lamp.id) & Q(lamp_location__online_timestamp__gt = if_online())).delete()
                else:
                    BLOCK_THREADS = True
                    
                json_data_response = json.dumps({'HTTPRESPONSE': "1", 'BLOCK_THREADS' : str(BLOCK_THREADS)})
                return HttpResponse(json_data_response)
                
            
        except Exception,e:
            if e:
                json_data_response = json.dumps({'HTTPRESPONSE': str(e)})
            else:
                json_data_response = json.dumps({'HTTPRESPONSE': "ERROR"})  
            return HttpResponse(json_data_response) 
    
def lamphandle(request):
    try:
        data = json.loads(request.POST["content"]) 
        data_type = json.loads(request.POST["content_type"]) 
        if data and data_type=="edit_lamp":
            for lamp_json in data:
                lamp_id=int(lamp_json['id'])
                try:
                    lamp = get_object_or_404(Lamp,id = lamp_id)  # @UndefinedVariable
                    lamp.geom = Point(lamp_json['lng'], lamp_json['lat'], srid=4326)
                    lamp.max_range = int(lamp_json['max_range'])
                    lamp.lamp_number = lamp_json['lamp_number']
                    lamp.save()
                    delete_reg(lamp.lamp_number)
                    json_data_response = json.dumps({'HTTPRESPONSE': '1'})
                    return HttpResponse(json_data_response)
                except Exception,e:
                    json_data_response = json.dumps({'HTTPRESPONSE': str(e)})
                    return HttpResponse(json_data_response)
        elif data and data_type=="add_lamp":
            for lamp_json in data:
                try:
                    lamp = Lamp.objects.create(lamp_number = lamp_json['lamp_number'], max_range=lamp_json['max_range'],geom=Point(lamp_json['lng'], lamp_json['lat'], srid=4326))  # @UndefinedVariable
                    delete_reg(lamp_json['lamp_number'])
                    json_data_response = json.dumps({'HTTPRESPONSE': '1', 'id': lamp.id})
                    return HttpResponse(json_data_response)
                except Exception,e:
                    json_data_response = json.dumps({'HTTPRESPONSE': str(e)})
                    return HttpResponse(json_data_response)
        elif data and data_type=="add_parking":
            for parking_json in data:
                parking_lamp_location = get_object_or_404(Lamp, id=int(parking_json['lamp_location']))
                parking_lamp_capacity = int(parking_json['capacity'])
                parking_lamp_geom = Polygon(tuple(tuple(x) for x in parking_json['geom']))
                parking_lamp_geom.srid=4326
                try:
                    parking = Parking.objects.create(capacity=parking_lamp_capacity,lamp_location=parking_lamp_location,geom=parking_lamp_geom)  # @UndefinedVariable
                    json_data_response = json.dumps({'HTTPRESPONSE': "1", 'id': parking.id })
                    return HttpResponse(json_data_response)
                except Exception,e:
                    json_data_response = json.dumps({'HTTPRESPONSE': str(e)})
                    return HttpResponse(json_data_response)
        elif data and data_type=="edit_parking":
            for parking_json in data:
                parking_lamp_id=int(parking_json['id'])
                parking_lamp_location = get_object_or_404(Lamp, id=int(parking_json['lamp_location']))
                parking_lamp_capacity = int(parking_json['capacity'])
                parking_lamp_geom = Polygon(tuple(tuple(x) for x in parking_json['geom']))
                parking_lamp_geom.srid=4326
                try:
                    parking = get_object_or_404(Parking, id = parking_lamp_id)  # @UndefinedVariable
                    parking.capacity = parking_lamp_capacity
                    parking.lamp_location = parking_lamp_location
                    parking.geom = parking_lamp_geom
                    parking.save()
                    json_data_response = json.dumps({'HTTPRESPONSE': '1'})
                    return HttpResponse(json_data_response)
                except Exception,e:
                    json_data_response = json.dumps({'HTTPRESPONSE': str(e)})
                    return HttpResponse(json_data_response)
        elif data and data_type == "set_queue":
                json_data_response = json.dumps({'HTTPRESPONSE': "1"})
                return HttpResponse(json_data_response)
        elif data and data_type == "reset_sig":
                add_reg(data,0,0,True)
                json_data_response = json.dumps({'HTTPRESPONSE': "1"})
                return HttpResponse(json_data_response)
    except Exception,e:
        json_data_response = json.dumps({'HTTPRESPONSE': str(e)})
        return HttpResponse(json_data_response)
    
@api_view(['GET'])
def parking_list(request):
    if request.method == 'GET':
        parkings = Parking.objects.all()
        serializer = ParkingSerializer(parkings, many=True)
        return Response(serializer.data)
    
@api_view(['GET'])
def parking_detail(request, pk):
    try:
        parking = Parking.objects.get(pk=pk)
    except Parking.DoesNotExist:
        return Response(status=status.HTTP_404_NOT_FOUND)

    if request.method == 'GET':
        serializer = ParkingSerializer(parking)
        return Response(serializer.data) 
    
