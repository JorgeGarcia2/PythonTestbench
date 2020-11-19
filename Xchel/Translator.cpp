#include <cstdlib>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <string>
#include <vector>
#include <regex>

#include "NumGen.cpp"

using namespace std;

//Dataclass for the types of ports
class Data{

    public:

    string typ;
    int Ran[3]={0,0,0};
    string Nam;
    int val;
    int step;

    //Constructor to initialize name, range, value, step and type
    Data(string d2, int R[3], string in, int va, int s){
        Nam=d2;
        Ran[0]=R[0];
        Ran[1]=R[1];
        Ran[2]=R[2];
        typ=in;
        val=va%(2^Ran[2]);
        step=s;
    } Data(){Nam="";}//Empty constructor

    //Function to print the contents of the data object in a easier human readable way
    void Print(){
        cout << "|  |->" << Nam << endl;
        cout << "|  |  |->Range: [" << to_string(Ran[0]) << ":" << to_string(Ran[1]) << "]" <<endl;
        cout << "|  |  |->Value: " << val << endl;
        cout << "|  |  \\->Type: " << typ << endl;
        cout << "|  |" << endl;
    }
};

class Translator{

    vector<Data> infoI;
    vector<Data> infoO;
    Data clk=Data();
    Data rst=Data();
    string mod="";
    fstream file;
    bool Auto=false;
    NumGen ng;
    int cy;
    string radix;

