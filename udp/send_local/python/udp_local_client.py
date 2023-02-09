#!/usr/bin/env python3
import socket
from argparse import ArgumentParser
from datetime import datetime

MAX_BYTES = 65535

def client(port):
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    text = 'The time is {}'.format(datetime.now())
    data = text.encode('ascii')
    sock.sendto(data, ('127.0.0.1', port))
    print('The OS assigned me the address {}'.format(sock.getsockname()))
    data, address = sock.recvfrom(MAX_BYTES)  # Danger! See Chapter 2
    text = data.decode('ascii')
    print('The server {} replied {!r}'.format(address, text))

if __name__ == '__main__':
    parser = ArgumentParser(description='Send and receive UDP locally')
    parser.add_argument('--port', '-p', type=int, default=1060, help='UDP port (default 1060)')
    args = parser.parse_args()

    print("args: {}".format(args))

    client(args.port)
