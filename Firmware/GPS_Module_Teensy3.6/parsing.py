import pandas as pd 
import numpy as np
import re 


f = open('T_SWE_2022_2023/Firmware/GPS_Module_Teensy3.6/testresults3.txt', "r")   # open file from T_SWE_2022_2023 Directory

line = f.readline() # read line from file

print(line)

f.close()
