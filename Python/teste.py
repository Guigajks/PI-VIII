from threading import Thread
import traceback
from netifaces import ifaddresses, AF_INET
import socket 
import pandas as pd
from functools import reduce

LISTENING_IP = socket.gethostbyname(ifaddresses('wlp1s0')[AF_INET][0]['addr'])
LISTENING_PORT = 10000
BUFFER_SIZE = 1024
# WEBSOCKET = "ws://localhost:8000/ws/"

tst = []
df = pd.DataFrame()

def listen_UDP():

	sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM) # IPv4, UDP
	sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
	sock.bind((LISTENING_IP, LISTENING_PORT))
	# ws = websocket.create_connection(WEBSOCKET)

	while True:
		data, address = sock.recvfrom(BUFFER_SIZE)
		data = data.decode('utf-8')
		try:
			if data != '':
				print(data)
				tst.append(int(data))

				if len(tst) == 300:
					nome_arquivo = input('Nome do arquivo:\n')
					
					media = float(reduce(lambda x,y: x + y, tst)) / float(len(tst))
					tst.append(media)
					tst.append(float(nome_arquivo))

					df[nome_arquivo] = tst
					tst.clear()
					
					sock.close()
					opcao = int(input('Continuar? 1 - Sim, 0 - Não:\n'))					
					if opcao:
						sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
						sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
						sock.bind((LISTENING_IP, LISTENING_PORT))
						continue
					else:
						df.to_csv('amostras7.csv', index=False, encoding='utf-8') 
						quit()

		except Exception as ex:
			print(ex)
			# traceback.print_exc()

if __name__ == "__main__":
	Thread_UDP = Thread(target=listen_UDP)
	# Thread_TCP = Thread(target=listen_TCP)

	print ("Starting Server...")
	Thread_UDP.start()
	# Thread_TCP.start()
	print ("Server Started!")