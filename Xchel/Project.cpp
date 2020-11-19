#include "Translator.cpp"

using namespace std;

int main()
{
    //Get Operating System and clear the terminal
    (getenv("windir")==NULL)?system("clear"):system("cls");
    //Create new translator object
    Translator Tr;
    //Translate verilog code
    Tr.Translate();
    //Print the information stored in the translator object
    Tr.Print();
    //Write the information in the testbench
    Tr.Write();
}