#!/bin/python3.6
from Testbench import Testbench

my_testench = Testbench()

response = "Y"
while(response != "N" and response != "n"):
    if(my_testench.getFile()):
        #print(my_testench.designCode)
        my_testench.getData()
        #print(f"\n\n{my_testench.data}")
        my_testench.createTB()
        break
    else:
        response = input("\nDo you want to try again? (Y/N)\n")