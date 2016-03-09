from django.contrib.gis import admin as gisadmin
from django.contrib import admin
from django.conf.urls import patterns
from .models import Lamp, Area, Parking,Lamp_Register_Queue, Lamp_Error, BLE_Tag
from django.contrib.gis.geoip import GeoIP
from django.contrib.gis.gdal import SpatialReference, CoordTransform
from django.contrib.gis.geos import Point
import urllib2
from . import views
from django.template.loader import render_to_string
import json
from django.utils.html import escape
#__geoip_cache = {}

#def lon_lat(ip, dbname='GeoIP.dat', cache=True, update=True):
#    g = None
#    if cache: g = __geoip_cache.get(dbname)
#    if g is None: 
#        if update_db:
#            update_db(dbname=dbname)
#        g = GeoIP(dbname)
#        __geoip_cache[dbname] = g

#    return g.lon_lat(ip)

#def update_db(src='http://geolite.maxmind.com/download/geoip/database/GeoLiteCity.dat.gz', dbname='GeoIP.dat', thresh=None):
#    import os.path, datetime
#    thresh = thresh or datetime.timedelta(days=7)
#    try:
#        mod_time = datetime.datetime.fromtimestamp(os.path.getctime(dbname))
#    except OSError: mod_time = None
#    cur_time = datetime.datetime.now()
#    if not mod_time or cur_time-mod_time > thresh:
#        import requests
#        r = requests.get(src)
#        data = r.content
#        if src.endswith(".gz"):
#            import gzip, StringIO #io.StringIO doesn't seem to do the job. StringIO.StringIO is legacy.
#            s = StringIO.StringIO(data)
#            data = gzip.GzipFile(fileobj=s).read()
#        open(dbname, "wb").write(data)
   

                
class AreaAdmin(gisadmin.OSMGeoAdmin):
    def __init__(self, *args, **kwargs):
        #try:
            #request = kwargs.pop('request', None)
            #ip = '217.172.243.178'
            #response = urllib2.urlopen('https://freegeoip.net/json/'+ip)
            #data= response.read()
            #print 'Except not found ' + escape(repr(request))
            #data = json.loads(data)
            #lat = data['latitude']
            #lon = data['longitude']
        #except Exception as e:
            #lat = 55.817
            #lon = 12.467
            #print '%s (%s)' % (e.message, type(e))
        lat = 55.817
        lon = 12.467
        ct = CoordTransform(SpatialReference(4326), SpatialReference(3857))
        pbt = Point(lon,lat,srid=4326)
        pbt.transform(ct)
        self.default_lon,self.default_lat = pbt.x,pbt.y   
        return super(AreaAdmin, self).__init__(*args, **kwargs)
    
    
    default_zoom = 11
    display_wkt = False
    

class LampAdmin(gisadmin.OSMGeoAdmin):
    default_zoom = 11
    display_wkt = False
    
class ParkingAdmin(gisadmin.OSMGeoAdmin):
    default_zoom = 11
    display_wkt = False   
    
def index(self, *args, **kwargs):
    sup_extra_context= views.context_render()
    sup_extra_context.update({'admin_area_map': render_to_string('parking/admin_area_map.html',sup_extra_context)})
    return admin.site.__class__.index(self, extra_context=sup_extra_context, *args, **kwargs)

class LampRegisterQueueAdmin(admin.ModelAdmin):
    pass

class LampErrorAdmin(admin.ModelAdmin):
    pass

class CarsAdmin(admin.ModelAdmin):
    pass

admin.site.index = index.__get__(admin.site, admin.site.__class__)
admin.site.register(BLE_Tag, CarsAdmin)
admin.site.register(Lamp, LampAdmin) 
admin.site.register(Parking, ParkingAdmin)    
admin.site.register(Area, AreaAdmin)
admin.site.register(Lamp_Register_Queue, LampRegisterQueueAdmin)
admin.site.register(Lamp_Error, LampErrorAdmin)
    
