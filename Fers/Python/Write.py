#!/usr/bin/python3.6
import string       #To convert number between different bases

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

    if (data_tb.clock != None): #sequential
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
