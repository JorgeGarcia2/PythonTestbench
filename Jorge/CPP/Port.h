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
    Port(){
        cout << "\nSe ah creado un objeto tipo Port Vacio";
    }

    Port(string  name, infoPort defPort){
        namePort = name;
        rangePort = defPort[0] - defPort[1];
        downTo = (rangePort >= 0);
        rangePort = int(abs(rangePort));
        Port::PrintPort();
    }

    void PrintPort(void){
        cout << "\nPort name: " << namePort << "\nBus size: " << rangePort << endl;
    }
};

class Input : public Port{
    public:
    int value;
    int step;
    
    Input(){
        cout << "\nSe ah creado un objeto tipo Input Vacio";
    }
    Input(string  name, infoPort defPort):Port(name, defPort){
        value = defPort[2];
        step = defPort[3];
        if(value == -1)//Random
            value = experimental::randint(0,int(exp2 (rangePort)) - 1);
        Input::PrintPort();
    }
    void PrintPort(void){

        cout << "Value: " << value << "\nStep: " << step << endl;
    }

    string printValue(char radix){
        string strnum = "";
        char* buffer;
        strnum += namePort + "_TB = " + to_string(rangePort + 1);
        switch (radix)
        {
            case 'b':
                itoa (value,buffer,2);
                strnum += "'b";
                if(downTo){
                    for (int i = 0; i < strlen(buffer); i++) {
                        strnum += buffer[i];
                    }
                }
                else{
                    for (int i = strlen(buffer) -1; i >= 0; i--) {
                        strnum += buffer[i];
                    }
                }
                break;
            case 'h':
                itoa (value,buffer,16);
                strnum += "h";
                for (int i = 0; i < strlen(buffer); i++) {
                    strnum += buffer[i];
                }
                break;
            default:
                strnum += "'d" + to_string(value);
                break;
        }
        strnum += ";\n";
        return strnum;
    }
};


bool splitText(string &text_to_split, string &text_result) {
    bool result = false;
    regex split("\\W*(\\w+)\\W*");
    smatch m;
    if(regex_search(text_to_split,m,split))
    {   text_result = m[1];
        //cout << "text: "<< text_result <<"|" <<m[1]<<endl;
        text_to_split = m.suffix().str();
        result = true;
    }
    
    return result;
}