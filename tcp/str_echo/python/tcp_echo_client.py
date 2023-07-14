#!/usr/bin/env python3

import argparse, socket
import sys

def str_cli(fp, sock):
    sock_fp = sock.makefile()
    while True:
        sendline = fp.readline()
        if not sendline:
            return

        sock.sendall(sendline.encode())

        recvline = sock_fp.readline()
        if not recvline:
            print("str_cli: server terminated prematurely")
            return

        print(recvline, end="")

def client(host, port):
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.connect((host, port))
    print('Client has been assigned socket name', sock.getsockname())

    str_cli(sys.stdin, sock)

    sock.close()

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='Send and receive over TCP')
    parser.add_argument('--host', type=str, default='127.0.0.1', help='IP address the client sends to (default: %(default)s)')
    parser.add_argument('--port', '-p', type=int, default=1060, help='TCP port number (default: %(default)s)')
    args = parser.parse_args()

    print("args: {}".format(args))

    client(args.host, args.port)
