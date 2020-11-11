#!/bin/python3.6
 # !/bin/python3.8 --> From Jorge
import re
from ModulePort import Module


class Testbench:
    def __init__(self):
        self.designCode = None
        self.data = {"module":[],
                    "input": [],
                    "output": []}
        self.moduleTOP = None
        
        self.time = 10
        self.radix = "dec"

    #Prompts for valid file name and assigns file text to string
    def getFile(self):
        noError = True
        fileName = input("\nWhat is the top model's filename? ")
        if (not re.match(r"\.sv",fileName)): 
            fileName = fileName + ".sv"
        try: 
            f = open(fileName,"r")
            self.designCode = f.read()
            print("File read successfully")
            f.close()
        except:
            print(f"The {fileName} file was not found or you do not have read permissions")
            noError = False
        return noError

    def getData(self):
        #Remove all the comments
        self.designCode = re.sub(r"(\/\/.*)", "", self.designCode) #erase comment line
        self.designCode = re.sub(r"(\/\*)(.|\n)*?(\*\/)", "", self.designCode) #erase block comment
  
        pattern = "\W*((module|input|output|inout)\s*(reg|\s*)\s*(\[\d+:\d+\]\s*|\s+)\s*(((,\s*|\s*)((?!input|output|inout)[_a-zA-Z]\w*))*))"
        
        match = re.search(pattern, self.designCode)
        self.designCode = re.sub(pattern, "", self.designCode, 1)
        
        while match:
            names = match.group(5).replace(" ","").split(",")
            if re.search(":",match.group(4)): 
                ran = re.split(r"[:\[\] ]",match.group(4))
            else: 
                ran=["","0","0",""]
                
            for i in range(0,len(names)):
                self.data[match.group(2).replace(' ','')].append([names[i], int(ran[1]), int(ran[2])])
                   
            match = re.search(pattern, self.designCode)
            self.designCode = re.sub(pattern, "", self.designCode, 1)

    def getInitVal(self):
        print("\nEnter the initial value and the steps for the entries listed below separated by enter.\n"
              "(The default values ​​will be a random numbering and steps of 1).\n")
        for e in self.data["input"]:

            if (e[1]==e[2] and re.search("\w*[cC][lL]\w*[kK]\w*", e[0])):
                res = input(f"\nInput {e[0]} has been detected as a possible clock signal. Is this correct? (Y/N)\n")
                if (res == "Y"):
                    e += ["c",0]
                    continue
            
            if (e[1]==e[2] and re.search("\w*[rR]\w*[sS]\w*[tT]\w*", e[0])):
                res = input(f"\nInput {e[0]} has been detected as a possible reset signal. Is this correct? (Y/N)\n")
                if (res == "Y"):
                    e += ["r",0]
                    continue

            res = input(f"\n{e[0]} [{e[1]}:{e[2]}]\n\tInitial value: ")
            if res != "": e.append(int(res))
            else: e.append("R")
            res = input(f"\tStep: ")
            if res != "": e.append(int(res))
            else: e.append(1)
        
        self.moduleTOP = Module(self.data)

    def write_head(self):
        head =  ("//time scale\n"
        "`timescale 1ns/1ps\n"
        "//Main Testbench Starts here\n"
        f"module {self.moduleTOP.module_name}_TB;\n")

        if (self.moduleTOP.clock != None):
            head += f"reg {self.moduleTOP.clock.namePortTB()};\n"
        if (self.moduleTOP.reset != None):
            head += f"reg {self.moduleTOP.reset.namePortTB()};\n"
        for i in self.moduleTOP.inputs:
            head += f"reg {i.rangePortTB()}{i.namePortTB()};\n"
        for i in self.moduleTOP.outputs:
            head += f"wire {i.rangePortTB()}{i.namePortTB()};\n"
        return head

    def write_instMod(self):
        instMod = f"\n{self.moduleTOP.module_name} UUT("
        if (self.moduleTOP.clock != None):
            instMod += f".{self.moduleTOP.clock.namePort}({self.moduleTOP.clock.namePortTB()}), "
        if (self.moduleTOP.reset != None):
            instMod += f".{self.moduleTOP.reset.namePort}({self.moduleTOP.reset.namePortTB()}), "
        for i in self.moduleTOP.inputs:
            instMod += f".{i.namePort}({i.namePortTB()}), "
        for i in self.moduleTOP.outputs:
            instMod += f".{i.namePort}({i.namePortTB()})"
            if(i != self.moduleTOP.outputs[-1]):
                instMod += ", "
            else: 
                instMod += ");\n\n"
            
        return instMod

    def write_body(self):
        body = ""

        if (self.moduleTOP.clock != None): #sequential
            body += f"\talways forever #1 {self.moduleTOP.clock.namePortTB()} = ~{self.moduleTOP.clock.namePortTB()};\n\n"
        
        body += ("initial\n"
        "\tbegin\n"
        f'\t\t$dumpfile("{self.moduleTOP.module_name}.vcd");\n'
        f"\t\t$dumpvars(1, {self.moduleTOP.nameTB()});\n\n")

        if (self.moduleTOP.clock != None):
            body += f"\t\t{self.moduleTOP.clock.namePortTB()} = 0;\n"

        if (self.moduleTOP.reset != None):
            body += f"\t\t{self.moduleTOP.reset.namePortTB()} = 1;\n"

        #This initializes all the ports
        for i in self.moduleTOP.inputs:
            body += f"\t\t{i.namePortTB()} = {i.rangePort + 1}'b0;\n"
        
        if (self.moduleTOP.reset != None):
            body += "\n\t\t#2\n"
            body += f"\t\t{self.moduleTOP.reset.namePortTB()} = 0;\n"
        
        body += "\n\t\t#1\n"

        for times in range(self.time):
            for i in self.moduleTOP.inputs:
                body += f"\t\t{i.printValue(self.radix)};\n"
            body += "\n\t\t#1\n"
        
        body += "\t\t$finish;\n\tend\nendmodule"
        
        return body
    
    def getRadix_Time(self):
        self.time = int(input("How many time intervals do you want? "))
        self.radix = input("Choose the test vectors radix ('bin', 'dec' or 'hex') ")
    
    def createTB(self):
        f = open(self.moduleTOP.module_name + "_testbench.sv", "w")
        f.write(self.write_head())
        f.write(self.write_instMod())
        f.write(self.write_body())
        f.close()
        print(f"\n{self.moduleTOP.module_name}_testbench.sv file has been created successfully")