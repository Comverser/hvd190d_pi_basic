#!/usr/bin/env python3

import RPi.GPIO as gpio
import sys

gpio.setmode(gpio.BCM)
gpio.setup(16, gpio.OUT)

gpio.output(16, gpio.LOW)
print(sys.version)
