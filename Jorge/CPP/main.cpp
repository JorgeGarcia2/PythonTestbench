#include "Testbench.h"
using namespace std;
int main() {
    int timeUsr;
    string radixUsr;
    fstream  textFile;
    string tempText;
    string path = "design.sv";
    Testbench my_TB;

    srand(time(nullptr)); // use current time as seed for random generator

    while (true){
        cout << "What's the file name?\n";
        cin >> path;
        path+=(path.find(".sv")==string::npos)?".sv":"";
        textFile.open(path, ios::in);
        if (!textFile) {
            cout << "Inexistent file!\n";
            textFile.open("design.sv", ios::in);
            if (!textFile){
                cout << "Default file not found. Do you want to try again?\n";
                cin >> tempText;
                if(tempText != "Y" && tempText != "y"){
                    cout << "Exiting the program";
                    return 1;
                }
            }
            else{
                cout << "Default file design.sv found, using it!\n";
                break;
            }
        }else{
            cout << "file found!";
            break;
        }
    }
    while (!textFile.eof()){
        getline(textFile, tempText);
       my_TB.designCode += tempText;
    } 
    textFile.close();  

    my_TB.getData();
    
    cout << "How many time intervals do you want? ";
    cin >> timeUsr;
    my_TB.time = timeUsr;
    cout << "Choose the test vectors radix ('bin', 'dec' or 'hex') ";
    cin >> radixUsr;
    my_TB.radix = radixUsr;

    my_TB.writeTB();
    return 0;
} 