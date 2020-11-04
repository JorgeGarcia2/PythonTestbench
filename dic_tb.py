#!/usr/bin/python3.6
import random

class Port:
    def __init__(self, port_def):
        self.namePort = port_def[0]
        difPort = port_def[1] - port_def[2]
        if (difPort > 0):
            self.rangePort = difPort
            self.downtoPort = True
        else:
            self.rangePort = difPort * -1
            self.downtoPort = False

    def namePortTB(self):
        return self.namePort + "_tb"

    def rangePortTB(self):
        strRange = ""
        if (self.rangePort != 0):
            if(self.downtoPort): 
                strRange = f"[{self.rangePort}:0]"
            else: 
                strRange = f"[0:{self.rangePort}]"
        return strRange
    
    def randValue(self, base = "bin"):
        if (base == "bin"):
            value = decimalToBinary(random.randint(0,2**(self.rangePort)))
        else:
            value = random.randint(0,2**(self.rangePort))
        return f"{self.namePortTB()} = {self.rangePort + 1}'d{value}"

         

class Module_tb:
    def __init__(self, module_def):
        self.module_name = module_def["module_name"]
        self.inputs = []
        self.outputs = []

        for i in module_def["input"]:
            self.inputs.append(Port(i))

        for o in module_def["output"]:
            self.outputs.append(Port(o))

    def nameTB(self):
        return self.module_name + "_tb"

def decimalToBinary(num):
    bin = ""
    if (num > 1):
        bin += f"{num % 2}"
        num = num // 2
    return bin 

def write_head(data_tb):
    head =  ("//time scale\n"
    "`timescale 1ns/1ps\n"
    "//Main Testbench Starts here\n"
    f"module {data_tb.module_name}_tb;\n")

    for i in data_tb.inputs:
        head += f"wire {i.rangePortTB()}{i.namePortTB()};\n"
    for i in data_tb.outputs:
        head += f"reg {i.rangePortTB()}{i.namePortTB()};\n"
    return head

def write_instMod(data_tb):
    instMod = f"\n{data_tb.module_name} UUT("
    for i in data_tb.inputs:
        instMod += f".{i.namePort}({i.namePortTB()}), "
    for i in data_tb.outputs:
        instMod += f".{i.namePort}({i.namePortTB()})"
        if(i != data_tb.outputs[-1]):
            instMod += ", "
        else: 
            instMod += ");\n\n"
        
    return instMod

def write_body(data_tb):
    body = ("initial\n"
    "\tbegin\n"
    f'\t\t$dumpfile("{data_tb.module_name}.vcd");\n'
    f"\t\t$dumpvars(1, {data_tb.nameTB()}.vcd);\n\n")

    for i in data_tb.inputs:
        body += f"\t\t{i.namePortTB()} = {i.rangePort + 1}'b0';\n"
    body += "\n\t\t#1\n"

    for tiemes in range(10):
        for i in data_tb.inputs:
            body += f"\t\t{i.randValue()};\n"
        body += "\n\t\t#1\n"
    
    body += "\t\t$finish;\n\tend\nendmodule"
    
    return body



design_data = {
    "module_name": 'TOP',
    "input" : [["A", 31, 0], ["B", 0, 0], ["C", 0, 0]],
    "output": [["X", 0, 0], ["Y", 7, 0], ["Z", 0, 5]]
}

my_tb = Module_tb(design_data)

f = open(my_tb.module_name + "_testbench.sv", "w")

f.write(write_head(my_tb))
f.write(write_instMod(my_tb))
f.write(write_body(my_tb))

f.close()