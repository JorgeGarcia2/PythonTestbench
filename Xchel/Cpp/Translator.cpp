#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <regex>

#include "NumGen.cpp"

using namespace std;

class Data{

    public:

    int Ran[2]={0,0};
    string Nam;
    string typ;
    vector<string> dim;
    string ini;
    int val;

    Data(string d1, string d2, int R[2], vector<string> v, string in, int va){
        typ=d1;
        Nam=d2;
        Ran[0]=R[0];
        Ran[1]=R[1];
        for(int i=0;i<v.size();i++) dim.push_back(v[i]);
        if (dim.size()==0) dim.push_back({0,0});
        ini=in;
        val=va;
    }

    Data(string d1){
        typ="module";
        Nam=d1;
    }

    void print(){
        cout << "|  |->Name: " << Nam << endl;
        cout << "|  |->Range: [" << to_string(Ran[0]) << ":" << to_string(Ran[1]) << "]" <<endl;
        cout << "|  |->Initial value: " << ini << endl;
        cout << "|  \\->Dimensions:";
        for (int i=0;i<dim.size();i++) cout << " [" << dim[i] << "]";
        cout << endl << "|" << endl;
    }
};

class Translator{

    vector<Data> infoI;
    vector<Data> infoO;
    Data mod=Data("");
    fstream file;

    public:

        void Translate(){
            string t;
            cout << "Do you want an automatic translation?[Y/n]\n";
            cin >> t;
            bool Auto=(t!="n"&&t!="N");

            string Line;
            regex e1("\\W*((output|input|module)\\s*(reg|wire|\\s*)\\s*(\\[\\d+:\\d+\\]\\s*|\\s+)\\s*(((,\\s*|\\s*)((?!input|output|inout)[_a-zA-Z]\\w*\\s*))*)((\\[\\d+:\\d+\\]\\s*)*|\\s+))");
            regex e2("[\\w:'`]+");
            while (1){
                cout << "What's the file name?\n";
                cin >> t;
                if (t.find(".sv")==string::npos)t+=".sv";
                file.open(t, ios::in);
                if (!file) {
                    cout << "Inexistent file!\n";
                    file.open("design.sv", ios::in);
                    if (!file) cout << "default file not found!\n";
                    else{
                        cout << "default file design.sv found, using it!\n";
                        break;
                    }
                }else{
                    cout << "file found!";
                    break;
                }
            }
            
            while (!file.eof()) {
                getline(file,Line);
                regex_iterator <string::iterator> it (Line.begin(),Line.end(),e1);
                regex_iterator <string::iterator> rend;

                regex_iterator <string::iterator> rtemp=it;
                
                while (it!=rend) {
                    string st=it->str();
                    regex_iterator <string::iterator> its (st.begin(),st.end(),e2);
                    st=its->str();
                    its++;
                    if(st=="module"){
                        mod=Data(its->str());
                    }else{
                        t=st;
                        vector<string> d;
                        vector<string> n;
                        int r[2]={0,0};
                        int f0=0;
                        int f1=0;
                        while(its!=rend){
                            st=its->str();
                            if(st.find(":")!=string::npos){
                                if(f1==0){
                                    stringstream ss;
                                    ss=stringstream(st);
                                    string s;
                                    getline(ss, s, ':');
                                    r[0]=stoi(s);
                                    getline(ss, s, ':');
                                    r[1]=stoi(s);
                                }else{
                                    d.push_back(st);
                                }
                                f1++;
                            }else{
                                if (st!="reg"&&st!="wire"){
                                    if(f1==0){
                                        f1++;
                                    }
                                    n.push_back(st);
                                    f0++;
                                }
                            }
                            ++its;
                        }
                        if (d.size()==0) d.push_back("0:0");
                        for (int i=0;i<n.size();i++){
                            if(t=="input"){
                                regex et1("\\w*[cC][lL]\\w*[kK]\\w*");
                                regex et2("\\w*[rR]\\w*[sS]\\w*[tT]\\w*");
                                string in;
                                in=(regex_search(n[i].begin(),n[i].end(),et1)&&r[0]==r[1])?"c":(regex_search(n[i].begin(),n[i].end(),et2)&&r[0]==r[1])?"r":"0";
                                if(!Auto){
                                    t=(in=="c")?"clock":(in=="r")?"reset":"complete";
                                    cout << "\nThe signal: " << n[i] << "Was found as a signal with value: " << t << ", is this correct?\n"
                                    "(Y for yes, c for clock, R for random, r for reset or a value for a complete signals initial value)\n";
                                    cin >> t;
                                    t=(t=="y"||t=="Y")?t:(t.find_first_not_of( "0123456789" ) == string::npos)?t:(t=="c"&&t=="r"&&t=="R")?t:in;
                                }
                                int v;
                                if (t.find_first_not_of( "0123456789" ) == string::npos) infoI.push_back(Data("input",n[i],r,d,in,stoi(t)));
                                else infoI.push_back(Data("input",n[i],r,d,in,-1));
                            }else infoO.push_back(Data(t,n[i],r,d,"",-1));
                        }
                    }
                    ++it;
                }
            }
            file.close();
            return;
        }

