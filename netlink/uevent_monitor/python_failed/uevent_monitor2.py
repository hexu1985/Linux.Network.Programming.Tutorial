#!/usr/bin/env python3

import time
import signal
import threading
import netlink
import datetime

def receive_signal(signum, stack):
    print('Received:', signum)

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
    signal.signal(signal.SIGINT, receive_signal)
    t = threading.Thread(target=MonitorNetlinkUevent, daemon=True)
    t.start()
    while True:
        print("uevent_monitor is running..., {}".format(datetime.datetime.now()))
        time.sleep(1)
    print("***********************ends************************")

if __name__ == "__main__":
    main()
