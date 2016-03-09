from rest_framework import serializers
from .models import Parking
from rest_framework_gis.serializers import GeoFeatureModelSerializer

class ParkingSerializer(GeoFeatureModelSerializer):
    occupancy = serializers.SerializerMethodField()
    
    class Meta:
        model = Parking
        geo_field = "geom"
        fields = ('id', 'lamp_location', 'capacity','occupancy', 'geom')
        
    def get_occupancy(self, obj):
        return Parking.parking_objects.occupancy(obj.id)