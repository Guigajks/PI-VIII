from django.shortcuts import render, HttpResponse, get_object_or_404

from .models import Mac
# Create your views here.
def index(request):
	return render(request, 'map/index.html', {})

def get_mac(request, mac):
	print(mac)
	if request.method == "GET":
		try:
			obj = Mac.objects.filter(mac_address = mac.lower())
		# obj = get_object_or_404(Mac, mac_address=mac)
			if obj.exists():
				return HttpResponse('ok', content_type="text/plain")
			else:
				return HttpResponse('not ok', content_type="text/plain")
		except:
			# return HttpResponse('not ok', content_type="text/plain")
			print('erro')