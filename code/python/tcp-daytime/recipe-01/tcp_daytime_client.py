#!/usr/bin/env python3
# Python Network Programming Cookbook -- Chapter - 1
# This program is optimized for Python 2.7.
# It may run on any other version with/without modifications.

import socket
import sys

import argparse

def tcp_daytime_client(host, port):
    """ A simple echo client """
    # Create a TCP/IP socket
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    # Connect the socket to the server
    server_address = (host, port)
    print("Connecting to %s port %s" % server_address)
    sock.connect(server_address)
    
    # Send data
    try:
        # Look for the response
        while True:
            data = sock.recv(128)
            if not data:
                break;
            print(data.decode('ascii'), end='')
    except socket.error as e:
        print("Socket error: %s" %str(e))
    except Exception as e:
        print("Other exception: %s" %str(e))
    finally:
        print("Closing connection to the server")
        sock.close()
    
if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='Socket Server Example')
    parser.add_argument('--host', action="store", dest="host", type=str, default="localhost")
    parser.add_argument('--port', action="store", dest="port", type=int, required=True)
    given_args = parser.parse_args() 
    host = given_args.host
    port = given_args.port
    tcp_daytime_client(host, port)
