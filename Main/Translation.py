#!/bin/python3
import re

class Translator:

    #Dictionary initialization with 'input' and 'output' lists
    dic={}
    dic["input"]=[]
    dic["output"]=[]

    #Class initialization
    def __init__(self):
        #Ask for a valid filename while not valid
        self.Fname=input("What is the top model's filename? ")
        #Add extension if not existent
        if not re.match(r"\.sv",self.Fname): self.Fname=self.Fname+".sv"

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
            for mat in re.finditer(r"\W*((output|input|module)\s*(reg|wire|\s*)\s*(\[\d+:\d+\]\s*|\s+)\s*(((,\s*|\s*)((?!input|output|inout)[_a-zA-Z]\w*\s*))*))",line):
                match=mat.groups()[1:]
                #Check if the keyword is module
                if match[0].replace(" ", "")!="module":
                    #Save the type of data (Input, Output, Inout)
                    typ=match[0]
                    #Check if the group containing the range of the data bus is existent, if not load with bus from 0 to 0
                    if re.search(":",match[2]): ran=re.split(r"[:\[\] ]",match[2])
                    else: ran=["","0","0",""]
                    #iterate over all names in the name group of the RegEx
                    for i in re.findall(r"[\w'-]+",match[3]):
                        lis=[] #Start with an empty list
                        lis.append(i.replace(" ", "")) #Add data name
                        lis.append(int(ran[1].replace(" ", ""))) #Add inferior range
                        lis.append(int(ran[2].replace(" ", ""))) #Add superior range
                        # If the type is input, do pertinent assignments of the last item
                        # to know if data has a clock, reset, random or init value
                        if typ=="input":
                            #Check if it's a clock with size 1 and starting with c
                            if re.search(r"\w*[cC][lL]\w*[kK]\w*",i) and ran[1]==ran[2]: lis.append("c")
                            #Check if it's a reset with size 1 and starting with r
                            elif re.search(r"\w*[rR]\w*[sS]\w*[tT]\w*",i) and ran[1]==ran[2]: lis.append("r")
                            #Check if the initial values are automated, in that case use random values
                            #In other case, ask the user
                            elif not f: lis.append(input("What's the initial value of ("+i+") of range ["+
                            str(int(ran[1])+int(ran[2]))+"]?\n"+
                            "(Type 'c' for clock and 'r' for reset and ('R' or blank) for random type of inputs)\n"))
                            else: lis.append("R")
                            #Append list to dictionary
                        self.dic[typ].append(lis)
                else:
                    #If the keyword is module, only use the name
                    self.dic[match[0].replace(" ", "")]=match[4].replace(" ", "")
        #Close the file
        F.close()
