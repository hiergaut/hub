
import time
import hubPy

# from hubPy import *
from time import sleep


outputSensor = hubPy.OutputSensor('pySensor', 'pyStream')

for i in range(0, 10):
    print(i)
    outputSensor.start = i
    outputSensor.end = i
    outputSensor.x = i
    outputSensor.y = i
    outputSensor.z = i
    outputSensor.w0 = i
    outputSensor.w1 = i
    outputSensor.w2 = i
    outputSensor.w3 = i
    outputSensor.newAcq()
    sleep(0.1)
