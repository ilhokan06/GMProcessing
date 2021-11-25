# -*- coding: utf-8 -*-
"""
This code is developed to convert the format of strong ground motion
data from TADAS (https://tadas.afad.gov.tr/) into PEER ".AT2" format.
The folder path including the ground motion (GM) file and the name
of the GM file (in lines 18-19) needs to be provided as inputs to
the following code.

Okan Ilhan
Ankara Yildirim Beyazit University
Department of Civil Engineering

First V.: 24th Nov, 2021
"""

import os

inputloc = "E:\\TOSHIBA-BACK-UP\\AYBU-COURSES\\GEOTECHNICAL_EARTHQUAKE_ENGINEERING\\ASSIGNMENTS\\ASSIGNMENT_4"
filename = "234632_20140719154833_3525_ap_Acc_E.asc"

########### Reading Part ###########
os.chdir(inputloc)

f = open(filename, "r")
Data = f.read()
Data_sp = Data.split("\n")

########### Writing Part ###########
Data_w = ""

## Preparation of header ##
Data_w = Data_w+"DEEPSOIL BC STRONG MOTION DATA CONVERTED TO AT2 FORMAT"+"\n"

E_ID = Data_sp[1].split("EVENT_ID: ")[-1]
S_ID = Data_sp[14].split("STATION_CODE: ")[-1]
E_a_ST_ID = "E_ID: "+E_ID+" "+"STATION_ID: "+S_ID+"\n"

Data_w = Data_w+E_a_ST_ID
Data_w = Data_w+"ACCELERATION TIME HISTORY IN UNITS OF G"+"\n"

DT = Data_sp[28].split("SAMPLING_INTERVAL_S: ")[-1]
NPTS = Data_sp[29].split("NDATA: ")[-1]
DT_NPTS = NPTS+"    "+DT+"    "+"NPTS, DT\n"
Data_w = Data_w+DT_NPTS

## Preparation of Acceleration Data ##
counter = 0
Acc_Data_Temp = []

for ind_D, i_D in enumerate(Data_sp):
    if ind_D < 64:
        pass
    elif (ind_D >= 64) and (counter < 5) and (i_D != ""):
        Acc_Data_Temp.append(float(i_D)/980.665)
        counter = counter + 1
    elif (ind_D >= 64) and (counter == 5) and (i_D != ""):
        Acc_Data_Temp_w = "%16.6E%16.6E%16.6E%16.6E%16.6E\n" % (Acc_Data_Temp[0], Acc_Data_Temp[1], Acc_Data_Temp[2],
                                                             Acc_Data_Temp[3], Acc_Data_Temp[4])
        Data_w = Data_w+Acc_Data_Temp_w
        
        counter = 0
        Acc_Data_Temp = []
        counter = counter + 1
        Acc_Data_Temp.append(float(i_D)/980.665)
        
if len(Acc_Data_Temp) != 0:
    Acc_Data_Temp_w_st = ""
    Acc_Data_Temp_w_tup = tuple(Acc_Data_Temp)
    for _ in range(len(Acc_Data_Temp)): Acc_Data_Temp_w_st = Acc_Data_Temp_w_st+"%16.6E"
    Acc_Data_Temp_w = Acc_Data_Temp_w_st % Acc_Data_Temp_w_tup
    Data_w = Data_w + Acc_Data_Temp_w
        
filename_w = filename.split(".asc")[0]
filename_w = filename_w + ".AT2"

f_w = open(filename_w, "w")
f_w.write(Data_w)

        
    