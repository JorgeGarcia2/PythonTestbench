#!/usr/bin/python3

import random
import string # To convert number between different bases

digs = string.digits + string.ascii_letters

class Port:
    def __init__(self, port_def):
        self.namePort = port_def[0]                          #Nombre del puerto
        if (port_def[-1] != None):
            self.value = port_def[3]                             #Valor inicial
        self.rangePort = port_def[1] - port_def[2] #Tamaño del bus -1
        if (self.rangePort > 0):
            self.downtoPort = True                           #Down to 
        else:
            self.rangePort *= -1                             #To
            self.downtoPort = False

        if (self.value == 'R'):                          
            self.values = random.randint(0, 2**(self.rangePort)) #Asigna valor random al valor inicial
        elif (self.value != 'c' and self.value != 'r'):
            self.values = int(self.value)                        #Asigna valor preestablecido al valor inicial
        self.dim=port_def[3:]
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
            valueStr = f"b{int2base(self.values, 2)}"
        elif (base == "hex"):
            valueStr = f"h{int2base(self.values, 16)}"
        else:
            valueStr = f"d{int2base(self.values, 10)}"
        
        
        if (self.value == 'R'):                          
            self.values = random.randint(0, 2**(self.rangePort)) #Asigna valor random al valor inicial
        else:
            self.values += 1                        #Asigna valor siguiente
        if(self.values >= 2**(self.rangePort + 1)):
            self.values = 0
        
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
                if self.clock == None: self.clock = Port(i)
                else:
                    i[3]="R"
                    self.inputs.append(Port(i))
            elif (i[3] == 'r'):
                if self.reset == None: self.reset = Port(i)
                else:
                    i[3]="R"
                    self.inputs.append(Port(i))
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
    head =  ("//Testbench created automatically with a program written in Python 3.8 by:\n"
    "//\tGarcía Vidal Jorge Alberto\n"
    "//\tGuevara Zavala Arturo\n"
    "//\tMorales Hurtado David Xchel\n"
    "//\tRodriguez Contreras Luis Fernando\n"
    "//\n//For the first project in the class of professor:\n"
    "//\tCarolina Rosas Huerta\n//\n"
    "//In the Silicon Verification Program\n//\n"
    "//And modified according to the criteria of:\n"
    "//\tDavid Xchel Morales Hurtado\n\n"
    "//time scale\n"
    "`timescale 1ns/1ps\n\n"
    "//Main Testbench Starts here\n"
    f"module {data_tb.module_name}_TB();\n\n"
    "\t//Clock and reset signal instantiation as registers\n")

    if (data_tb.clock != None):
        head += f"\treg {data_tb.clock.namePortTB()};\n"
    if (data_tb.reset != None):
        head += f"\treg {data_tb.reset.namePortTB()};\n"
    head+="\n\t//Instantiation of other input signals\n"
    for i in data_tb.inputs:
        head += f"\treg {i.rangePortTB()}{i.namePortTB()};\n"
    head+="\n\t//Instantiation of output signals\n"
    for i in data_tb.outputs:
        head += f"\twire {i.rangePortTB()}{i.namePortTB()}"
        for j in range(0,len(i.dim),2):
            if i.dim[j] != i.dim[j+1]:
                head+=f"[{i.dim[j]}:{i.dim[j+1]}]"
        head+=";\n"
    return head

def write_instMod(data_tb):
    instMod = (f"\n\t//Instantiation of the {data_tb.module_name} module\n"
    f"\t{data_tb.module_name} UUT(")
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
        body += ("\t//Inicialization of the clock signal\n"
        f"\talways forever #1 {data_tb.clock.namePortTB()} = ~{data_tb.clock.namePortTB()};\n\n")
    
    body += ("\t//Initialization of the testing values\n\tinitial\n"
    "\tbegin\n"
    f'\t\t//File and variables to simulate\n\t\t$dumpfile("{data_tb.module_name}.vcd");\n'
    f"\t\t$dumpvars(1, {data_tb.nameTB()});\n\n"
    "\t\t//Initial value of the input signals with reset on\n")

    try:
        clki=int(input("What is the initial value for the clock? (default is 0)\n"))%2
    except:
        print("Value unknown, using default: 0\n")
        clki=0
    if (data_tb.clock != None):
        body += f"\t\t{data_tb.clock.namePortTB()} = {clki};\n"

    try:
        rsti=int(input("What is the value when the reset signal is active? (default is 1)\n"))%2
    except:
        print("Value unknown, using default: 1\n")
        rsti=1
    if (data_tb.reset != None):
        body += f"\t\t{data_tb.reset.namePortTB()} = {rsti};\n"
    try:
        cy=int(input("How many cycles do you want the program to iterate over? (default is 10) "))
        if cy>1000: raise Exception
    except:
        print("Value unknown or too big, using default: 10\n")
        cy=10

    #This initializes all the ports
    for i in data_tb.inputs:
        body += f"\t\t{i.namePortTB()} = {i.rangePort + 1}'b0;\n"
    
    body+="\n\t\t//Turning off reset signal for test begining"
    if (data_tb.reset != None):
        body += "\n\t\t#2\n"
        body += f"\t\t{data_tb.reset.namePortTB()} = {(rsti+1)%2};\n"
    
    body+=f"\n\t\t//The testbench will iterate over {cy} cycles\n"

    for times in range(cy):
        body += f"\t\t//Iteration: {times+1}\n\t\t#1\n"
        for i in data_tb.inputs:
            body += f"\t\t{i.printValue('bin')};\n"
        body+="\n"
    
    body += "\n\t\t//Ending iteration\n\t\t#1\n\t\t$finish;\n\tend\nendmodule"
    
    return body
