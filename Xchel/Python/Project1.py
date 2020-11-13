#!/bin/python3

from Translation import Translator #Import custom class Translator
import dic_tb

#Create a new translator object
Trad = Translator()
#Translate the loaded file
Trad.Translate_TOP()
#Print loaded file dictionary
Trad.print()

my_tb = dic_tb.Module_tb(Trad.dic)

f = open(my_tb.module_name + "_testbench.sv", "w")
f.write(dic_tb.write_head(my_tb))
f.write(dic_tb.write_instMod(my_tb))
f.write(dic_tb.write_body(my_tb))
f.close()