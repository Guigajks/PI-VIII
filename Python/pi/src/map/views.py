from django.shortcuts import render

from .models import Mac
# Create your views here.
def index(request):
    return render(request, 'map/index.html', {})

def get_mac(request):
    if request.method == "GET":
        Mac.objects.get(mac_address=)