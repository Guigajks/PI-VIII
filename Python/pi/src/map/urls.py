# map/urls.py
from django.conf.urls import url, re_path

from . import views

urlpatterns = [
    url(r'^', views.index, name='index'),
    re_path(r'^map/(?P<mac>[0-9A-Fa-f]{2}[:-]{5}[0-9A-Fa-f]{2}$)', views.get_mac),
    # url(r'^(?P<room_name>[^/]+)/$', views.room, name='room'),
]