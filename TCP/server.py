# source code https://stackoverflow.com/questions/29022329/basic-udp-tcp-program

from threading import Thread
import socket 

LISTENING_IP = socket.gethostbyname(socket.gethostname())
LISTENING_PORT = 300
BUFFER_SIZE = 1024

def parse_data(data):
    split = data.split('>')


def listen_UDP():

    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM) # IPv4, UDP
    sock.bind((LISTENING_IP, LISTENING_PORT))

    while True:
        data, address = sock.recvfrom(BUFFER_SIZE)
        print ("UDP Messsage from address: ", address)
        print ("Message: ", data)

def listen_TCP():

    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM) # IPv4, TCP
    sock.bind((LISTENING_IP, LISTENING_PORT))

    while True:
        sock.listen(1)
        conn, address = sock.accept()
        print ("TCP connection from", address)
        data = conn.recv(BUFFER_SIZE)
        print ("Mesage: ", data)
        conn.close()

if __name__ == "__main__":
    Thread_UDP = Thread(target=listen_UDP)
    Thread_TCP = Thread(target=listen_TCP)

    print ("Starting Server...")
    Thread_UDP.start()
    Thread_TCP.start()
    print ("Server Started!")