#include "read_design.hpp"
#include "write_tb.hpp"

using namespace std;

//This function removes single- and multiline comments
void regex_comments(string& input_file)
{
    string new_input_file;

    regex regex_comments("(\\/\\*([^*]|[\r\n]|(\\*+([^*\\/]|[\r\n])))*\\*+\\/)|(\\/\\/.*)");
    regex_replace(back_inserter(new_input_file), input_file.begin(), input_file.end(), regex_comments, "");
    input_file = new_input_file;

    //cout << "Lo que queda es\n\n" << input_file << endl;
}

//This function stores all parameters found in the Verilog design file for later use
void regex_parameters(const string& input_file, vector<string>&  param_vector)
{
    string parameters;
    smatch string_match;

    regex regex_parameter("parameter\\s*\\w*\\s*=\\s*\\d+\\s*(,\\s*\\w*\\s*=\\s*\\d*\\s*)*");   //RegEx used to obtain the parameters (per line)
    auto param_begin = sregex_iterator(input_file.begin(), input_file.end(), regex_parameter);
    auto param_end = sregex_iterator();
    for (sregex_iterator i = param_begin; i!= param_end; ++i)
    {
        smatch match = *i;
        parameters += match.str();
    }
    //cout << parameters << endl << endl;
    
    regex regex_individual_parameter("\\w*\\s*=\\s*\\d+");  //RegEx used to obtain each parameter
    while (regex_search(parameters, string_match, regex_individual_parameter))
    {
        param_vector.push_back(string_match.str());         //Store new parameter in param_vector
        parameters = string_match.suffix().str();
    }

    //for (int i=0; i< param_vector.size(); i++)
        //cout << param_vector[i] << endl;

    //cout << "El tamaño de param_vector es " << param_vector.size() << endl;
}

//This function obtains the Verilog design module's name and stores it
void regex_module(string& input_file, string& module_name)
{
    smatch string_match;

    regex regex_module_name("module\\s+(\\w+)");
    if (regex_search(input_file, string_match, regex_module_name))
    {
        module_name = string_match[1];
        input_file = string_match.suffix().str();
    }
    //cout << "\nNombre del modulo: " << module_name << endl << endl;
}

//This function obtains the input|output ports and store them
void regex_ports(string& input_file, Port_vector& p_vector)   
{
    smatch string_match;
    string ports_type, ports_bus, ports;

    //RegEx used to obtain the input|output|inout ports
    regex regex_port("(input|output|inout)\\s*(reg|wire|\\s*)\\s*(\\[.*:.*\\]\\s*|\\s+)\\s*(((\\,\\s*|\\s*)((?!input|output|inout)\\w*\\s*))*)");
    while (regex_search(input_file, string_match, regex_port))
    {
        ports_type = string_match[1];
        ports_bus = string_match[3];
        ports = string_match[4];
        input_file = string_match.suffix().str();

        //cout << "Lo que atrape fue: " << ports_type << " " << ports_bus << " " << ports << endl;
        
        regex regex_individual_port("(\\w+)");                      //Create a RegEx that obtains each port individually
        while (regex_search(ports, string_match, regex_individual_port))
        {
            Port new_Port(ports_type, ports_bus, string_match[1]);  //Create a new Port object
            p_vector.push_back(new_Port);                           //Store the Port object in the vector
            ports = string_match.suffix().str();
        }
    }
}

int main(int argc, char** argv)     //It receives an input argument, such as a .txt file, at execution time
{
    if (argc < 2)
        return(EXIT_FAILURE);       //Exit if no input argument was given

    ifstream in(argv[1]);           //Read the file into "in"
    if (!in)
        exit(EXIT_FAILURE);         //Exit if input file does not exist

    string verilog_str;             //Write all information inside string called 'verilog_str'
    if (in)
    {
      ostringstream ss;
      ss << in.rdbuf();
      verilog_str = ss.str();
    }

    //cout << "\n\nEl archivo verilog es:\n\n" << verilog_str << endl;

    //cout << "\n************************\n" << endl;

                                                    //https://blog.ostermiller.org/finding-comments-in-source-code-using-regular-expressions/
    regex_comments(verilog_str);                    //Call regex_comments in order to remove single- and multiline comments

    //cout << "\n************************\n" << endl;

    vector<string> param_vector;                    //Create a vector of parameters
    regex_parameters(verilog_str, param_vector);    //Call regex_parameters in order to store the Verilog design's parameters

    string module_name;
    regex_module(verilog_str, module_name);         //Call regex_module in order to obtain the module's name

    Port_vector p_vector;                           //Create a vector of Port objects to later pass to the regex_ports function
    regex_ports(verilog_str, p_vector);             //Execute regex_ports as long as there is still input|output|inout ports to collect

    //cout << "\n\nNo quedan mas busquedas\n\n" << endl;

    ofstream generated_testbench_file;
    generated_testbench_file.open(module_name + "_tb.sv");

    generated_testbench_file << tb_header(module_name, p_vector, param_vector);
    generated_testbench_file << tb_DUT(module_name, p_vector);
    generated_testbench_file << tb_clock_reset(p_vector);
    generated_testbench_file << tb_test_vectors(module_name, p_vector);

    generated_testbench_file.close();

    return 0;
 }