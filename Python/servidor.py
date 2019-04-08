# source code https://stackoverflow.com/questions/29022329/basic-udp-tcp-program

from threading import Thread
from netifaces import ifaddresses, AF_INET
import socket
import websocket
import requests

import calculadora_posicao as cp
import json

import traceback
import re

# LISTENING_IP = socket.gethostbyname(socket.gethostbyname(socket.gethostname()))
LISTENING_IP = socket.gethostbyname(ifaddresses('wlp1s0')[AF_INET][0]['addr'])
print(LISTENING_IP)
LISTENING_PORT = 10000
LISTENING_PORT2 = 10001
BUFFER_SIZE = 1024
WEBSOCKET = "ws://localhost:8000/ws/"

# declarando variável global para que as threads possam acessar o mesmo ws
ws= None

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
			cp.calcula_raio(2400.0, float(tst.split('-')[1]))
		) for tst in tmp
	]
	print(networks)
	return json.dumps({'mac_address': mac_address, 'trilateracao': cp.trilateracao(networks)})

def listen_UDP1():
	print('Starting UDP socket 1')
	global ws
	sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
	sock.bind((LISTENING_IP, LISTENING_PORT))

	while True:
		data, address = sock.recvfrom(50)
		data = data.decode('utf-8')
		try:
			if data.find('>') != -1:
				data = parse_data(data)
				print(data)
				if ws.connected:
					ws.send(data)
				else:
					ws = websocket.create_connection(WEBSOCKET)
					ws.send(data)

		except Exception as ex:
			print(ex)

# def listen_UDP2():
# 	print('Starting UDP socket 2')
# 	global ws
# 	sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
# 	sock.bind((LISTENING_IP, LISTENING_PORT2))

# 	while True:
# 		data, address = sock.recvfrom(50)
# 		data = data.decode('utf-8')
# 		print('Dado recebido UDP 2',data)
# 		try:
# 			if len(data) == 17:
# 				r = requests.get(f'http://localhost:8000/map/{data}')
# 				print(r.content)
# 				if r.content == b"ok":
# 					sock.sendto(r.content, address)
# 				elif r.content == b"not ok":
# 					if ws.connected:
# 						ws.send(r.content)
# 					else:
# 						ws = websocket.create_connection(WEBSOCKET)
# 						ws.send(r.content)

# 		except Exception as ex:
# 			print(ex)


if __name__ == "__main__":

	# iniciando o ws globalmente
	ws = websocket.create_connection(WEBSOCKET)

	# iniciando os dois sockets UDP, um para recebimento da mensagem para calculo das distâncias,
	# e outro para o MAC address
	Thread_UDP = Thread(target=listen_UDP1)
	# Thread_UDP2 = Thread(target=listen_UDP2)

	print ("Starting Server...")
	Thread_UDP.start()
	# Thread_UDP2.start()

	print ("Server Started!")
