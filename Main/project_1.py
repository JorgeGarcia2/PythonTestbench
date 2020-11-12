#!/bin/python3

from Translation import Translator
import dic_tb

Trad = Translator()
Trad.Translate_TOP()

print(Trad.dic)

my_tb = dic_tb.Module_tb(Trad.dic)


f = open(my_tb.module_name + "_testbench.sv", "w")
f.write(dic_tb.write_head(my_tb))
f.write(dic_tb.write_instMod(my_tb))
f.write(dic_tb.write_body(my_tb))
f.close()