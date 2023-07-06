#!/usr/bin/env python3

import time
import netlink
import datetime

class MyUeventObserver(netlink.UeventObserver):
    def __init__(self):
        netlink.UeventObserver.__init__(self)
        self.count = 1

    def onReceiveUevent(self, data):
        if len(data) < 32:
            print("invalid message")
            return

        self.count += 1
        print("***********************msg {} start***********************".format(self.count))
        print("received {} bytes\n{}".format(len(data), data))
        print("***********************msg {} ends************************".format(self.count))

def MonitorNetlinkUevent():
    monitor = netlink.NetlinkUeventMonitor(0)
    observer = MyUeventObserver()
    monitor.addObserver(observer)

    monitor.start()
    while True:
        print("uevent_monitor is running..., {}".format(datetime.datetime.now()))
        time.sleep(1)

def main():
    print("***********************start***********************")
    MonitorNetlinkUevent()
    print("***********************ends************************")

if __name__ == "__main__":
    main()
