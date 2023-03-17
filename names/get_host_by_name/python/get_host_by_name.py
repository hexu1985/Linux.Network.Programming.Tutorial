#!/usr/bin/env python
# Python Network Programming Cookbook -- Chapter - 1
# This program is optimized for Python 2.7.
# It may run on any other version with/without modifications.

import sys
import socket

def get_host_by_name(remote_host):
    try:
        print("IP address of {}: {}".format(remote_host, socket.gethostbyname(remote_host)))
    except socket.error as err_msg:
        print("{}: {}".format(remote_host, err_msg))
    
if __name__ == '__main__':
    remote_host = 'www.python.org'
    if len(sys.argv) == 2:
        remote_host = sys.argv[1]
    get_host_by_name(remote_host)
