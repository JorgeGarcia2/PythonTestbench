#include <iostream>
#include <fstream>
#include <regex>
#include <iterator>
#include <string>
#include <vector>

using namespace std;

class Port
{
    public:
        //Constructor
        Port(string type, string bus, string name)
        {
            port_type = type;
            port_bus = bus;
            port_name = name;
            //cout << "New Port object --> " << port_type << " " << port_bus << " " << port_name << endl;
        }

        //Accessor methods
        string get_type() {return port_type;}
        string get_bus() {return port_bus;}
        string get_name() {return port_name;}
        bool get_clock() {return clock;}
        bool get_reset() {return reset;}

        //Mutator methods
        void set_clock() {clock = true;}
        void set_reset() {reset = true;}
    
    private:
        //Attributes
        string port_type, port_bus, port_name;
        bool clock = false;
        bool reset = false;
};

typedef vector<Port> Port_vector;

void regex_comments(string& input_file);
void regex_parameters(const string& input_file, vector<string>& param_vector);
void regex_module(string& input_file);
void regex_ports(string& input_file, Port_vector& p_vector);