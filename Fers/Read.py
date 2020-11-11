#!/usr/bin/python3.6
import re
import random
import Write

class Port:

    def __init__(self, port_def):
        self.namePort = port_def[0]                         #Name of the port
        if (port_def[-1] != None):
            self.value = port_def[-1]                       #Initial value
        self.rangePort = port_def[1] - port_def[2]          #Bus size -1
        if (self.rangePort > 0):
            self.downtoPort = True                          #Down to 
        else:
            self.rangePort *= -1                            #To
            self.downtoPort = False
        
        if (self.value == 'R'):                     
            self.value = random.randint(0, 2**(self.rangePort)) #Assigns a random initial value
        elif (self.value != 'c' and self.value != 'r'):
            self.value = int(self.value)                        #Assigns the user-defined initial value
        self.print    

    #Method to concatenate the name of the port with "_TB"
    def namePortTB(self):
        return self.namePort + "_TB"

    #Method to obtain the range representation
    def rangePortTB(self):
        strRange = ""
        if (self.rangePort != 0):
            if(self.downtoPort): 
                strRange = f"[{self.rangePort}:0]"
            else: 
                strRange = f"[0:{self.rangePort}]"
        return strRange

    #Method to print the value of the port
    def printValue(self, base = "dec"):
        if (base == "bin"):
            valueStr = f"b{Write.int2base(self.value, 2)}"
        elif (base == "hex"):
            valueStr = f"h{Write.int2base(self.value, 16)}"
        else:
            valueStr = f"d{Write.int2base(self.value, 10)}"
        
        self.value += 1
        if(self.value >= 2**(self.rangePort + 1)):
            self.value = 0
        
        return f"{self.namePortTB()} = {self.rangePort + 1}'{valueStr}"

    #Method to print the attributes of the port
    def print(self):
        print(f"\nPort name: {self.namePort}\nBus size: {self.rangePort}\nPort value: {self.value}\n")


class Module_tb:

    def __init__(self):
        self.module_name = ""
        self.inputs = []
        self.inputs_temp = []
        self.outputs = []
        self.outputs_temp = []
        self.clock = None
        self.reset = None

    def signal_settings(self):
        for i in self.inputs_temp:
            if (i[3] == 'c'):
                self.clock = Port(i)
            elif (i[3] == 'r'):
                self.reset = Port(i)
            else:
                self.inputs.append(Port(i))

        for o in self.outputs_temp:
            self.outputs.append(Port(o))

    def nameTB(self):
        return self.module_name + "_TB"

######################### MAIN CODE #########################

#Ask for the name of the Verilog design file and store it in 'File_Name'
File_Name = input("What is the name of the Verilog design file? ")
#Add '.sv' extension if not existent
if not re.match(r"\.sv", File_Name): File_Name = File_Name + ".sv"
#Open file
File = open(File_Name, "r")

#Ask for the usage of random or manual initial values
if (input("Do you want initial values to be random? [type n for manual, any other key for random]\n") == 'n'):
    use_random_values = False
else:
    use_random_values = True

#Create a 'Module_tb' object for later use
my_tb = Module_tb()

#Iterate over lines in the file
for line in File.readlines():
    #Iterate over found RegEx in the line
    for mat in re.finditer(r"\W*((output|input|^module)\s*(reg|wire|\s*)\s*(\[\d+:\d+\]\s*|\s+)\s*(((,\s*|\s*)((?!input|output|inout)[_a-zA-Z]\w*\s*))*))",line):
        match = mat.groups()[1:]
        #Check if the keyword is module
        if match[0].replace(" ", "") != "module":
            #Save the type of data (Input, Output, Inout)
            typ = match[0]
            #Check if the group containing the range of the data bus is existent, if not load with bus from 0 to 0
            if re.search(":",match[2]): ran = re.split(r"[:\[\] ]", match[2])
            else: ran = ["","0","0",""]
            #Iterate through all names in the name group of the RegEx
            for i in re.findall(r"[\w'-]+", match[3]):
                lis = [] #Start with an empty list
                lis.append(i.replace(" ", "")) #Add data name
                lis.append(int(ran[1].replace(" ", ""))) #Add inferior range
                lis.append(int(ran[2].replace(" ", ""))) #Add superior range
                # If the type is input, do pertinent assignments of the last item
                # to know if data has a clock, reset, random or init value
                if typ == "input":
                    #Check if it's a clock with size 1 and starting with c
                    if re.search("\w*[cC][lL]\w*[kK]\w*", i) and ran[1] == ran[2]: lis.append("c")
                    #Check if it's a reset with size 1 and starting with r
                    elif re.search("\w*[rR]\w*[sS]\w*[tT]\w*", i) and ran[1] == ran[2]: lis.append("r")
                    #Check if the initial values are automated, in that case use random values
                    #In other case, ask the user
                    elif not use_random_values: lis.append(input("What's the initial value of ("+i+") of range ["+str(int(ran[1])+int(ran[2]))+"]?\n(Type 'R' for a random value)\n"))
                    else: lis.append("R")
                    #Append list to dictionary
                    my_tb.inputs_temp.append(lis)
                elif typ == "output":
                    my_tb.outputs_temp.append(lis)
        else:
            #If the keyword is module, only use the name
            my_tb.module_name = match[4].replace(" ", "")
#Close the file
File.close()

#Just to make sure all data is in place
print(my_tb.module_name, my_tb.inputs_temp, my_tb.outputs_temp)

#Create the port settings (name, range, initial value)
my_tb.signal_settings()

#Open a Testbench file and write all the contents
f = open(my_tb.module_name + "_testbench.sv", "w")
f.write(Write.write_head(my_tb))
f.write(Write.write_instMod(my_tb))
f.write(Write.write_body(my_tb))
f.close()

############################ END ############################