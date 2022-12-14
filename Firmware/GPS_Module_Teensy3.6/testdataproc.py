#! python3

import matplotlib as mplib
import numpy as np
import pandas as pd 
import re 

# Set up pattern recognition
pointXPatt = r'Point x = (\d{7}.\d{8})'
pointYPatt = r'Point y = (\d{8}.\d{8})'
satPatt = r'Satellites: (\d?)'
distPatt = r'Distance = (\d+.\d{8})'

pointXRegex = re.compile(pointXPatt)
pointYRegex = re.compile(pointYPatt)
satRegex = re.compile(satPatt) 
distRegex = re.compile(distPatt)

# Read the file 
test3 = open('T_SWE_2022_2023/Firmware/GPS_Module_Teensy3.6/testresults3.txt', "r")   # open file from T_SWE_2022_2023 Directory
test4 = open('T_SWE_2022_2023/Firmware/GPS_Module_Teensy3.6/testresults4.txt', "r")

strTest3 = test3.read()
strTest4 = test4.read()

# print(strTest3) #Sanity Check

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

# print(strDist_t4) #Sanity Check 

# Collect data from pattern strings
    # Collecting point data

    # Collecting satellite data

    # Collecting distance data

# Converting the data
pointX_t3 = int(strPointX_t3)
pointY_t3 = int(strPointX_t3)
sat_t3 = int(strSatellites_t3)
dist_t3 = int(strDist_t3)

pointX_t4 = int(strPointX_t4)
pointY_t4 = int(strPointX_t4)
sat_t4 = int(strSatellites_t4)
dist_t4 = int(strDist_t4)

# print(pointX_t3)

# Plot the data


# Old Crap 🤢
# while line is not None:
#     line = f.readline() # read line from file

#     print(line)

test3.close()
test4.close()
