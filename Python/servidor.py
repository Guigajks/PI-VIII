# source code https://stackoverflow.com/questions/29022329/basic-udp-tcp-program

from threading import Thread
import websocket
import traceback
# import asyncio
from netifaces import ifaddresses, AF_INET
import socket 
import calculadora_posicao as cp
import json

# LISTENING_IP = socket.gethostbyname(socket.gethostbyname(socket.gethostname()))
LISTENING_IP = socket.gethostbyname(ifaddresses('wlp1s0')[AF_INET][0]['addr'])
LISTENING_PORT = 10000
BUFFER_SIZE = 1024

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

	while True:
		data, address = sock.recvfrom(BUFFER_SIZE)
		print ("UDP Messsage from address: ", address)
		print ("Message: ", data)
		print("Send to:", address)
		sock.sendto(data,address)


def listen_TCP():
	sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM) # IPv4, TCP
	sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
	sock.bind((LISTENING_IP, LISTENING_PORT))

	# ws = websocket.create_connection("ws://localhost:8000/ws/")
	# websocket.enableTrace(True) 

	while True:
		sock.listen(1)
		conn, address = sock.accept()
		print ("TCP connection from", address)
		
		data = conn.recv(BUFFER_SIZE)
		if data:
			try:
				# print(data.decode('ascii'))
				tst = parse_data(data.decode('utf-8'))
				print(tst)
				# sock.send(b'ok')
				# ws.send(tst)
			except Exception as ex:
				print(ex)
				# traceback.print_exc()
			finally:
				conn.send(b'ok')
				conn.close()
	# ws.close()


if __name__ == "__main__":
	Thread_UDP = Thread(target=listen_UDP)
	Thread_TCP = Thread(target=listen_TCP)

	print ("Starting Server...")
	Thread_UDP.start()
	Thread_TCP.start()
	print ("Server Started!")