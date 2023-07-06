#!/usr/bin/env python3

import time
import socket
import argparse

backlog = 5 

def server(host, port):
    """ A simple echo server """
    # Create a TCP socket
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    # Enable reuse address/port 
    sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    # Bind the socket to the port
    print("Starting up echo server on {} port {}".format(host, port))
    sock.bind((host, port))
    # Listen to clients, backlog argument specifies the max no. of queued connections
    sock.listen(backlog) 
    while True: 
        client, address = sock.accept() 

        message = time.ctime()+"\r\n"
        client.sendall(message.encode('ascii'))

        # end connection
        client.close() 
   
if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='Socket Server Example')
    parser.add_argument('--host', type=str, default='127.0.0.1', help='IP address the client sends to (default: %(default)s)')
    parser.add_argument('--port', '-p', type=int, default=1060, help='TCP port number (default: %(default)s)')
    args = parser.parse_args()
    print("args: {}".format(args))

    server(args.host, args.port)
