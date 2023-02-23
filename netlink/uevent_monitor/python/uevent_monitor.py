#!/usr/bin/env python3

import netlink

def MonitorNetlinkUevent():
    monitor = netlink.NetlinkUeventMonitor(0)
    count = 0
    while True:
        data = monitor.recv(4096)
        if len(data) < 32:
            print("invalid message")
            continue

        count += 1
        print("***********************msg {} start***********************".format(count))
        print("received {} bytes\n{}".format(len(data), data))
        print("***********************msg {} ends************************".format(count))


def main():
    print("***********************start***********************")
    MonitorNetlinkUevent()
    print("***********************ends************************")

if __name__ == "__main__":
    main()
