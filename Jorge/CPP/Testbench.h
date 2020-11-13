#include "Port.h"

class Testbench {       // The class
    public:    
    string designCode;              //texto del design
    string  name_Module;
    port_map inputs_module;
    port_map outputs_module;
    vector<Port> output_M;
    vector<Input> input_M;
    Input clk, rst;
    int time = 10;
    char radix = 'd';

    Testbench(){
        cout << "\nSe ah creado un objeto tipo TestBench";
    }

    void read_file(void)
    {
        fstream  textFile;
        string tempText;
        string path = "design.sv";

        textFile.exceptions (ifstream::failbit | ifstream::badbit );
        try
        {
            textFile.open(path, ifstream::in);
            while (!textFile.eof()){
                getline(textFile, tempText);
                designCode += tempText;
                //cout << tempText << endl;
            } 
            textFile.close();  
        }
        catch (ifstream::failure e) 
        {
            cout << "An exception occurred, file was not found\n";
        }
    }

    void getInitVal(void){
        regex re_Clk("\\w*[cC][lL]\\w*[kK]\\w*");
        regex re_Rst("\\w*[rR]\\w*[sS]\\w*[tT]\\w*");
        smatch m;
        char resp;
        int intResp;
       for (auto const& pair: inputs_module) {
            if(pair.second[0] == pair.second[1]){
                if(regex_search (pair.first,m,re_Clk)){
                cout << "\nInput " <<pair.first<< " has been detected as a possible clock signal."
                        << "Is this correct? (Y/N)\n";
                    cin >> resp;
                    if(resp == 'Y')
                        inputs_module[pair.first][2] = -2; //It is a clock
                }
                else if(regex_search (pair.first,m,re_Rst)){
                    cout << "\nInput " <<pair.first<< " has been detected as a possible reset signal."
                        << "Is this correct? (Y/N)\n";
                    cin >> resp;
                    if(resp == 'Y')
                        inputs_module[pair.first][2] = -3; //It is a reset
                }
           }
        }
        cout << "\nEnter the initial value and the steps for the entries listed below.\n"
             <<"(The default values ​​will be a random numbering and steps of 1).\n";
        for (auto const& pair: inputs_module) {
            if(pair.second[0] != pair.second[1]){
                cout  << "\nPort " << pair.first << "["<< inputs_module[pair.first][0] << ":" 
                << inputs_module[pair.first][1] << "] :" << endl; 
                cout << "\tInitial value: ";
                cin >> inputs_module[pair.first][2];
                cout << "\tStep: ";
                cin >> inputs_module[pair.first][3];
                cout << endl;
            }
        }
    }

    void getData(void){
        vector<string> tmpVec;             //vector para separar los nombres de puertos "clk|rst|btn"
        infoPort my_infoPort;           //Vector para los rangos del bus (rangoSup,rangoInf)
        string match_text, temp_text;   //Strings auxiliares
        int size;
        smatch m;                       //Resultados del match de RegEx 

        //Buscar el nombre del modulo
        regex re_TB("\\W*((module|input|output|inout)\\s*(reg|\\s*)\\s*(\\[\\d+:\\d+\\]\\s*|\\s+)\\s*(((,\\s*|\\s*)((?!input|output|inout)[_a-zA-Z]\\w*))*))");
        while(regex_search (designCode,m,re_TB)){//Mientra encuentre mas coinsidencias con input...

            if(m[2] == "module"){
                name_Module = m[5];
                cout << endl << "Encontre Modulo"<< endl;
            }
            else{
                match_text = m[5];
                //separa los nombre de puertos que esten juntos y los guarda en el vector tmp
                while(splitText(match_text, temp_text))
                    tmpVec.push_back(temp_text);
                //Obtiene los enteros del rango del bus, si no tiene (1 bit) coloca ambos en 0
                match_text = m[4];
                my_infoPort[0] = splitText(match_text, temp_text)?stoi (temp_text,nullptr,0):0;
                my_infoPort[1] = splitText(match_text, temp_text)?stoi (temp_text,nullptr,0):0;
                if(m[2] == "input"){
                    for(int i = 0; i < tmpVec.size(); i++) {
                    inputs_module.insert(pair <string, infoPort> (tmpVec.at(i), my_infoPort));
                    }   
                }
                else{
                    for(int i = 0; i < tmpVec.size(); i++) {
                    outputs_module.insert(pair <string, infoPort> (tmpVec.at(i), my_infoPort));
                    } 
                }
            }
            tmpVec.clear();
            designCode = m.suffix().str();
        }
        cout << "Module name: " << name_Module << endl;

        for (auto const& pair: inputs_module) {
            cout << "Inputs: " <<"name: "<< pair.first << "| range: " << pair.second[0] << "|" << pair.second[1] << endl;
        }

        for (auto const& pair: outputs_module) {
            cout << "Outputs: " <<"name: "<< pair.first << "| range: " << pair.second[0] << "|" << pair.second[1] << endl;
        }
        
    }
    void fillVectors(void){
        for (auto const& pair: inputs_module) {
            if(pair.second[2] == -2 ) //-1=Random,-2 = clock, -3 == rst
                clk = Input(pair.first,pair.second);
            else if(pair.second[2] == -3 ) //-1=Random,-2 = clock, -3 == rst
                rst = Input(pair.first,pair.second);
            else 
                input_M.push_back(Input(pair.first,pair.second));
        }
        for (auto const& pair: outputs_module) {
            output_M.push_back(Port(pair.first,pair.second));
        }
    }

