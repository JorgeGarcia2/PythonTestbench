#!/usr/bin/python3

import random
import string # To convert number between different bases

digs = string.digits + string.ascii_letters

class Port:
    def __init__(self, port_def):
        self.namePort = port_def[0]                          #Nombre del puerto
        if (port_def[-1] != None):
            self.value = port_def[-1]                             #Valor inicial
        self.rangePort = port_def[1] - port_def[2] #Tamaño del bus -1
        if (self.rangePort > 0):
            self.downtoPort = True                           #Down to 
        else:
            self.rangePort *= -1                             #To
            self.downtoPort = False

        if (self.value == 'R'):                          
            self.value = random.randint(0, 2**(self.rangePort)) #Asigna valor random al valor inicial
        elif (self.value != 'c' and self.value != 'r'):
            self.value = int(self.value)                        #Asigna valor preestablecido al valor inicial
        self.print    

    #Metodo para concatenar NamePort + _TB
    def namePortTB(self):
        return self.namePort + "_TB"

    #Metodo para obtener la representacion del rango
    def rangePortTB(self):
        strRange = ""
        if (self.rangePort != 0):
            if(self.downtoPort): 
                strRange = f"[{self.rangePort}:0]"
            else: 
                strRange = f"[0:{self.rangePort}]"
        return strRange

    #Metodo que imprime el valor del puerto
    def printValue(self, base = "dec"):
        if (base == "bin"):
            valueStr = f"b{int2base(self.value, 2)}"
        elif (base == "hex"):
            valueStr = f"h{int2base(self.value, 16)}"
        else:
            valueStr = f"d{int2base(self.value, 10)}"
        
        self.value += 1
        if(self.value >= 2**(self.rangePort + 1)):
            self.value = 0
        
        return f"{self.namePortTB()} = {self.rangePort + 1}'{valueStr}"

    #Metodo para imprimir los atributos del objeto Port
    def print(self):
        print(f"\nPort name: {self.namePort}\nBus size: {self.rangePort}\nPort value: {self.value}\n")

         

class Module_tb:
    def __init__(self, module_def):
        self.module_name = module_def["module"]
        self.inputs = []
        self.outputs = []
        self.clock = None
        self.reset = None

        for i in module_def["input"]:
            if (i[3] == 'c'):
                self.clock = Port(i)
            elif (i[3] == 'r'):
                self.reset = Port(i)
            else:
                self.inputs.append(Port(i))

        for o in module_def["output"]:
            self.outputs.append(Port(o))

    def nameTB(self):
        return self.module_name + "_TB"

def int2base(x, base):
    if x < 0:
        sign = -1
    elif x == 0:
        return digs[0]
    else:
        sign = 1

    x *= sign
    digits = []

    while x:
        digits.append(digs[int(x % base)])
        x = int(x / base)

    if sign < 0:
        digits.append('-')

    digits.reverse()

    return ''.join(digits)

def write_head(data_tb):
    head =  ("//time scale\n"
    "`timescale 1ns/1ps\n"
    "//Main Testbench Starts here\n"
    f"module {data_tb.module_name}_TB;\n")

    if (data_tb.clock != None):
        head += f"reg {data_tb.clock.namePortTB()};\n"
    if (data_tb.reset != None):
        head += f"reg {data_tb.reset.namePortTB()};\n"
    for i in data_tb.inputs:
        head += f"reg {i.rangePortTB()}{i.namePortTB()};\n"
    for i in data_tb.outputs:
        head += f"wire {i.rangePortTB()}{i.namePortTB()};\n"
    return head

def write_instMod(data_tb):
    instMod = f"\n{data_tb.module_name} UUT("
    if (data_tb.clock != None):
        instMod += f".{data_tb.clock.namePort}({data_tb.clock.namePortTB()}), "
    if (data_tb.reset != None):
        instMod += f".{data_tb.reset.namePort}({data_tb.reset.namePortTB()}), "
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
    body = ""

    if (data_tb.clock != None): #secuencial
        body += f"\talways forever #1 {data_tb.clock.namePortTB()} = ~{data_tb.clock.namePortTB()};\n\n"
    
    body += ("initial\n"
    "\tbegin\n"
    f'\t\t$dumpfile("{data_tb.module_name}.vcd");\n'
    f"\t\t$dumpvars(1, {data_tb.nameTB()});\n\n")

    if (data_tb.clock != None):
        body += f"\t\t{data_tb.clock.namePortTB()} = 0;\n"

    if (data_tb.reset != None):
        body += f"\t\t{data_tb.reset.namePortTB()} = 1;\n"

    #This initializes all the ports
    for i in data_tb.inputs:
        body += f"\t\t{i.namePortTB()} = {i.rangePort + 1}'b0;\n"
    
    if (data_tb.reset != None):
        body += "\n\t\t#2\n"
        body += f"\t\t{data_tb.reset.namePortTB()} = 0;\n"
    
    body += "\n\t\t#1\n"

    for times in range(10):
        for i in data_tb.inputs:
            body += f"\t\t{i.printValue('bin')};\n"
        body += "\n\t\t#1\n"
    
    body += "\t\t$finish;\n\tend\nendmodule"
    
    return body
