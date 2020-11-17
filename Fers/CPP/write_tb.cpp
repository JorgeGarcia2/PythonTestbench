#include "read_design.hpp"
#include "write_tb.hpp"

using namespace std;

string tb_header(const string& module_name, Port_vector& p_vector, const vector<string>& param_vector)
{
    string header_string = "";
    string input_string = "";
    string output_string = "";

    header_string += "`timescale 1ns/1ps\n\nmodule " + module_name + "_TB;\n\n";

    if (param_vector.size() > 0)                            //Check if there are any parameters and declare them if so
    {
        for (unsigned int i=0; i < param_vector.size(); i++)
            header_string += "parameter " + param_vector[i] + ";\n";
        header_string += "\n";                              //Add a newline after last parameter
    }

    for (unsigned int i=0; i < p_vector.size(); i++)        //Iterate through the Port vector
        if (p_vector[i].get_type() == "input")
            input_string += "reg " + p_vector[i].get_bus() + p_vector[i].get_name() + "_tb;\n";
        else
            output_string += "wire " + p_vector[i].get_bus() + p_vector[i].get_name() + "_tb;\n";
    
    header_string += input_string + output_string + "\n";   //Declare inputs first, then outputs

    //cout << header_string << endl;      //Just for checking purposes
    return header_string;
}

string tb_DUT(const string& module_name, Port_vector& p_vector)
{
    string DUT_string = "";
    string input_string = "";
    string output_string = "";

    DUT_string += module_name + " DUT(";

    for (unsigned int i=0; i < p_vector.size(); i++)    //Iterate through the Port vector
        if (p_vector[i].get_type() == "input")
            input_string += "." + p_vector[i].get_name() + "(" + p_vector[i].get_name() + "_tb), ";
        else
            output_string += "." + p_vector[i].get_name() + "(" + p_vector[i].get_name() + "_tb), ";

    DUT_string += input_string + output_string;         //Match inputs first, then outputs
    
    DUT_string.erase(DUT_string.length() - 2);          //Delete last two chars from string: the comma and the blank space
    DUT_string += ");\n\n";
    
    //cout << DUT_string << endl;         //Just for checking purposes
    return DUT_string;
}

string tb_clock_reset(Port_vector& p_vector)
{
    smatch string_match;
    string port_name;
    string clock_reset_string = "";
    string half_clock_period = "";
    string reset_duration = "";
    char clock_confirmation = ' ';
    char reset_confirmation = ' ';
    char reset_logic;

    regex regex_clock("\\w*[cC][lL]\\w*[kK]\\w*");      //RegEx used to try to guess the clock
    regex regex_reset("\\w*[rR]\\w*[sS]\\w*[tT]\\w*");  //RegEx used to try to guess the reset

    for (unsigned int i=0; i < p_vector.size(); i++)    //Iterate through the Port vector
    {
        if (p_vector[i].get_type() == "input" && p_vector[i].get_bus() == " ")  //Just search for 1-bit input type
        {
            port_name = p_vector[i].get_name();         //Store the 'name' of the object into 'port_name' because regex_search complains otherwise
            if (regex_search(port_name, string_match, regex_clock) && clock_confirmation != 'y')        //Try to detect clock, if not already found
            {
                cout << "Possible clock detected: " << port_name << ", is it correct? [y/n]" << endl;
                cin >> clock_confirmation;
                if (clock_confirmation == 'y')
                {
                    p_vector[i].set_clock();
                    cout << "What is the Half Clock Period?" << endl;
                    cin >> half_clock_period;
                    clock_reset_string += "//Clock Generator\ninitial " + port_name + "_tb = 1;\n"
                                        + "always #" + half_clock_period + " " + port_name
                                        + "_tb = ~" + port_name + "_tb;\n\n";
                }
            }
            else if (regex_search(port_name, string_match, regex_reset) && reset_confirmation != 'y')   //Try to detect reset, if not already found
            {
                cout << "Possible reset detected: " << port_name << ", is it correct? [y/n]" << endl;
                cin >> reset_confirmation;
                if (reset_confirmation == 'y')
                {
                    p_vector[i].set_reset();
                    cout << "Is the reset signal active-HIGH? [y/n]" << endl;       
                    cin >> reset_logic;
                    if (reset_logic == 'y')
                        reset_logic = '1';
                    else
                        reset_logic = '0';
                    cout << "How long is the reset signal in units of time?" << endl;
                    cin >> reset_duration;
                    clock_reset_string += "//Reset signal\ninitial\n  begin\n    "
                                        + port_name + "_tb = " + reset_logic + ";\n"
                                        + "    #" + reset_duration + "\n    " + port_name + "_tb = ~"
                                        + port_name + "_tb;\n  end\n\n";
                }
            }
        }
    }
    
    //cout << clock_reset_string << endl;       //Just for checking purposes
    return clock_reset_string;
}

