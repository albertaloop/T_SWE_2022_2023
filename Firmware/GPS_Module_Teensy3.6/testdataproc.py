

import pandas as pd 
import numpy as np
import re 

# Set up pattern recognition
pointXPatt = r'Point x = %d%d%d%d%d%d%d.%d%d%d%d%d%d%d%d'
pointYPatt = r'Point y = %d%d%d%d%d%d%d.%d%d%d%d%d%d%d%d'
satPatt = r'Satellites: %d'
distPatt = r'Distance = %d.'

pointXRegex = re.compile()
pointYRegex = re.compile()
satRegex = re.compile() 
distRegex = re.compile()

# Read the file 
test3 = open('T_SWE_2022_2023/Firmware/GPS_Module_Teensy3.6/testresults3.txt', "r")   # open file from T_SWE_2022_2023 Directory
test4 = open('T_SWE_2022_2023/Firmware/GPS_Module_Teensy3.6/testresults4.txt', "r")

strTest3 = test3.read()
strTest4 = test4.read()

# Collect pattern strings 
strPointX = pointXRegex.findall(strTest3 + strTest4) #confirm if concatenating strings work
strPointY = pointYRegex.findall(strTest3 + strTest4)
strSatellites = satRegex.findall(strTest3 + strTest4)
strDist = distRegex.findall(strTest3 + strTest4)

# Collect data from pattern strings
    # Collecting point data

    # Collecting satellite data

    # Collecting distance data

# Converting the data

# Plot the data


# Old Crap 🤢
# while line is not None:
#     line = f.readline() # read line from file

#     print(line)

test3.close()
test4.close()
