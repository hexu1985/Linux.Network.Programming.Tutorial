#!/usr/bin/env python3

import os
import socket
from argparse import ArgumentParser

MAX_BYTES = 65535

def server(path):
    print("starting unix domain socket server.")
    sock = socket.socket(socket.AF_UNIX, socket.SOCK_DGRAM)

    if os.path.exists(path):
        os.unlink(path)
    sock.bind(path)

    print('Listening at {}'.format(sock.getsockname()))
    while True:
        data, address = sock.recvfrom(MAX_BYTES)
        text = data.decode('ascii')
        print('The client at {} says {!r}'.format(address, text))
        text = 'Your data was {} bytes long'.format(len(data))
        data = text.encode('ascii')
        sock.sendto(data, address)

if __name__ == '__main__':
    parser = ArgumentParser(description='Send and receive UNIX domain datagram locally')
    parser.add_argument('--path', type=str, default="/tmp/unix_domain_socket_server", help='unix-domain address the server listens at (default: %(default)s)')
    args = parser.parse_args()

    print("args: {}".format(args))

    server(args.path)
