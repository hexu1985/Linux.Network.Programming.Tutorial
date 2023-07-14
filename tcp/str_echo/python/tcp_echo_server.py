#!/usr/bin/env python3

import argparse, socket

def str_echo(sock):
    while True:
        data = sock.recv(1024)
        if not data:
            return

        sock.sendall(data)

def server(host, port):
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    sock.bind((host, port))
    sock.listen(1)
    print('Listening at', sock.getsockname())
    while True:
        print('Waiting to accept a new connection')
        sc, sockname = sock.accept()
        print('We have accepted a connection from', sockname)
        print('  Socket name:', sc.getsockname())
        print('  Socket peer:', sc.getpeername())

        str_echo(sc)
        sc.close()

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='Send and receive over TCP')
    parser.add_argument('--host', type=str, default='0.0.0.0', help='IP address the server listens at (default: %(default)s)')
    parser.add_argument('--port', '-p', type=int, default=1060, help='TCP port number (default: %(default)s)')
    args = parser.parse_args()

    print("args: {}".format(args))

    server(args.host, args.port)
