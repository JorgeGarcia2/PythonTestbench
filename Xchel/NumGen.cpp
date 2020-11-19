#include <algorithm>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <experimental/random>

using namespace std;

class NumGen{

    public:

        //Constructor with seed initialization
        NumGen(){
            srand(time(nullptr));
        }

        //Function to get a random value
        int RanGen(int val){
            int rn = experimental::randint(0,val);
            return rn;
        }

        //Function to get a converted value to another base (Binary, Octal or Hexadecimal)
        string NumCon(char c,int x){
            //Initialization of variables
            string st="";
            int temp=x;

            //Checks base to convert
            switch (c) {
            case 'b':
                //Get the remainders dividing by 2 and creating the reversed number
                while(temp>0){
                    st+=to_string(temp%2);
                    temp=temp/2;
                }
                //If the string is empty, append a "0"
                st==""?st+="0b'":st+="b'";
                //Reverse the string to get number
                reverse(st.begin(),st.end());
                break;
            case 'o':
                //Get the remainders dividing by 8 and creating the reversed number
                while(temp>0){
                    st+=to_string(temp%8);
                    temp=temp/8;
                }
                //If the string is empty, append a "0"
                st==""?st+="0o'":st+="o'";
                //Reverse the string to get number
                reverse(st.begin(),st.end());
                break;
            case 'h':
                //Get the remainders dividing by 16 and creating the reversed number
                while(temp>0){
                    //Check if remainder is bigger than 10, if so use char values to get value (a,b,c,d,e,f)
                    temp%16<10?st+=to_string(temp%16):st+=char(temp%16+87);
                    temp=temp/16;
                }
                //If the string is empty, append a "0"
                st==""?st+="0h'":st+="h'";
                //Reverse the string to get number
                reverse(st.begin(),st.end());
                break;
            default:
                //If decimal or others, append a "d'""
                st="'d"+to_string(temp);
            }
            return st;//Returns string containing base representation of number
        }
};