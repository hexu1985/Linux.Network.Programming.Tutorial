
import time
import logging
import netlink
import datetime
import functools
import psutil
import sched

from worker_thread import WorkerThread

LOGGER = logging.getLogger()

UEVENT_RECVTIME = 'receive_time'
UEVENT_ACTION = 'ACTION'
UEVENT_DEVTYPE = 'DEVTYPE'
UEVENT_DEVNAME = 'DEVNAME'

class UeventMonitor(netlink.UeventObserver):
    def __init__(self):
        netlink.UeventObserver.__init__(self)
        self.monitor = netlink.NetlinkUeventMonitor(0)
        self.worker_thread = WorkerThread()
        self.pending_event_list = list()
        self.dev_mountpoint_map = dict()
        self.scheduler = sched.scheduler(time.time, time.sleep)
        self.pending_check_interval_sec = 5
        self.pending_expired_time_sec = 30

    def parserRawData(self, data):
        lines = data.split('\n')
        event = dict()
        for line in lines:
            if line.count('=') == 1:
                key, value = line.split('=')
                event[key] = value
        
        dt = datetime.datetime.now()
        event[UEVENT_RECVTIME] = dt
        
        return event

    # override netlink.UeventObserver.onReceiveUevent
    def onReceiveUevent(self, data):
        if len(data) < 32:
            print("invalid message")
            return

        event = self.parserRawData(data)
        task = functools.partial(self.doProcessEvent, event=event)
        self.worker_thread.putTask(task)

    def run(self):
        self.updateMountpoint()

        self.worker_thread.start()

        self.monitor.addObserver(self)
        self.monitor.start()

        self.scheduler.enter(self.pending_check_interval_sec, 0, self.checkPendingEvent)
        self.scheduler.run()

    def checkPendingEvent(self):
        task = functools.partial(self.doCheckPendingEvent)
        self.worker_thread.putTask(task)
        self.scheduler.enter(self.pending_check_interval_sec, 0, self.checkPendingEvent)

    def doCheckPendingEvent(self):
        pending_event_list = self.pending_event_list
        self.pending_event_list = list()
        now = datetime.datetime.now()
        expired_time = datetime.timedelta(seconds=self.pending_expired_time_sec)
        for event in pending_event_list:
            if (now - event[UEVENT_RECVTIME]) > expired_time:
                LOGGER.info("event is expired: {}".format(event))
            else:
                self.onPartitionAdd(event)

    def updateMountpoint(self):
        dev_mountpoint_map = dict()
        for partition in psutil.disk_partitions():
            dev_mountpoint_map[partition.device] = partition.mountpoint
        self.dev_mountpoint_map = dev_mountpoint_map
        LOGGER.debug("dev_mountpoint_map: {}".format(dev_mountpoint_map))

    def doProcessEvent(self, event):
        has_devtype, devtype = self.getEventField(event, UEVENT_DEVTYPE)
        if not has_devtype or devtype != 'partition':
            LOGGER.debug("not partition")
            return

        has_action, action = self.getEventField(event, UEVENT_ACTION)
        if not has_action:
            LOGGER.debug("no action fields")
            return

        has_devname, devname = self.getEventField(event, UEVENT_DEVNAME)
        if not has_devname: 
            LOGGER.debug("no devname fields")
            return

        if not devname.startswith('/dev'):
            LOGGER.debug("devname not startswith /dev: {}".format(devname))
            return

        if action == 'add':
            self.onPartitionAdd(event)
        elif action == 'remove':
            self.onPartitionRemove(event)

        LOGGER.debug("date: {}, event: {}".format(datetime.datetime.now(), event))

    def onPartitionAdd(self, event):
        self.updateMountpoint()

        devname = event[UEVENT_DEVNAME]
        if devname not in self.dev_mountpoint_map:
            LOGGER.info("add event to pending_event_list")
            self.pending_event_list.append(event)
            return
        
        mountpoint = self.dev_mountpoint_map[devname]

        # TODO report partition add
        LOGGER.info("partition {} add on {} at {}".format(devname, mountpoint, event[UEVENT_RECVTIME]))

    def onPartitionRemove(self, event):
        devname = event[UEVENT_DEVNAME]
        if devname not in self.dev_mountpoint_map:
            LOGGER.error("not found mountpoint by devname: {}".format(devname))
            return 

        mountpoint = self.dev_mountpoint_map[devname]

        # TODO report partition remove
        LOGGER.info("partition {} remove on {} at {}".format(devname, mountpoint, event[UEVENT_RECVTIME]))

    def getEventField(self, event, key):
        if key in event:
            return True, event[key]
        else:
            return False, None

if __name__ == "__main__":
    logging.basicConfig(level=logging.INFO, format=("[%(levelname)s][%(asctime)s][%(threadName)s:%(thread)d]"
                     "[%(filename)s:%(lineno)d] %(message)s"))
    monitor = UeventMonitor()
    monitor.run()
