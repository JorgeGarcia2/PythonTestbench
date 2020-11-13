#include "Testbench.h"
using namespace std;
int main() {
    srand(time(nullptr)); // use current time as seed for random generator
    cout << "Hola";
    Testbench my_TB;

    my_TB.read_file();
    cout <<endl << my_TB.designCode;
    my_TB.getData();
    my_TB.getInitVal();
    my_TB.fillVectors();
    
    my_TB.writeHead();
} 