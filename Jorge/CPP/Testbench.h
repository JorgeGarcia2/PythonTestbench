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
    string radix = "dec";

    Testbench(){;}

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

    void getData(void){
        vector<string> tmpVec;             //vector para separar los nombres de puertos "clk|rst|btn"
        infoPort my_infoPort;           //Vector para los rangos del bus (rangoSup,rangoInf)
        string match_text, temp_text;   //Strings auxiliares
        int size;
        char resp;
        int intResp; 
        smatch m;   

        //Buscar el nombre del modulo
        regex re_TB("\\W*((module|input|output|inout)\\s*(reg|\\s*)\\s*(\\[\\d+:\\d+\\]\\s*|\\s+)\\s*(((,\\s*|\\s*)((?!input|output|inout)[_a-zA-Z]\\w*))*))");
        regex re_Clk("\\w*[cC][lL]\\w*[kK]\\w*");
        regex re_Rst("\\w*[rR]\\w*[sS]\\w*[tT]\\w*");
        
        while(regex_search (designCode,m,re_TB)){//Mientra encuentre mas coinsidencias con input...

            if(m[2] == "module"){
                name_Module = m[5];
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
                my_infoPort[2] = 0;
                my_infoPort[3] = 1;
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

        /*cout << "Module name: " << name_Module << endl;

        for (auto const& pair: inputs_module) {
            cout << "Inputs: " <<"name: "<< pair.first << "| range: " << pair.second[0] << "|" << pair.second[1] << endl;
        }

        for (auto const& pair: outputs_module) {
            cout << "Outputs: " <<"name: "<< pair.first << "| range: " << pair.second[0] << "|" << pair.second[1] << endl;
        }*/
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
                cin >> temp_text;
                inputs_module[pair.first][2] = (isdigit(temp_text[0]))?stoi(temp_text): -1;
                cout << "\tStep: ";
                cin >> inputs_module[pair.first][3];
                cout << endl;
            }
        }

        //Fill Vectors
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

    void writeTB(void){
        string text_tb; //string que se escribira en el archivo testbench
        fstream  tbFile;

        text_tb = "`timescale 1ns / 1ps\n"
                "module " + name_Module + "_TB;\n";
        if(clk.namePort != "?")
            text_tb += "\treg " + clk.namePort + "_TB;\n";
        if(rst.namePort != "?")
            text_tb += "\treg " + rst.namePort + "_TB;\n";
        //Entradas
        for(int i = 0; i < input_M.size(); i++) {
            if(input_M[i].rangePort == 0)
                text_tb += "\treg " + input_M[i].namePort + "_TB;\n";
            else{
                if(input_M[i].downTo)
                    text_tb += "\treg [" + to_string(input_M[i].rangePort) + ":0] " + input_M[i].namePort + "_TB;\n"; 
                else
                    text_tb += "\treg [0:" + to_string(input_M[i].rangePort) +"] " + input_M[i].namePort + "_TB;\n"; 
            }
        }
        //Salidas
        for(int i = 0; i < output_M.size(); i++) {
            if(output_M[i].rangePort == 0)
                text_tb += "\twire " + output_M[i].namePort + "_TB;\n";
            else{
                if(output_M[i].downTo)
                    text_tb += "\twire [" + to_string(output_M[i].rangePort) + ":0] " + output_M[i].namePort + "_TB;\n"; 
                else
                    text_tb += "\twire [0:" + to_string(output_M[i].rangePort) +"] " + output_M[i].namePort + "_TB;\n"; 
            }
        }
        text_tb += "\n\t" + name_Module + " UUT(";
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

        if(clk.namePort != "?")
            text_tb += "\n\talways forever #1 " + clk.namePort + "= ~"+ clk.namePort + ";";

        text_tb += "\n\n\tinitial begin\n"
                "\t\t//Initial condition\n";
        if(clk.namePort != "?")
            text_tb += "\t\t" + clk.namePort + " = 0\n";
        if(rst.namePort != "?")
            text_tb += "\t\t" + rst.namePort + " = 0\n";
        for(int i = 0; i < input_M.size(); i++) {
            text_tb += "\t\t" + input_M[i].printValue(radix);
            input_M[i].value += input_M[i].step;
        }
        for(int j = 0; j < time; j++){
            text_tb +="\n\t\t#1\t//Iteration " + to_string(j+1) +"\n";
            for(int i = 0; i < input_M.size(); i++) {
                text_tb += "\t\t" + input_M[i].printValue(radix);
                input_M[i].nextValue();
            }
        }
        text_tb += "\tend\n"
                "\n\tinitial begin\n"
                "\t\t$dumpvars(1, "+name_Module+"_TB);\n"
                "\t\t$dumpfile(\""+name_Module+".vcd\");\n"
                "\tend\n"
                "endmodule";
        tbFile.open(name_Module+"_testbench.sv", ofstream::out);
        tbFile << text_tb;
        tbFile.close();
    }
};