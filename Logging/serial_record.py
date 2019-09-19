#!/usr/bin/env python

# Script to record all measurements from the Iron

import serial
import os
import numpy as np
#import matplotlib.pyplot as plt
from datetime import datetime

import Gnuplot

ser = serial.Serial('/dev/ttyUSB1', 115200, timeout=0.2)

logfile = open("templog_{}.csv".format(datetime.now().strftime("%Y%m%d_%H%M%S")),"a+")
last_temp = -1

entry_found = False

ki = 0
kp = 0
kd = 0

print("Waiting for start")

while entry_found == False:
    try:
        line = ser.read(13)
        #print(line)
        if line[0] != 0xAA: ## If the frame does not start with 0xAA -> discard
          continue

        entry_found = True
        kp = int.from_bytes(line[1:5],  byteorder='big') / 1000
        ki = int.from_bytes(line[5:9],  byteorder='big') / 1000
        kd = int.from_bytes(line[9:13], byteorder='big') / 1000
        print("Kp: {:.3f} Ki: {:.3f} Kd: {:.3f}".format(kp, ki, kd))
        logfile.write("# Kp: {:.3f} Ki: {:.3f} Kd: {:.3f}".format(kp, ki, kd))

    except KeyboardInterrupt:
        break
    except:
        #print("Invalid frame received, skipping " + str(line))
        continue

print("Start found")

while True:
    try:
        line = ser.read(8)
        #print(line)
        if line[0] != 0xAB: ## If the frame does not start with 0xAB -> discard
          continue
        ctime = int.from_bytes(line[1:3], byteorder='big')
        ttemp = int.from_bytes(line[3:5], byteorder='big')
        ctemp = int.from_bytes(line[5:7], byteorder='big')
        cpwr  = line[7]
        ctime = ctime / 10
        ctemp = ctemp / 100
        ttemp = ttemp / 10
        if last_temp == -1:
            last_temp = ctemp
        tdelta = (ctemp - last_temp) * 50
        #logfile.write("%.1f;%.1f;%.2f;%.2f;%d\n" % (ctime, ttemp, ctemp, tdelta, cpwr))
        logfile.write("%.1f;%.1f;%.2f;%.2f;%d;%.2f\n" % (ctime, ttemp, ctemp, tdelta, cpwr, ttemp - ctemp))
        print("{:8.1f}  -  Target {:5.1f} °C  -  Ist {:6.2f} °C  -  Abweichung {:7.2f} °C  -  Delta {:5.2f} d°C/s  -  P {:3d} %".format(ctime, ttemp, ctemp, ttemp - ctemp, ctemp - last_temp, cpwr))
        last_temp = ctemp
    except KeyboardInterrupt:
        break
    except:
        #print("Invalid frame received, skipping " + str(line))
        continue

logfile.close()

gp = Gnuplot.Gnuplot()

gp.load("pid.gp")
gp.set_string("out", os.path.splitext(logfile.name)[0] + ".pdf")
gp.set_string("title", "Temperature Curve Measurement "   +
              datetime.now().strftime("%H:%M - %d.%m.%Y") +
              " k_p: {:.3f} k_i: {:.3f} k_d: {:.3f}".format(kp, ki, kd))
gp.plot("\'{}\' using 1:2 title \'Target Temperature [°C]\' with lines,\
         \'{}\' using 1:3 title \'Iron Temperature [°C]\' with lines,\
         \'{}\' using 1:4 title \'DTemperature [°C/10]\' with lines,\
         \'{}\' using 1:6 title \'Error [°C]\' with lines,\
         \'{}\' using 1:5 title \'Heating [%]\' with lines,\
         ".format(logfile.name, logfile.name, logfile.name, logfile.name, logfile.name))