        void write(){

            cout << "The values will be complete unless the program finds a clock or reset signal (containing clk or rst)" << endl;

            NumGen ng;
            file.open(mod.Nam+"_testbench.sv", ios::out);

            cout << "How many cycles do you want?\n";
            int cy;
            cin >> cy;
            if(!cin||cy>1000) cy=10;
            cout << "\n\n Generation:\n\n";
//////////////////////////HEADER WRITING///////////////////////////
            Filout("//Testbench created automatically with a program written in c++ by:\n"
            "//\tMorales Hurtado David Xchel\n"
            "//\n//For the first project in the class of professor:\n"
            "//\t//Jorge Martínez Carbballido\n//\n"
            "//In the Silicon Verification Program\n\n"
            "//time scale\n"
            "`timescale 1ns/1ps\n\n"
            "//Main Testbench Starts here\n"
            "module "+mod.Nam+"_TB();\n\n");
            Filout("   //Input signals\n");
            for (int i=0;i<infoI.size();i++){
                Filout("   reg"+(infoI[i].Ran[1]!=infoI[i].Ran[0]?" ["+to_string(infoI[i].Ran[1]-infoI[i].Ran[0])+"] ":" ")+infoI[i].Nam+"_tb ");
                for(int j=0;j<infoI[i].dim.size();j++) if(infoI[i].dim[j]!="0:0") Filout("["+infoI[i].dim[j]+"]");
                Filout(";\n");
            }
            Filout("\n   //Output signals\n");
            for (int i=0;i<infoO.size();i++){
                Filout("   wire"+(infoI[i].Ran[1]!=infoI[i].Ran[0]?" ["+to_string(infoI[i].Ran[1]-infoI[i].Ran[0])+"] ":" ")+infoO[i].Nam+"_tb ");
                for(int j=0;j<infoO[i].dim.size();j++) if(infoO[i].dim[j]!="0:0") Filout("["+infoO[i].dim[j]+"]");
                Filout(";\n");
            }

            Filout("   //TOP module initialization\n   "+mod.Nam+" UUT(");
            for (int i=0;i<infoI.size();i++)
                Filout(" ."+infoI[i].Nam+"("+infoI[i].Nam+"_tb),");
            for (int i=0;i<infoO.size();i++)
                Filout(" ."+infoO[i].Nam+"("+infoO[i].Nam+"_tb),");
            
//////////////////////////Initializations///////////////////////////
            Filout(");\n\n   //Clock initialization\n");
            for(int clk=0;clk<infoI.size();clk++)
                infoI[clk].ini=="c"?Filout("   always forever #1 "+infoI[clk].Nam+"_tb=~"+infoI[clk].Nam+"_tb;\n"): Filout("");

            Filout("\n   initial\n   begin\n      $dumpfile(\""+mod.Nam+".vcd\");\n      $dumpvars(1,"+mod.Nam+"_tb);\n\n      //Variable initialization\n");

            stringstream ss;
            string s;
            for(int i=0;i<infoI.size();i++){
                int a=abs(infoI[i].Ran[1]-infoI[i].Ran[0])+1;
                infoI[i].ini=="r"?Filout("      "+infoI[i].Nam+"_tb=1;\n"):infoI[i].val==-1?Filout("      "+infoI[i].Nam+"_tb=0;\n"):Filout("      "+infoI[i].Nam+"_tb="+infoI[i].ini+";\n");
            }
            Filout("\n      //Reset inactive\n      #2\n");
            for(int rst=0;rst<infoI.size();rst++)
                infoI[rst].ini=="r"?Filout("      "+infoI[rst].Nam+"_tb=0;\n"): Filout("");
//////////////////////////////Cycles/////////////////////////////////
            Filout("\n      //The program is made for "+to_string(cy)+" cycles\n");
            for(int j=1;j<cy;j++){
                Filout("      //Iteration: "+to_string(j)+"\n      #1\n");
                for(int i=0;i<infoI.size();i++){
                    int a=abs(infoI[i].Ran[1]-infoI[i].Ran[0])+1;
                    ng.Ran_Gen(pow(2,a)-1);
                    if (infoI[i].ini!="r"&&infoI[i].ini!="c"){
                        infoI[i].ini=="R"?Filout("      "+infoI[i].Nam+"_tb="+to_string(a)+ng.NumCon('b')+";\n"):Filout("      "+infoI[i].Nam+"_tb="+to_string(infoI[i].val)+";\n");
                    }
                }
                Filout("\n");
            }
            Filout("      //Ending iteration: "+to_string(cy)+"\n      #1\n      $finish;\n   end\nendmodule\n");
            file.close();
            return;
        }

        void print(){
            cout << "\nmodule: " << mod.Nam << endl;
            cout << "|->Type: Input" << endl;
            for (int i=0;i<infoI.size();i++) infoI[i].print();
            cout << "|->Type: Out" << endl;
            for (int i=0;i<infoO.size();i++) infoO[i].print();
        }

        void Filout(string arg){
            cout << arg;
            file << arg;
        }
};