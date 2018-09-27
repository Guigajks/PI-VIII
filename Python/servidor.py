# source code https://stackoverflow.com/questions/29022329/basic-udp-tcp-program

from threading import Thread
from netifaces import ifaddresses, AF_INET
import socket 

LISTENING_IP = socket.gethostbyname(ifaddresses('wlan0')[AF_INET][0]['addr'])
LISTENING_PORT = 5000
BUFFER_SIZE = 1024

def parse_data(data):
    try:

        mac_address = data.split('>')[0]
        networks = [tuple(dado.split('-')) for dado in data.split(';')[1]]

        return (mac_address, networks)
    except:
        print "Parsing failed"
        return "Error"

def listen_UDP():

    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM) # IPv4, UDP
    sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    sock.bind((LISTENING_IP, LISTENING_PORT))

    while True:
        data, address = sock.recvfrom(BUFFER_SIZE)
        print ("UDP Messsage from address: ", address)
        print ("Message: ", data)

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
