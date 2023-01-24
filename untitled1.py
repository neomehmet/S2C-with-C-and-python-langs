# -*- coding: utf-8 -*-
"""
Created on Tue Jan 24 15:37:35 2023

@author: mkose - gtu - supervised by fatma nur esirci
this code convert sequential to combination 
delete dff's and set the dff's input as output wire and dff's output as input wire 
to time analyze
m.kose2019@gtu.edu.tr
"""

class cables():
    inWire = list()
    outWire = list()

def SetLine( line ):
    line = line.replace( "(" , " ")  
    line = line.replace( "," , " ")  
    line = line.replace( ")" , " ")  
    line = line.replace( ";" , " ")  
    line = line.replace("\n", "") 
    return line

def Misalnum(line):
    res = ""
    for i in line:
        if (not i.isalnum() ) and i.isprintable():
            res += i
    return res

def inOutGet(lines, inout):
    index = 0
    while(index < len(lines)):
        indexStart = lines[index].find("CK ") +3
        temp = lines[index][indexStart:]
        indexStop = temp.find(" ") +1
        inout.outWire.append( temp[:indexStop ] )
        inout.inWire.append( temp[indexStop : ] )
        inout.outWire[index] = inout.outWire[index].replace(" ","")
        inout.inWire[index] = inout.inWire[index].replace(" ", "")
        index +=1
    return inout

sequential = open("verilog.v.txt")

lines = sequential.readlines()

inout = cables()        
dfflist = list()
index = 0
lines2 = list()

for i in lines:
    if i.find("dff") == 2 :
        dfflist.append(i)
    
for i in dfflist:
    lines2.append(SetLine(i)) 

inout = inOutGet(lines2, inout )

combinational = open("comb.txt","a")

intemp = ""
for i in inout.inWire:
    intemp  += i +","
intemp = intemp[:-1]
outtemp = ""
for i in inout.outWire:
    outtemp  += i + ","
outtemp = outtemp[:-1]

sequential.close()
a2 = open("verilog.v.txt")
lines = a2.readlines()

index = 0

while( index < len(lines) ):
    if  True and lines[index].count("module"):
        temp = lines[index];
        temp = temp.replace(")",",")
        temp = temp.replace(";","")
        temp += "\t\t" + intemp + "," + "\n\t\t"  + outtemp
        temp = temp + " ) ; \n"
        combinational.write(temp)
        print(temp)
        
    elif "dff" in lines[index]:
        print("buldm")
    
    elif "input" in lines[index]:
        temp = lines[index].replace(";" , ",")
        temp +=  "\t\t" + outtemp +" ; \n"
        combinational.write(temp)   
    
    elif lines[index].count("output"):
        temp = lines[index].replace(";" , ",")
        
        temp +=  "\t\t" + intemp +" ; \n"
        combinational.write(temp)
    
    elif lines[index].count("wire"):
        break                
    
    else:
        combinational.write(lines[index])
    index += 1

while( index < len(lines)):
    if lines[index].count(";"):
        break
    for i in inout.inWire :
        if lines[index].count(i):
            state = 1
        else:
            state = 0
    
    index += 1
    
    

combinational.close()
sequential.close()
