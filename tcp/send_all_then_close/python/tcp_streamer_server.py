#!/usr/bin/env python3

import socket
from argparse import ArgumentParser

def server(host, port):
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    sock.bind((host, port))
    sock.listen(1)
    print('Listening at', sock.getsockname())
    sc, sockname = sock.accept()
    print('Accepted connection from', sockname)
    sc.shutdown(socket.SHUT_WR)
    message = b''
    while True:
        more = sc.recv(8192)  # arbitrary value of 8k
        if not more:  # socket has closed when recv() returns ''
            print('Received zero bytes - end of file')
            break
        print('Received {} bytes'.format(len(more)))
        message += more
    print('Message:\n')
    print(message.decode('ascii'))
    sc.close()
    sock.close()

if __name__ == '__main__':
    parser = ArgumentParser(description='Transmit & receive a data stream')
    parser.add_argument('--host', type=str, default='0.0.0.0', help='IP address the server listens at (default: %(default)s)')
    parser.add_argument('--port', '-p', type=int, default=1060, help='TCP port number (default: %(default)s)')
    args = parser.parse_args()

    print("args: {}".format(args))

    server(args.host, args.port)
