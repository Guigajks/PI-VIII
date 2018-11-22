# source code https://stackoverflow.com/questions/29022329/basic-udp-tcp-program

from threading import Thread
from netifaces import ifaddresses, AF_INET
import socket 
import websocket

import calculadora_posicao as cp
import json

import traceback
import re

# LISTENING_IP = socket.gethostbyname(socket.gethostbyname(socket.gethostname()))
LISTENING_IP = socket.gethostbyname(ifaddresses('wlp1s0')[AF_INET][0]['addr'])
LISTENING_PORT = 10000
BUFFER_SIZE = 1024
WEBSOCKET = "ws://localhost:8000/ws/"

re_door = re.compile(r'^([0-9A-Fa-f]{2}[:-]){5}([0-9A-Fa-f]{2})$') # 20:e8:17:05:d7:69
re_data = re.compile(r"""^
	# 20:e8:17:05:d7:69>A1P1-50;A1P2-48.05;A1P3-46.05
	([0-9A-Fa-f]{2}[:-]){5}([0-9A-Fa-f]{2}) #20:e8:17:05:d7:69
	> #>
	(\w+\-((\d){2,3}((\.)(\d){2,3})?);){2} #A1P1-50;A1P2-48.05;
	(\w+\-((\d){2,3}((\.)(\d){2,3})?)) #A1P3-46.05
""")

def parse_data(data):
	print(data)
	# data = '20:e8:17:05:d7:69>A1P1-50;A1P2-48.05;A1P3-46.05'
	mac_address = data.split('>')[0]
	# print(mac_address, 'print 1')
	tmp = data.split('>')[1].split(';')
	tmp.sort()
	networks = [
		(
			cp.retorna_posicao(tst.split('-')[0]), 
			cp.calcula_raio(2400.0,float(tst.split('-')[1]))
		) for tst in tmp
	]
	print(networks)
	return json.dumps({ 'mac_address': mac_address, 'trilateracao': cp.trilateracao(networks) })

def listen_UDP():

	sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM) # IPv4, UDP
	sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
	sock.bind((LISTENING_IP, LISTENING_PORT))
	
	ws = websocket.create_connection(WEBSOCKET)

	while True:
		data, address = sock.recvfrom(BUFFER_SIZE)
		data = data.decode('utf-8')
		print ("UDP Messsage from address: ", address)
		print ("Message: ", data)
		try:
			if re_door.search(data):
				sock.sendto('ok', address)
				# TODO
					# request
			elif re_data.search(data):
				data = parse_data(data)
				if ws.connected:
					ws.send(data)
				else:
					ws.connect(WEBSOCKET)
					ws.send(data)
		except Exception as ex:
			print(ex)
			# traceback.print_exc()

# def listen_TCP():
# 	sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM) # IPv4, TCP
# 	sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
# 	sock.bind((LISTENING_IP, LISTENING_PORT))

# 	# websocket.enableTrace(True) 

# 	while True:
# 		sock.listen(1)
# 		conn, address = sock.accept()
# 		print ("TCP connection from", address)
		
# 		data = conn.recv(BUFFER_SIZE)
# 		if data:
# 			try:
# 				# print(data.decode('ascii'))
# 				tst = parse_data(data.decode('utf-8'))
				
# 				print(tst)
# 				# sock.send(b'ok')
# 				ws.send(tst)
# 			except Exception as ex:
# 				print(ex)
# 				# traceback.print_exc()
# 			finally:
# 				conn.send(b'ok')
# 				conn.close()
# 	# ws.close()


if __name__ == "__main__":
	Thread_UDP = Thread(target=listen_UDP)
	# Thread_TCP = Thread(target=listen_TCP)

	print ("Starting Server...")
	Thread_UDP.start()
	# Thread_TCP.start()
	print ("Server Started!")