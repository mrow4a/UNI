"""sup_web URL Configuration

The `urlpatterns` list routes URLs to views. For more information please see:
    https://docs.djangoproject.com/en/1.8/topics/http/urls/
Examples:
Function views
    1. Add an import:  from my_app import views
    2. Add a URL to urlpatterns:  url(r'^$', views.home, name='home')
Class-based views
    1. Add an import:  from other_app.views import Home
    2. Add a URL to urlpatterns:  url(r'^$', Home.as_view(), name='home')
Including another URLconf
    1. Add an import:  from blog import urls as blog_urls
    2. Add a URL to urlpatterns:  url(r'^blog/', include(blog_urls))
"""
from django.conf.urls import url
from . import views

urlpatterns = [
    url(r'^$', views.index, name='index'),
    url(r'^area/', views.area_map, name='area_map'),
    url(r'^lamphandle/', views.lamphandle, name='lamphandle'),
    url(r'^area_get/', views.area_get, name='area_get'),
    url(r'^parking_get/', views.parking_get, name='parking_get'),
    url(r'^lamp_online_get/', views.lamp_online_get, name='lamp_online_get'),
    url(r'^lamp_register_get/', views.lamp_register_get, name='lamp_register_get'),
    url(r'^parkings/$', views.parking_list), 
    url(r'^parkings/(?P<pk>[0-9]+)$', views.parking_detail),
]