    public:
        //Function to translate source verilog code
        void Translate(){
            string t;

            //Ask if an automatic translation is needed (Default is Yes)
            cout << "Do you want an automatic translation?[Y/n]\n";
            cin >> t;
            Auto=(t!="n"&&t!="N");

            while (1){
                //Ask for file name
                cout << "What's the file name?\n";
                cin >> t;
                //Check if the name has suffix ".sv", if not, append it
                if (t.find(".sv")==string::npos)t+=".sv";
                //Try to open file
                file.open(t, ios::in);
                if (!file) {
                    //If file cannot be opened, try to open default "design.sv" file
                    cout << "Cannot open file!\n";
                    file.open("design.sv", ios::in);
                    //If default file is found, break the cycle, if not ask again
                    if (!file) cout << "default file not found!\n";
                    else{
                        cout << "default file design.sv found, using it!\n";
                        break;
                    }
                }else{
                    //Break while if file is found
                    cout << "file found!";
                    break;
                }
            }

            //Get contents of file and close
            string fileCont = " " + string((istreambuf_iterator<char>(file) ),(istreambuf_iterator<char>()));
            file.close();

            //Erase comments
            regex e1("(//.*)");                 //erase comment line
            regex e2("(/\\*)(.|\n)*?(\\*\\/)"); //erase comment block
            fileCont = regex_replace(fileCont, e1, "");
            fileCont = regex_replace(fileCont, e2, "");

            //RegEx to find modules, inputs, outputs, inouts
            e1=regex("\\W+((module|input|output|inout)\\s*(reg|wire|\\s*)\\s*(\\[[\\w\\s\\+\\-\\*]+:[\\w\\s\\+\\-\\*]+\\]\\s*|\\s+)\\s*(((,\\s*|\\s*)((?!input|output|inout)[_a-zA-Z]\\w*))*))");
            e2=regex("[\\w:]+");

            smatch match;
            string matcha;
            string res;

            //Iterate over found keywords
            while (regex_search (fileCont,match,e1)) {
                //Initialization of temporal variables for Data construction
                string type,typ;
                vector<string> name;
                int val, steps;
                int Ran[3]={0,0,1};
                matcha = match.str();
                fileCont = match.suffix().str();
                regex_search (matcha,match,e2);
                type = match.str();
                matcha = match.suffix().str();

                //Detect if the Data type is module
                if (type.find("module")!=string::npos){
                    //Get module name
                    regex_search (matcha,match,e2);
                    mod = match.str();
                }else{
                    regex_search (matcha,match,e2);
                    string temp = match.str();

                    //Detect if reg or wire keyword is found, ignore
                    if (temp.find("reg")!=string::npos || temp.find("wire")!=string::npos){
                        matcha = match.suffix().str();
                        regex_search (matcha,match,e2);
                    }
                    temp = match.str();

                    //Detect if range is found, get values
                    if (temp.find(":")!=string::npos){
                        regex_search (temp,match,regex("\\d+"));
                        Ran[0] = stoi(match.str());
                        string matchat = match.suffix().str();
                        regex_search (matchat,match,regex("\\d+"));
                        Ran[1] = stoi(match.str());
                        Ran[2]=abs(Ran[1]-Ran[0])+1;
                        regex_search (matcha,match,e2);
                        matcha=match.suffix().str();
                        regex_search (matcha,match,e2);
                        temp=match.str();
                    }

                    //Get names for the Data while dimension string found or nothing more found
                    do{
                        temp=match.str();
                        if (temp.find(":")!=string::npos){
                            break;
                        }else{
                            name.push_back(temp);
                            matcha=match.suffix().str();
                        }
                    }
                    while (regex_search (matcha,match,e2));

                    //If the data is of type output, add to outputs
                    if (type.find("output")!=string::npos) for (string x:name) infoO.push_back(Data(x, Ran, "O", 0, 0));
                    else{
                        smatch matchat;
                        for (string x:name){
                            typ="O";
                            steps=-1;

                            //If it's of size 1 bit, check if it's clock or reset
                            if (Ran[2]==1){

                                //If c, l, k found and there is no clock in the data, ask the user if it's a clock signal
                                if (clk.Nam=="" &&regex_search(x,matchat,regex("\\w*[cC]\\w*[lL]\\w*[kK]\\w*"))){
                                    cout << "\nThe signal \"" << x << "\" was detected as clock, is this correct?(Y/N)\n";
                                    
                                    cin >> res;
                                    if (res != "N" && res != "n"){
                                        typ = "c";
                                        steps = 1;

                                    //If not and no auto translation, ask type of value
                                    }else if(!Auto){
                                        cout << "What is its value type then?([r]eset, [c]lock, [i]ncreasing, [d]ecreasing, [s]tatic, [R]andom)\n(Default is random)\n";
                                        
                                        cin >> res;
                                        (res != "r" && res != "c" && res != "d" && res != "i" && res != "s")?typ="R":typ=res;
                                    }
                                }

                                //If r, s, t found and there is no reset in the data, ask the user if it's a reset signal
                                if (rst.Nam=="" &&regex_search(x,matchat,regex("\\w*[rR]\\w*[sS]\\w*[tT]\\w*"))){
                                    cout << "\nThe signal \"" << x << "\" was detected as reset, is this correct?(Y/N)\n";
                                    
                                    cin >> res;
                                    if (res != "N" && res != "n"){
                                        typ = "r";
                                        steps = 1;

                                    //If not and no auto translation, ask type of value
                                    }else if(!Auto){
                                        cout << "What is its value type then?([r]eset, [c]lock, [i]ncreasing, [d]ecreasing, [s]tatic, [R]andom)\n(Default is random)\n";
                                        
                                        cin >> res;
                                        (res != "r" && res != "c" && res != "d" && res != "i" && res != "s")?typ="R":typ=res;
                                    }
                                }
                            }

                            //If the translation is automatic, give automatic values (Random if not clock or reset)
                            if (Auto){
                                if(typ=="c") clk=Data(x, Ran, "c", 0, 1);
                                else if(typ=="r") rst=Data(x, Ran, "r", 1, 1);
                                else infoI.push_back(Data(x, Ran, "R", 0, 0));
                            }else{
                                //Ask data type if not asked before
                                if (typ=="O"){
                                    cout << "\nWhat is the type of data of \"" << x << "\" ["+to_string(Ran[0])+":"+to_string(Ran[1])+"]?(Ther can only be one clock and reset at most)\n([r]eset, [c]lock, [i]ncreasing, [d]ecreasing, [s]tatic, [R]andom)\n(Default is random)\n";
                                    
                                    cin >> res;
                                    (res != "r" && res != "c" && res != "d" && res != "i" && res != "s")?typ="R":typ=res;
                                }
                                //Detect problems
                                (typ=="c"&&clk.Nam!="")?typ="R":(typ=="r"&&rst.Nam!="")?typ="R":typ=typ;
                                cout << x << typ << endl;
                                //In case of incremental or decremental, ask for step value
                                if (typ == "d" || typ == "i"){
                                    cout << "What is the step of the operations in the signal? ";
                                    
                                    cin >> res;
                                    try {
                                        steps=stoi(res);
                                    } catch (invalid_argument &except) {
                                        steps=1;
                                    }
                                }else steps=1;
                                //In case of not Random, ask for initial value
                                if (typ != "R"){
                                    cout << "What is the initial value in the signal? ";
                                    
                                    cin >> res;
                                    try {
                                        val=stoi(res);
                                    } catch (invalid_argument &except) {
                                        (typ=="r")?val=1:val=0;
                                    }
                                }else{
                                    //Give initial value if random
                                    val = ng.RanGen(pow(2,Ran[2])-1);
                                }

                                //Store the data in its corresponding place
                                if(typ=="c") clk=Data(x, Ran, "c", 0, 1);
                                else if(typ=="r") rst=Data(x, Ran, "r", 1, 1);
                                else infoI.push_back(Data(x, Ran, typ, val, steps));
                            }
                        }
                    }
                }
            }
            return;
        }

