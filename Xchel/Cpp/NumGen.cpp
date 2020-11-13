#include <algorithm>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <experimental/random>

using namespace std;

class NumGen{

    int rn=0;

    public:

        NumGen(){
            srand(time(nullptr));
        }

        void Ran_Gen(int val){
            rn = experimental::randint(0,val);
            return;
        }

        string NumCon(char c){
            string st="";
            int temp=rn;
            switch (c) {
            case 'b':
                while(temp>0){
                    st+=to_string(temp%2);
                    temp=temp/2;
                }
                st==""?st+="0b'":st+="b'";
                reverse(st.begin(),st.end());
                break;
            case 'o':
                while(temp>0){
                    st+=to_string(temp%8);
                    temp=temp/8;
                }
                st==""?st+="0o'":st+="o'";
                reverse(st.begin(),st.end());
                break;
            case 'h':
                while(temp>0){
                    temp%16<10?st+=to_string(temp%16):st+=char(temp%16+87);
                    temp=temp/16;
                }
                st==""?st+="0h'":st+="h'";
                reverse(st.begin(),st.end());
                break;
            default:
                st="'d"+st;
            }
            return st;
        }
};