    void writeHead(void){
        string text_tb; //string que se escribira en el archivo testbench
        fstream  tbFile;

        text_tb = "`timescale 1ns / 1ps\n"
                "module " + name_Module + "_TB;\n";
        if(clk.namePort != "?")
            text_tb += "reg " + clk.namePort + "_TB;\n";
        if(rst.namePort != "?")
            text_tb += "reg " + rst.namePort + "_TB;\n";
        //Entradas
        for(int i = 0; i < input_M.size(); i++) {
            if(input_M[i].rangePort == 0)
                text_tb += "reg " + input_M[i].namePort + "_TB;\n";
            else{
                if(input_M[i].downTo)
                    text_tb += "reg [" + to_string(input_M[i].rangePort) + ":0] " + input_M[i].namePort + "_TB;\n"; 
                else
                    text_tb += "reg [0:" + to_string(input_M[i].rangePort) +"] " + input_M[i].namePort + "_TB;\n"; 
            }
        }
        //Salidas
        for(int i = 0; i < output_M.size(); i++) {
            if(output_M[i].rangePort == 0)
                text_tb += "wire " + output_M[i].namePort + "_TB;\n";
            else{
                if(output_M[i].downTo)
                    text_tb += "wire [" + to_string(output_M[i].rangePort) + ":0] " + output_M[i].namePort + "_TB;\n"; 
                else
                    text_tb += "wire [0:" + to_string(output_M[i].rangePort) +"] " + output_M[i].namePort + "_TB;\n"; 
            }
        }
        text_tb += name_Module + " UUT(";
        if(clk.namePort != "?")
            text_tb += "." + clk.namePort + "("+ clk.namePort + "_TB), ";
        if(rst.namePort != "?")
            text_tb += "." + rst.namePort + "("+ rst.namePort + "_TB), ";
        for(int i = 0; i < input_M.size(); i++) {
            text_tb += "." + input_M[i].namePort + "("+ input_M[i].namePort + "_TB), ";
	    }   
        for(int i = 0; i < output_M.size(); i++) {
            text_tb += "." + output_M[i].namePort + "("+ output_M[i].namePort + "_TB), ";
	    }
        text_tb.pop_back();
        text_tb.pop_back();
        text_tb += ");\n";

        text_tb += "initial\n"
                "\tbegin\n"
                "\t//Initial condition\n";
        if(clk.namePort != "?")
            text_tb += "\t" + clk.printValue(radix);
        if(rst.namePort != "?")
            text_tb += "\t" + rst.printValue(radix);
        for(int i = 0; i < input_M.size(); i++) {
            cout << i;
            text_tb += "\t" + input_M[i].printValue(radix);
            input_M[i].value += input_M[i].step;
        }
        for(int j = 0; j < time; j++){
            text_tb +="\t\t#1\t//Iteration " + to_string(j) +"\n";
            for(int i = 0; i < input_M.size(); i++) {
                cout << i;
                text_tb +="\t\t";
                text_tb += input_M[i].printValue(radix);
                input_M[i].value += input_M[i].step;
            }
        }
        text_tb += "\tend\n"
                "endmodulen\n";

        tbFile.open(name_Module+"_testbench.sv", ofstream::out);
        tbFile << text_tb;
        tbFile.close();

    }
};