from channels.routing import ProtocolTypeRouter, URLRouter
import map.routing

application = ProtocolTypeRouter({
    'websocket': URLRouter(
            map.routing.websocket_urlpatterns
        ),
})