from django.contrib.gis.db import models as gismodels
from django.db import models
from django.core.validators import MaxValueValidator, MinValueValidator
from django.utils import timezone
from django.db.models import Q
import datetime 
from parking.circles import get_intersection_2p
from django.contrib.gis.geos import Point,Polygon

def if_online(time_delta=None):
    curdate = timezone.now()
    if time_delta:
        delta = curdate - datetime.timedelta(seconds=time_delta)
    else:
        delta = curdate - datetime.timedelta(seconds=60)
    return delta

class Area(gismodels.Model):
    CITY = '12'
    AREA = '14'
    PARKING = '17'
    AREA_CHOICES = (
        (CITY, 'City'),
        (AREA, 'Area'),
        (PARKING, 'Parking'),
    )
    name = models.CharField(max_length=256, default='World', blank=True, db_column='name', primary_key=True)
    area_type = models.CharField(('area zoom'), max_length=2, choices=AREA_CHOICES, default=CITY, db_column='area_type', blank=True)
    geom = gismodels.PointField()
    objects = gismodels.GeoManager()

    def __unicode__(self):
        return self.name

class Lamp(gismodels.Model):
    lamp_number = models.CharField(max_length=256, default='None', blank=True, db_column='lamp_number')
    max_range = models.IntegerField(default=20,
        validators=[
            MaxValueValidator(100),
            MinValueValidator(1)
        ])
    online_timestamp = models.DateTimeField(default=timezone.now, blank=True)
    geom = gismodels.PointField()
    objects = gismodels.GeoManager()
    
    def __unicode__(self):
        return unicode(self.id)


    
class BLE_Tag(gismodels.Model):
    plate_number = models.CharField(max_length=256, default='None', blank=True, db_column='plate_number')
    lamp_location = models.ForeignKey(Lamp, null=True)
    distance = models.FloatField(default=0,
        validators=[
            MaxValueValidator(1000),
            MinValueValidator(0)
        ])
    timestamp = models.DateTimeField(default=timezone.now, blank=True)
    def __unicode__(self):
        return unicode(self.plate_number)

class ParkingManager(models.Manager):
    def occupancy(self, parkingid=None):
        TIME_TO_DELETE_CAR = 10000
        count = 0 
        if parkingid == None:
            lamplocation = self.lamp_location
        else:
            lamplocation = self.filter(id=parkingid).values_list('lamp_location')[0]
        tags_found = BLE_Tag.objects.filter(Q(lamp_location=lamplocation) & Q(timestamp__gt = if_online(TIME_TO_DELETE_CAR)))
        if tags_found:  
            pn = tags_found.values_list('plate_number')
            if pn:
                for tag_pn in pn:
                    multiple_tag_detection = BLE_Tag.objects.filter(Q(plate_number=tag_pn[0]) & Q(timestamp__gt = if_online(TIME_TO_DELETE_CAR))).order_by('distance')
                    #print str(multiple_tag_detection.values_list('plate_number','distance', 'lamp_location__id'))
                    if multiple_tag_detection.count()==1:
                        count+=1   
                    elif multiple_tag_detection.count()>1:
                        result = []
                        lamp_loc_list = multiple_tag_detection.values_list('lamp_location__geom')
                        for geom in lamp_loc_list:
                            result.append(geom[0])
                        #lamps_locs = Lamp.objects.filter(pk__in = [list[0],list[1]])
                        distance = multiple_tag_detection.values_list("distance", flat=True)
                        result_points = get_intersection_2p(result[0].y,result[0].x,distance[0],result[1].y,result[1].x,distance[1])
                        #print result_points
                        #if (multiple_tag_detection.values_list("lamp_location")[0] == lamplocation):
                        if result_points:
                            geom_point1=Point(float(result_points[0][1]), float(result_points[0][0]))
                            geom_point2=Point(float(result_points[1][1]), float(result_points[1][0]))
                            if multiple_tag_detection.count()==2:
                                exact_parking = Parking.objects.filter(Q(geom__intersects=geom_point1) & Q(geom__intersects=geom_point2) & Q(lamp_location=lamplocation))                           
                                if (exact_parking.exists()):
                                    count+=1
                                else:
                                    probable_parking = Parking.objects.filter((Q(geom__intersects=geom_point1) | Q(geom__intersects=geom_point2)) & Q(lamp_location=lamplocation))
                                    if (probable_parking.exists()):
                                        count+=1
                                
                            elif multiple_tag_detection.count()>2:
                                geom_point3=Point(float(result[2].x), float(result[2].y))
                                if(abs(geom_point1.distance(geom_point3)-distance[2])>abs(geom_point2.distance(geom_point3)-distance[2])):
                                    check_point = geom_point2
                                else:
                                    check_point = geom_point1
                                exact_parking = Parking.objects.filter(Q(geom__intersects=check_point) & Q(lamp_location=lamplocation))                           
                                if (exact_parking.exists()):
                                    count+=1
                                #count += 1
                                    
                                
                                    
        return count

        
class Parking(gismodels.Model):
    lamp_location = models.ForeignKey(Lamp, null=True)
    capacity = models.IntegerField(default=1,
        validators=[
            MaxValueValidator(1000),
            MinValueValidator(0)
        ])
    geom = gismodels.PolygonField()
    objects = gismodels.GeoManager()
    parking_objects = ParkingManager()
    
    
    def __unicode__(self):
        return unicode(self.id)
   
class Lamp_Register_Queue(models.Model):
    lamp_number = models.CharField(max_length=256, default='None', blank=True, db_column='lamp_number')

    def __unicode__(self):
        return unicode(self.lamp_number)    
    
class Lamp_Error(models.Model):
    lamp_location = models.ForeignKey(Lamp, null=True)
    timestamp = models.DateTimeField(default=timezone.now, blank=True)
    error_status = models.CharField(max_length=256, default='None', blank=True, db_column='error_status')
    reset_signal = models.BooleanField(default=False)
    def __unicode__(self):
        return unicode(str(self.lamp_location) + ": " + str(self.timestamp) + " " + str(self.error_status))        