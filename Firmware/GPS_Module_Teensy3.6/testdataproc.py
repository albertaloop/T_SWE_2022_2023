#! python3

import numpy as np
import matplotlib.pyplot as plt
import pandas as pd 
import re

# Set up pattern recognition
pointXPatt = r'Point x = (\d{7}.\d{8})'
pointYPatt = r'Point y = (\d{7}.\d{8})'
originXPatt = r'Origin x = (\d{7}.\d{8})'
originYPatt = r'Origin y = (\d{7}.\d{8})'
satPatt = r'Satellites: (\d?)'
distPatt = r'Distance = (\d+.\d{8})'

pointXRegex = re.compile(pointXPatt)
pointYRegex = re.compile(pointYPatt)
originXRegex = re.compile(originXPatt)
originYRegex = re.compile(originYPatt)
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
strOriginX_t3 = originXRegex.findall(strTest3)
strOriginY_t3 = originYRegex.findall(strTest3)
strSatellites_t3 = satRegex.findall(strTest3)
strDist_t3 = distRegex.findall(strTest3)

# Test 4 results
strPointX_t4 = pointXRegex.findall(strTest4) #confirm if concatenating strings work
strPointY_t4 = pointYRegex.findall(strTest4)
strOriginX_t4 = originXRegex.findall(strTest4)
strOriginY_t4 = originYRegex.findall(strTest4)
strSatellites_t4 = satRegex.findall(strTest4)
strDist_t4 = distRegex.findall(strTest4)

# print(strDist_t4) # Sanity Check 

# Converting data from string to float
X_t3 = [float(i) for i in strPointX_t3]
Y_t3 = [float(i) for i in strPointY_t3]
Xo_t3 = [float(i) for i in strOriginX_t3]
Yo_t3 = [float(i) for i in strOriginY_t3]
# sat_t3 = [int(i) for i in strSatellites_t3] # There are no values in this
dist_t3 = [float(i) for i in strDist_t3]

X_t4 = [float(i) for i in strPointX_t4]
Y_t4 = [float(i) for i in strPointY_t4]
Xo_t4 = [float(i) for i in strOriginX_t4]
Yo_t4 = [float(i) for i in strOriginY_t4]
sat_t4 = [int(i) for i in strSatellites_t4]
dist_t4 = [float(i) for i in strDist_t4]

# print(pointX_t3) # Sanity Check

# Processing Position Data
# Test 3 Positon Data
X_t3_arr = np.array(X_t3)
Xo_t3_arr = np.array(Xo_t3)
Y_t3_arr = np.array(Y_t3)
Yo_t3_arr = np.array(Yo_t3)

posX_t3_arr = np.subtract(X_t3_arr, Xo_t3_arr)
posY_t3_arr = np.subtract(Y_t3_arr, Yo_t3_arr)
posX_t3 = list(posX_t3_arr)
posY_t3 = list(posY_t3_arr)

# Test 4 Position Data 
X_t4_arr = np.array(X_t4)
Xo_t4_arr = np.array(Xo_t4)
Y_t4_arr = np.array(Y_t4)
Yo_t4_arr = np.array(Yo_t4)

posX_t4_arr = np.subtract(X_t4_arr, Xo_t4_arr)
posY_t4_arr = np.subtract(Y_t4_arr, Yo_t4_arr)
posX_t4 = list(posX_t4_arr)
posY_t4 = list(posY_t4_arr)

# Plotting test data against each other
fig, (ax1, ax2) = plt.subplots(2,1)
ax1.plot(posX_t3, posY_t3, 'r', posX_t4, posY_t4, 'b')
ax2.plot(dist_t3, 'r', dist_t4, 'b')
plt.show()

# Closing files
test3.close()
test4.close()
