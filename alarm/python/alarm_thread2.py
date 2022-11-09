#!/usr/bin/env python3

from threading import Timer
from datetime import datetime

class Alarm:
    def __init__(self, seconds:int, message:str):
        self.seconds = seconds
        self.message = message

def callback(alarm:Alarm):
    print("alarm timer at {}".format(datetime.now().strftime("%Y-%m-%d %H:%M:%S")))
    print("({}) {}\n".format(alarm.seconds, alarm.message))

if __name__ == "__main__":
    while True:
        line = input("Alarm> ")
        if len(line) <= 1:
            continue

        try:
            seconds, *message = line.split(' ')
            alarm = Alarm(int(seconds), ' '.join(message))
            print("start timer at {}".format(datetime.now().strftime("%Y-%m-%d %H:%M:%S")))
            t = Timer(interval=int(seconds), function=callback, args=(alarm, ))
            t.start()
        except:
            print("Bad command")

