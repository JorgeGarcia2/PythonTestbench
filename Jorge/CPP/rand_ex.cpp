// Random numbers
#include <cstdlib>
#include <iostream>
#include <ctime>
#include <string>
#include <experimental/random>
using namespace std;

 
int main() 
{
    srand(time(nullptr)); // use current time as seed for random generator
    int random_variable = experimental::randint(0,255);
    cout << "Random value on [0 " << RAND_MAX << "]: " 
              << random_variable << '\n';

    string test_vector;
    test_vector += "#1 addr_var = 3d'" + to_string(random_variable);
}