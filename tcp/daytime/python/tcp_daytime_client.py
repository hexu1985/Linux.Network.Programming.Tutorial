#!/usr/bin/env python3

import socket
import argparse

def client(host, port):
    """ A simple echo client """
    # Create a TCP/IP socket
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    # Connect the socket to the server
    sock.connect((host, port))
    
    # Send data
    try:
        # Look for the response
        while True:
            data = sock.recv(128)
            if not data:
                break;
            print(data.decode('ascii'), end='')
    except socket.error as e:
        print("Socket error: {}".format(e))
    except Exception as e:
        print("Other exception: {}".format(e))
    finally:
        sock.close()
    
if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='Socket Client Example')
    parser.add_argument('--host', type=str, default='127.0.0.1', help='IP address the client sends to (default: %(default)s)')
    parser.add_argument('--port', '-p', type=int, default=1060, help='TCP port number (default: %(default)s)')
    args = parser.parse_args()

    client(args.host, args.port)
