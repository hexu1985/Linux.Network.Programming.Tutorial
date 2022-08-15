#!/usr/bin/env python3
# Python Network Programming Cookbook -- Chapter - 1
# This program is optimized for Python 2.7.
# It may run on any other version with/without modifications.

import socket
import sys

import argparse

def echo_client(host, port):
    """ A simple echo client """
    # Create a TCP/IP socket
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    # Connect the socket to the server
    server_address = (host, port)
    print("Connecting to %s port %s" % server_address)
    sock.connect(server_address)
    
    while True:
        message = sys.stdin.readline()
        if not message:
            break
        buf = message.encode('ascii')
        sock.sendall(buf)
        n_expected = len(buf)
        n_recv = 0
        buf = b''
        while n_recv < n_expected:
            data = sock.recv(n_expected-n_recv)
            buf += data
            n_recv += len(data)
        message = buf.decode('ascii')
        print(message, end='')

    sock.close()
    
if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='Socket Server Example')
    parser.add_argument('--host', action="store", dest="host", type=str, default="localhost")
    parser.add_argument('--port', action="store", dest="port", type=int, required=True)
    given_args = parser.parse_args() 
    host = given_args.host
    port = given_args.port
    echo_client(host, port)
