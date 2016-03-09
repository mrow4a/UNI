from sympy.geometry import *
from sympy import *
from decimal import *

def deg_meters_point(lon, lat, latMid):
    return Point(m_per_deg_lon(latMid) * lon, m_per_deg_lat(latMid) * lat).evalf()

def m_per_deg_lat(latMid):
    return 111132.954 - 559.822 * cos( 2.0 * latMid ) + 1.175 * cos( 4.0 * latMid)

def m_per_deg_lon(latMid):
    return 111412.84*cos(latMid ) - 93.5*cos( 3.0 * latMid )  - 0.118 * cos( 5.0 * latMid)

def get_intersection_2p(ay,ax,ad,by,bx,bd):
    INIT_PREC = 10
    while(true):
        if INIT_PREC>0: 
            getcontext().prec = INIT_PREC
            deltaLat = Decimal(by) - Decimal(ay)
            deltaLon = Decimal(bx) - Decimal(ax)
                
            deltaLat = Decimal(deltaLat)
            deltaLon = Decimal(deltaLon)
            a = Point(0, 0)
            b = deg_meters_point(deltaLon, deltaLat,ay)
            distance = abs(a.distance(b).evalf())
            ca = Circle(a, ad)
            cb = Circle(b, bd)
            #print str(ay)+" "+str(ax)+" "+str(by)+" "+str(bx)
            #print distance
            result = intersection(ca,cb)
            if len(result)==2:
                result1 = result[0].evalf()
                result2 = result[1].evalf()
                
                #print str(result1.y)+" "+str(result1.x)+" "+str(result2.y)+" "+str(result2.x)
                return ((ay+result1.y/m_per_deg_lat(ay),ax+result1.x/m_per_deg_lon(ay)),(ay+result2.y/m_per_deg_lat(ay),ax+result2.x/m_per_deg_lon(ay)))
                #return ((ay + result1.y/m_per_deg_lat(ay),ax + result1.x/m_per_deg_lon(ay)),(ay + result2.y/m_per_deg_lat(ay),ax + result2.x/m_per_deg_lon(ay)))
            elif distance<(ad+bd):
                INIT_PREC-=1
            elif len(result)==0 and distance>=(ad+bd):
                return 0
            elif distance>=(ad+bd):
                return 0
            else:
                return 0
        else:
            return 0
