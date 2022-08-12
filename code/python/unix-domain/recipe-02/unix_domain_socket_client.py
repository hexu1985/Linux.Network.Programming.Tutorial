#!/usr/bin/env python3
# Python Network Programming Cookbook -- Chapter - 3
# This program is optimized for Python 2.7.
# It may run on any other version with/without modifications.

import socket
import sys
import os

SERVER_PATH = "/tmp/python_unix_socket_server"
CLIENT_PATH = "/tmp/python_unix_socket_client"

def run_unix_domain_socket_client():
    """ Run "a Unix domain socket client """
    sock = socket.socket(socket.AF_UNIX, socket.SOCK_DGRAM)

    # Bind the socket with the path where the client is listening
    client_address = CLIENT_PATH+"."+str(os.getpid())
    print("bind with %s" % client_address)
    try:
        sock.bind(client_address)
    except socket.error as msg:
        print(msg, file=sys.stderr)
        sys.exit(1)
    
    # Connect the socket to the path where the server is listening
    server_address = SERVER_PATH 
    print("connecting to %s" % server_address)
    try:
        sock.connect(server_address)
    except socket.error as msg:
        print(msg, file=sys.stderr)
        os.remove( client_address )
        sys.exit(1)
    
    try:
        message = "This is the message.  This will be echoed back!"
        print("Sending [%s]" %message)
        sock.sendall(message.encode('ascii'))
        amount_expected = len(message)
        
        data = sock.recv(amount_expected)
        print("Received [%s]" % data, file=sys.stderr)

        sock.sendall("DONE".encode('ascii'))
    
    finally:
        print("Closing client")
        sock.close()
        os.remove( client_address )

if __name__ == '__main__':
    run_unix_domain_socket_client()
