#!/usr/bin/env python3
import os
import socket
from argparse import ArgumentParser
from datetime import datetime

MAX_BYTES = 65535

def client(path):
    """ Run "a Unix domain socket client """
    sock = socket.socket(socket.AF_UNIX, socket.SOCK_DGRAM)
    mypath = "/tmp/python_unix_socket_client.{}".format(os.getpid())
    sock.bind(mypath)
    text = 'The time is {}'.format(datetime.now())
    data = text.encode('ascii')
    sock.sendto(data, path)
    print('The OS assigned me the address {}'.format(sock.getsockname()))
    data, address = sock.recvfrom(MAX_BYTES)  # Danger! See Chapter 2
    text = data.decode('ascii')
    print('The server {} replied {!r}'.format(address, text))
    sock.close()
    os.unlink(mypath)

if __name__ == '__main__':
    parser = ArgumentParser(description='Send and receive UNIX domain datagram locally')
    parser.add_argument('--path', type=str, default="/tmp/python_unix_socket_server", help='unix-domain address the client sends to (default: %(default)s)')
    args = parser.parse_args()

    print("args: {}".format(args))

    client(args.path)
