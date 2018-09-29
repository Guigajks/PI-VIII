# source code https://stackoverflow.com/questions/29022329/basic-udp-tcp-program

from threading import Thread
from netifaces import ifaddresses, AF_INET
import socket 
import calculadora_posicao as cp
import sys

LISTENING_IP = socket.gethostbyname(ifaddresses('wlan0')[AF_INET][0]['addr'])
LISTENING_PORT = 5000
BUFFER_SIZE = 1024

mac = '18:FE:34:CB:35:70'

def parse_data(data):
    try:
        teste = '20:e8:17:05:d7:69>A0P0-50;A0P1-48.05;A0P2-46.05'
        mac_address = teste.split('>')[0]

        tmp = teste.split('>')[1].split(';')
        networks = [(cp.retorna_posicao(tst.split('-')[0]), cp.calcula_raio(2400.0,float(tst.split('-')[1]))) for tst in tmp]

        return (mac_address, cp.trilateracao(networks))
    except Exception as ex:
        print ex
        return "Error"

def listen_UDP():

    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM) # IPv4, UDP
    sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    sock.bind((LISTENING_IP, LISTENING_PORT))

    while True:
        data, address = sock.recvfrom(BUFFER_SIZE)
        print ("UDP Messsage from address: ", address)
        print ("Message: ", data+'oi')
        print("Send to:", address)
        sock.sendto(data+'oi',address)

def listen_TCP():

    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM) # IPv4, TCP
    sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    sock.bind((LISTENING_IP, LISTENING_PORT))

    while True:
        sock.listen(1)
        conn, address = sock.accept()
        print ("TCP connection from", address)
        data = conn.recv(BUFFER_SIZE)
        data = parse_data(data)
        print ("Mesage: ", data)
        conn.close()

if __name__ == "__main__":
    Thread_UDP = Thread(target=listen_UDP)
    Thread_TCP = Thread(target=listen_TCP)

    print ("Starting Server...")
    Thread_UDP.start()
    Thread_TCP.start()
    print ("Server Started!")
