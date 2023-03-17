#!/usr/bin/env python3
# Foundations of Python Network Programming, Third Edition
# https://github.com/brandon-rhodes/fopnp/blob/m/py3/chapter04/www_ping.py
# Find the WWW service of an arbitrary host using getaddrinfo().

import argparse, socket, sys

def connect_to(hostname_or_ip, servname_or_port):
    try:
        infolist = socket.getaddrinfo(
            hostname_or_ip, servname_or_port, 0, socket.SOCK_STREAM, 0,
            socket.AI_ADDRCONFIG | socket.AI_V4MAPPED | socket.AI_CANONNAME,
            )
    except socket.gaierror as e:
        print('Name service failure:', e.args[1])
        sys.exit(1)

    info = infolist[0]  # per standard recommendation, try the first one
    socket_args = info[0:3]
    address = info[4]
    s = socket.socket(*socket_args)
    try:
        s.connect(address)
    except socket.error as e:
        print('Network failure:', e.args[1])
    else:
        print('Success: host', info[3], 'is listening on port', address[1])

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='Try connecting by tcp')
    parser.add_argument('--host', type=str, required=True, help='hostname that you want to contact')
    parser.add_argument('--serv', type=str, default='www', help='TCP service name or port number (default: %(default)s)')
    args = parser.parse_args()
    connect_to(args.host, args.serv)
