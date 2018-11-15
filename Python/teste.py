from threading import Thread
import traceback
from netifaces import ifaddresses, AF_INET
import calculadora_posicao as cp
import socket 

LISTENING_IP = socket.gethostbyname(ifaddresses('wlp1s0')[AF_INET][0]['addr'])
LISTENING_PORT = 10000
BUFFER_SIZE = 1024

def listen_TCP():
	sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM) # IPv4, TCP
	sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
	sock.bind((LISTENING_IP, LISTENING_PORT))

	while True:
		sock.listen(1)
		conn, address = sock.accept()
		# print ("TCP connection from", address)
		
		data = conn.recv(BUFFER_SIZE)
		print(data)
		if data:
			try:
				# print(data.decode('ascii'))
				data = cp.calcula_raio(2422,int(data.decode('utf-8')))
				print(data)
				# print(tst)
				# sock.send(b'ok')
			except Exception as ex:
				print(ex)
				# traceback.print_exc()
			finally:
				conn.send(b'ok')
				conn.close()


if __name__ == "__main__":
	Thread_TCP = Thread(target=listen_TCP)

	print ("Starting Server...")
	Thread_TCP.start()
	print ("Server Started!")