string tb_test_vectors(const string& module_name, Port_vector& p_vector)
{
    string test_v_string = "";
    string bus_initialization_string = "";
    string bit_initialization_string = "";
    string for_loop_string = "";
    string combinational_string = "";
    char reset_logic;

    unsigned int stimuli;
    cout << "How many iterations/stimuli do you want? ";
    cin >> stimuli;

    test_v_string += "initial\n  begin\n    $dumpfile(\"" + module_name + ".vcd\");\n"
                   + "    $dumpvars(1, " + module_name + "_TB);\n\n"
                   + "    #" + to_string((stimuli + 1) * 2) + "\n    $finish;\n  end\n\n";

    for (unsigned int i=0; i < p_vector.size(); i++)    //Iterate through the Port vector
        //For the inputs with bus
        if (p_vector[i].get_type() == "input" && p_vector[i].get_bus() != " ")
            bus_initialization_string += "    " + p_vector[i].get_name() + "_tb = 0;\n";
        //For the 1-bit inputs
        else if (p_vector[i].get_type() == "input" && p_vector[i].get_bus() == " " && p_vector[i].get_clock() == false && p_vector[i].get_reset() == false)
            bit_initialization_string += "    " + p_vector[i].get_name() + "_tb = 0;\n";

    for_loop_string += "    //Providing stimuli for " + to_string(stimuli) + " time(s)\n"
                     + "    for(integer i = 0; i < " + to_string(stimuli) + "; i = i + 1)\n"
                     + "      begin\n        #2\n";

    char user_response = '-';                           //Initial character stored as indicator
    unsigned int combinational_iterator = 0;
    unsigned int combinational_delay = 0;

    for (unsigned int i=0; i < p_vector.size(); i++)    //Iterate through the Port vector
    {
        if (p_vector[i].get_type() == "input" && p_vector[i].get_bus() != " ") //For the inputs with bus
        {
            cout << "For signal: " << p_vector[i].get_name() << ", what should its value be?" << endl
                 << "1 --> random value (default)\n2 --> increase\n3 --> decrease\n" << endl;
            cin >> user_response;
            switch (user_response)
            {
                default:
                case '1':
                    for_loop_string += "        " + p_vector[i].get_name() + "_tb = $urandom();\n";
                    break;

                case '2':
                    for_loop_string += "        " + p_vector[i].get_name() + "_tb = i;\n";
                    break;
                
                case '3':
                    for_loop_string += "        " + p_vector[i].get_name() + "_tb = " + to_string(stimuli) + " - (1 + i);\n";
                    break;
            }
        }
        //For the 1-bit inputs (except clock and reset signals)
        else if (p_vector[i].get_type() == "input" && p_vector[i].get_bus() == " " && p_vector[i].get_clock() == false && p_vector[i].get_reset() == false)
        {
            combinational_delay = pow(2, combinational_iterator);
            combinational_string += "always #" + to_string(combinational_delay) + " " + p_vector[i].get_name()
                                  + "_tb = ~" + p_vector[i].get_name() + "_tb;\n";
            combinational_iterator++;
        } 
    }

    for_loop_string += "      end\n  end\n";    //Close the for loop and the bus input variables block

    if (bus_initialization_string != "")        //If any bus input variable was initialized
        test_v_string += "initial\n  begin\n    //Initialization of bus input variables\n"
                       + bus_initialization_string + "\n" + for_loop_string;

    if (bit_initialization_string != "")        //If any 1-bit input variable was initialized
        test_v_string += "\ninitial\n  begin\n    //Initialization of 1-bit input variables\n"
                       + bit_initialization_string + "  end\n\n//Combinational stimuli\n"
                       + combinational_string + "  \n\n";

    test_v_string += "\nendmodule";

    //cout << test_v_string << endl;      //Just for checking purposes
    return test_v_string;
}

// parámetro que pasa el usuario --> máximo de vectores para un bus (p/e max ~16 vectores de prueba)