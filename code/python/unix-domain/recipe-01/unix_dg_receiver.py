#!/usr/bin/env python3
# This program is optimized for Python 2.7.
# It may run on any other version with/without modifications.

import socket
import os
import sys

SERVER_PATH = "/tmp/python_unix_socket_server"
 
def run_unix_domain_socket_server():
    if os.path.exists(SERVER_PATH):
        os.remove( SERVER_PATH )
     
    print("starting unix domain socket server.")
    server = socket.socket( socket.AF_UNIX, socket.SOCK_DGRAM )
    server.bind(SERVER_PATH)
     
    print("Listening on path: %s" %SERVER_PATH)
    while True:
        datagram = server.recv( 1024 )
        if not datagram:
            break
        else:
            print("-" * 20)
            print("Receive message: %s\n"%datagram.decode('ascii'))
        if "DONE" == datagram:
            break
    print("-" * 20)
    print("Server is shutting down now...")
    server.close()
    os.remove(SERVER_PATH)
    print("Server shutdown and path removed.")

if __name__ == '__main__':
    if len(sys.argv) == 2:
        SERVER_PATH = sys.argv[1]
    run_unix_domain_socket_server()