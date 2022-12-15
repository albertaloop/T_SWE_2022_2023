#! python3

import numpy as np
import matplotlib.pyplot as plt
import pandas as pd 
import re 

# Set up pattern recognition
pointXPatt = r'Point x = (\d{7}.\d{8})'
pointYPatt = r'Point y = (\d{7}.\d{8})'
satPatt = r'Satellites: (\d?)'
distPatt = r'Distance = (\d+.\d{8})'

pointXRegex = re.compile(pointXPatt)
pointYRegex = re.compile(pointYPatt)
satRegex = re.compile(satPatt) 
distRegex = re.compile(distPatt)

# Read the file 
test3 = open('Firmware/GPS_Module_Teensy3.6/testresults3.txt', "r")   # open file from T_SWE_2022_2023 Directory
test4 = open('Firmware/GPS_Module_Teensy3.6/testresults4.txt', "r")

strTest3 = test3.read()
strTest4 = test4.read()

# print(strTest3) # Sanity Check

# Collect pattern strings 
# Test 3 results
strPointX_t3 = pointXRegex.findall(strTest3) #confirm if concatenating strings work
strPointY_t3 = pointYRegex.findall(strTest3)
strSatellites_t3 = satRegex.findall(strTest3)
strDist_t3 = distRegex.findall(strTest3)

# Test 4 results
strPointX_t4 = pointXRegex.findall(strTest4) #confirm if concatenating strings work
strPointY_t4 = pointYRegex.findall(strTest4)
strSatellites_t4 = satRegex.findall(strTest4)
strDist_t4 = distRegex.findall(strTest4)

# print(strDist_t4) # Sanity Check 
# print(strPointX_t3)

# Converting data from string to float
X_t3 = [float(i) for i in strPointX_t3]
Y_t3 = [float(i) for i in strPointY_t3]
# sat_t3 = [int(i) for i in strSatellites_t3]
dist_t3 = [float(i) for i in strDist_t3]

X_t4 = [float(i) for i in strPointX_t4]
Y_t4 = [float(i) for i in strPointY_t4]
sat_t4 = [int(i) for i in strSatellites_t4]
dist_t4 = [float(i) for i in strDist_t4]

# print(pointX_t3) # Sanity Check

# Plot the data
# Figure out mplib rq
# Plotting x v. y 
fig, ax = plt.subplots()
ax.plot(X_t3, Y_t3)
fig.show()


# Old Crap 🤢
# while line is not None:
#     line = f.readline() # read line from file

#     print(line)

test3.close()
test4.close()
