#!/bin/python3.6
 # !/bin/python3.8 --> From Jorge
import random
import string

class Module:
    def __init__(self, module_def):
        self.module_name = module_def["module"][0][0]
        self.inputs = []
        self.outputs = []
        self.clock = None
        self.reset = None

        for i in module_def["input"]:
            if (i[3] == 'c'):
                self.clock = Input(i)
            elif (i[3] == 'r'):
                self.reset = Input(i)
            else:
                self.inputs.append(Input(i))

        for o in module_def["output"]:
            self.outputs.append(Port(o))

    def nameTB(self):
        return self.module_name + "_TB"
        
    def print(self):
        print(f"Module Name: {self.module_name}\n")
        print("\nInputs: \n") 
        for element in self.inputs:
            element.print()
        print("\nOutputs: \n")
        for element in self.outputs:
            element.print()
        self.clock.print()
        self.reset.print()

class Port:
    def __init__(self, port_def):
        self.value=0
        self.namePort = port_def[0]     #Port Name
        self.rangePort = port_def[1] - port_def[2] #Bus size -1
        if (self.rangePort > 0):
            self.downtoPort = True      #Down to 
        else:
            self.rangePort *= -1        #To
            self.downtoPort = False
            
        self.print()

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

    #Metodo para imprimir los atributos del objeto Port
    def print(self):
        print(f"\nPort name: {self.namePort}\nBus size: {self.rangePort}\nPort value: {self.value}\n")
        
        
class Input(Port):
    def __init__(self, port_def):
        Port.__init__(self, port_def)
        self.value = port_def[3]                       #Initial values
        if (port_def[4] != 0): self.step = port_def[4] #Initial values
        else: self.step = 1

        if (self.value == 'R'):                          
            self.value = random.randint(0, 2**(self.rangePort)) #Asigna valor random al valor inicial
        elif (self.value != 'c' and self.value != 'r'):
            self.value = int(self.value)                        #Asigna valor preestablecido al valor inicial
        
    def printValue(self, base = "dec"):
        if (base == "bin"):
            if(self.downtoPort): valueStr = f"b{int2base(self.value, 2)}"
            else: valueStr = f"b{int2base(self.value, 2)[::-1]}"
        elif (base == "hex"):
            valueStr = f"h{int2base(self.value, 16)}"
        else:
            valueStr = f"d{int2base(self.value, 10)}"
        
        self.value += self.step
        if(self.value >= 2**(self.rangePort + 1)):
            self.value = 0
        
        return f"{self.namePortTB()} = {self.rangePort + 1}'{valueStr}"
        

               
                 
def int2base(x, base):
    digs = string.digits + string.ascii_letters
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