        //Function to write the file
        void Write(){

            //Check if its an automatic translation, warn
            if (Auto) cout << "The values will be completed unless the program finds a clock or reset signal (containing clk or rst)" << endl;

            //Open file
            file.open(mod+"_testbench.sv", ios::out);

            //Ask for radix and cycles
            cout << "What is your preferred radix?('b'inary, 'o'ctal, 'd'ecimal, 'h'exadecimal)[Decimal is the default]\n";
            cin >> radix;
            cout << "How many cycles do you want?\n";
            cin >> cy;
            //If the cycles are too big or couldn't read it, default to 10
            if(!cin||cy>1000){
                cy=10;
            }
            cout << "\n\n Generation:\n\n";
            time_t now = time(0);
//////////////////////////HEADER WRITING WITH DATE AND NAME///////////////////////////
            Filout("/******************************************************************\n"
                "* Testbench created automatically with a program written in C++ by:\n"
                "*\t - Morales Hurtado David Xchel\n*\n"
                "* For the first project in the class of professor:\n"
                "*\t - Carolina Rosas Huerta\n"
                "* In the Silicon Verification Program\n*\n"
                "* \tDesign Name : "+mod+"\n"
                "* \tFile Name : "+mod+"_testbench.sv\n"
                "* \tDate: "+ctime(&now)+"\n*\n");
            if (Auto) Filout("* The program mode used was Automatic\n");
            else Filout("* The program mode used was Custom\n");
            Filout("******************************************************************/\n\n"
            "//time scale\n"
            "`timescale 1ns/1ps\n\n"

            //Initialization of testbench module with input and output signals
            "//Main Testbench Starts here\n"
            "module "+mod+"_tb();\n\n");
            Filout("   //Input signals\n");
            if(clk.Nam!="") Filout("   reg "+clk.Nam+"_tb;\n");
            if(rst.Nam!="") Filout("   reg "+rst.Nam+"_tb;\n");
            for (int i=0;i<infoI.size();i++){
                Filout("   reg"+(infoI[i].Ran[1]!=infoI[i].Ran[0]?" ["+to_string(infoI[i].Ran[1]-infoI[i].Ran[0])+"] ":" ")+infoI[i].Nam+"_tb;\n");
            }
            Filout("\n   //Output signals\n");
            for (int i=0;i<infoO.size();i++){
                Filout("   wire"+(infoI[i].Ran[1]!=infoI[i].Ran[0]?" ["+to_string(infoI[i].Ran[1]-infoI[i].Ran[0])+"] ":" ")+infoO[i].Nam+"_tb;\n");
            }

            //Module initialization and wiring
            Filout("\n   //TOP module initialization\n   "+mod+" UUT(");
            if(clk.Nam!="") Filout(" ."+clk.Nam+"("+clk.Nam+"_tb),");
            if(rst.Nam!="") Filout(" ."+rst.Nam+"("+rst.Nam+"_tb),");
            for (int i=0;i<infoI.size();i++)
                Filout(" ."+infoI[i].Nam+"("+infoI[i].Nam+"_tb),");
            Filout(" ."+infoO[0].Nam+"("+infoO[0].Nam+"_tb)");
            for (int i=1;i<infoO.size();i++)
                Filout(", ."+infoO[i].Nam+"("+infoO[i].Nam+"_tb)");
            
//////////////////////////Initializations///////////////////////////
            Filout(");\n\n");

            //Clock initialization (If existent)
            (clk.Nam!="")?Filout("   //Clock initialization\n   always forever #1 "+clk.Nam+"_tb=~"+clk.Nam+"_tb;\n\n"): Filout("");
            
            //Start of initial with fumpvars and fumpfile
            Filout("   initial\n   begin\n      $dumpfile(\""+mod+".vcd\");\n      $dumpvars(1,"+mod+"_tb);\n\n      //Variable initialization\n");

            //Initialization of input signals
            for (auto i:infoI) Filout("      "+ i.Nam +"_tb="+ng.NumCon(radix[0],i.val)+";\n");
            //Initialization of clock and reset signals
            (clk.Nam!="")?Filout("      "+ clk.Nam +"_tb="+ng.NumCon(radix[0],clk.val)+";\n"): Filout("");
            (rst.Nam!="")?Filout("      "+ rst.Nam +"_tb="+ng.NumCon(radix[0],rst.val)+";\n\n      //Reset inactive\n      #2\n      "+rst.Nam+"_tb=~"+rst.Nam+"_tb;\n"): Filout("");
//////////////////////////////Cycles/////////////////////////////////
            Filout("\n      //The program is made for "+to_string(cy)+" cycles\n");

            //Iterate over number of cycles
            for(int j=1;j<=cy;j++){
                Filout("      //Iteration: "+to_string(j)+"\n      #1\n");
                for(auto i:infoI){
                    //Change value of the signal
                    if(i.typ=="R") i.val=ng.RanGen(pow(2,i.Ran[2])-1);
                    else if(i.typ=="i") i.val=(i.val+i.step)%(2^i.Ran[2]);
                    else if(i.typ=="d") i.val=(i.val-i.step)%(2^i.Ran[2]);
                    //Write the signal
                    Filout("      "+i.Nam+"_tb = "+to_string(i.Ran[2])+ng.NumCon(radix[0],i.val)+";\n");
                }
                Filout("\n");
            }
            //Finish the blocks in the file
            Filout("      //Ending iteration\n      #1\n      $finish;\n   end\nendmodule\n");
            file.close();
            return;
        }

        //Function to print to terminal easy to read translator contents
        void Print(){
            cout << "\nmodule: " << mod << endl;
            cout << "|->Type: Input" << endl;
            for (Data i:infoI) i.Print();
            cout << "|->Type: Output" << endl;
            for (Data i:infoO) i.Print();
            cout << "|->Type: Clock" << endl;
            clk.Print();
            cout << "|->Type: Reset" << endl;
            rst.Print();
        }

        //Function to write the file and the terminal at the same time
        void Filout(string arg){
            cout << arg;
            file << arg;
        }
};