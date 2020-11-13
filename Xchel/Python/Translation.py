#!/bin/python3

import os
import re

class Translator:

    #Class initialization with load method
    def __init__(self):
        self.load()

    #Load method for checking if file exists
    def load(self):  
        #Dictionary initialization with 'input', 'output' and 'module' subdictionaries for future expanding
        self.dic={}
        self.dic["input"]=[]
        self.dic["output"]=[]
        self.dic["inout"]=[]
        self.dic["module"]=""

        while 1:
            self.Fname=input("What is the top model's filename? ")
            if not ".sv" in self.Fname: self.Fname=self.Fname+".sv"
            try:
                F=open(self.Fname,"r")
                F.close()
                break
            except:
                print("File not found!")
                self.Fname=""
                for fil in os.listdir("."):
                    print(fil)
                    if ".sv" in fil and not "_testbench" in fil:
                        self.Fname=fil
                        print("sv file found, using: "+fil+"\n")
                        break
                if self.Fname=="": print("sv file not found in the directory, please enter a valid file along with its path")
                else: break
                print("\n") 

    #Translation function
    def Translate_TOP(self):
        #Open file
        F=open(self.Fname,"r")
        o=input("Do you want all values to be automatic? [Y/n]\n")
        #If user wants it, add values manually with a flag
        if o=="n"or o=="N": f=False
        else: f=True

        #Iterate over read lines in the file
        for line in F.readlines():

            #Iterate over found RegEx in the line
            for mat in re.finditer(r"\W*((output|input|module)\s*(reg|wire|\s*)\s*(\[\d+:\d+\]\s*|\s+)\s*(((,\s*|\s*)((?!input|output|inout)[_a-zA-Z]\w*\s*))*)((\[\d+:\d+\]\s*)*|\s+))",line):
                match=mat.groups()[1:]
                #Check if the keyword is module
                if match[0]!="module":
                    #Save the type of data (Input, Output, Inout)
                    typ=match[0]
                    #Start with empty range for the data
                    ran=[]
                    #Check if the group containing the range of the data bus is existent, if not load with bus from 0 to 0
                    if ":" in match[2]:
                        for d in re.finditer(r"(\d+)",match[2]):
                            if d.group()!="": ran.append(d.group())
                    else: ran=["0","0"]
                    dim=[]
                    if ":" in match[7]:
                        for d in re.finditer(r"(\d+)",match[7]):
                            if d.group()!="":
                                dim.append(d.group())

                    else: dim=["0","0"]
                    #Iterate over all data names with the same type, range and dimension
                    for i in re.findall(r"[\w'-]+",match[3]):
                        #Initialize dictionary
                        lis=[]
                        lis.append(i)
                        #Add range list
                        lis.append(int(ran[0]))
                        lis.append(int(ran[1]))
                        # If the type is input, do pertinent assignments of the last item
                        # to know if data has a clock, reset, random or init value
                        if typ=="input":
                            #Check if it's a clock with size 1 and starting with c
                            if re.search(r"\w*[cC][lL]\w*[kK]\w*",i) and ran[0]==ran[1]:
                                if f:
                                    lis.append("c")
                                else:
                                    lis.append(input("The signal ("+i+") was detected as clock, enter if it's correct\n"
                                "If it's not correct, Type 'r' for reset and 'R' for random type of inputs or any value for its initial value:\n"))
                                    if lis[3]=="": lis[3]="c"
                            #Check if it's a reset with size 1 and starting with r
                            elif re.search(r"\w*[rR]\w*[sS]\w*[tT]\w*",i) and ran[0]==ran[1]:
                                if f:
                                    lis.append("r")
                                else:
                                    lis.append(input("The signal ("+i+") was detected as reset, enter if it's correct\n"
                                "If it's not correct, Type 'c' for clock, 'R' for random type of inputs or any decimal value for its initial value in consecutive signals:\n"))
                                    if lis[3]=="": lis[3]="r"
                            #Check if the initial values are automated, in that case use random values
                            #In other case, ask the user
                            elif not f:
                                lis.append(input("What are the values of ("+i+") of range ["+ran[0]+":"+ran[1]+"] going to be?\n"
                            "(Type a decimal value for consecutive values, 'c' for clock and 'r' for reset and ('R' or blank) for random type of inputs)\n"))
                                if lis[3]=="": lis[3]="R"
                            else: lis.append("R")
                            if lis[3]!="c" and lis[3]!="r" and lis[3]!="R":
                                try:
                                    int(lis[3])
                                except:
                                    print("Value not recognized, using default \"R\" value\n")
                                    lis[3]="R"
                        for j in range(len(dim)): lis.append(int(dim[j]))
                        self.dic[typ].append(lis)
                else:
                    #If the keyword is module, only use the name
                    self.dic["module"]=match[4]
        #Close the file
        F.close()

    def print(self):
        
        for i in self.dic:
            if i=="module":
                print(i+":"+self.dic["module"]+"\n")
            else:
                print(i+":")
                for j in self.dic[i]:
                    print("|->Name: "+j[0]+"\n|   |->Range: ["+str(j[1])+":"+str(j[2])+"]",end="")
                    if i=="input":
                        print("\n|   |->ini: "+j[3]+"\n|   \\->Dim: ",end="")
                        for x in range(4,len(j),2): print("["+str(j[x])+":"+str(j[x+1])+"] ",end="")
                    else:
                        print("\n|   \\->Dim: ",end="")
                        for x in range(3,len(j),2): print("["+str(j[x])+":"+str(j[x+1])+"] ",end="")
                        print()
                    print("\n",end="")
                print("\n")