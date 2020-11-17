#include <iostream>
#include <regex>
#include <string>
#include <fstream>
#include <map>
#include <vector>
#include <sstream>
#include <array>
#include <cstdlib>
#include <ctime>
#include <experimental/random>
#include <cmath>

using namespace std;

typedef array <int, 4> infoPort;
typedef map<string,infoPort> port_map;

class Port{
    public:
    string namePort = "?";
    int rangePort;
    bool downTo;

    //constructor
    Port(){;}
        //cout << "\nSe ah creado un objeto tipo Port Vacio";
        

    Port(string  name, infoPort defPort){
        namePort = name;
        rangePort = defPort[0] - defPort[1];
        downTo = (rangePort >= 0);
        rangePort = int(abs(rangePort));
        //Port::PrintPort();
    }
};

class Input : public Port{
    public:
    int value;
    int step;
    
    Input(){;}
    Input(string  name, infoPort defPort):Port(name, defPort){
        value = defPort[2];
        step = defPort[3];
        if(value == -1)//Random
            value = experimental::randint(0,int(exp2 (rangePort)) - 1);
    }

    string printValue(string radix){
        string strnum = "";
        int temp = value;
        //char* buffer;
        if (radix == "bin"){
            while(temp>0){
                strnum+=to_string(temp%2);
                temp=temp/2;
            }
            if(downTo){
                strnum+=(strnum=="")?"0b'":"b'";
                reverse(strnum.begin(),strnum.end());
            }
            else{
                while(strnum.length()<(rangePort+1))
                    strnum.push_back('0');
                strnum.insert(0, (strnum=="")?"'b0":"'b");
            }
        }
        else if (radix == "hex"){
            while(temp>0){
                (temp%16<10)?strnum+=to_string(temp%16) : strnum+=char(temp%16+87);
                temp=temp/16;
            }
            strnum+=(strnum=="")?"0h'":"h'";
            reverse(strnum.begin(),strnum.end());
        }
        else
            strnum += "'d" + to_string(value);
        strnum += ";\n";
        return namePort + "_TB = " + to_string(rangePort + 1) + strnum;
    }

    void nextValue(void){
        if((value + step) < exp2 (rangePort + 1)){
            value += step;
        }
        else{
            value = ((value + step) - exp2 (rangePort + 1));
        }
    }
};