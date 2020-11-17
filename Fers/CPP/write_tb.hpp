#include <iostream>
#include <fstream>
#include <regex>
#include <string>
#include <cmath>

using namespace std;

string tb_header(const string& module_name, Port_vector& p_vector, const vector<string>& param_vector);
string tb_DUT(const string& module_name, Port_vector& p_vector);
string tb_clock_reset(Port_vector& p_vector);
string tb_test_vectors(const string& module_name, Port_vector& p_vector);