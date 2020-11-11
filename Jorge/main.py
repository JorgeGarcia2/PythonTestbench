#!/bin/python3.8
from  Testbench_Generator import Testbench

my_testench = Testbench()

response = "Y"
while(response == "Y"):
    if(my_testench.getFile()):
        #print(my_testench.designCode)
        my_testench.getData()
        my_testench.getInitVal()
        #print(f"\n\n{my_testench.data}")
        my_testench.getRadix_Time()
        my_testench.createTB()
        break
    else:
        response = input("\nDo you want to try again? (Y/N)\n")
