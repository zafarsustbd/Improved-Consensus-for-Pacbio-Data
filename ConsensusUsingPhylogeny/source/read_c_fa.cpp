#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <stdio.h>
#include <string>

using namespace std;

int main(int argc, char* argv[])
{
    freopen(argv[1], "r", stdin);
    freopen(argv[2], "w", stdout);

    int i, flag=false;
    string str;
    int idx = 0;
    while(getline(cin, str))
    {
        if(idx < 100)
        {  
	    idx++;
            //std::string seq_no = "seq"+to_string(idx);
	    cout << ">seq" << idx << endl;
//            cout << seq_no << endl; 
            cout << str << endl;
        }
        
    }

    return 0;
}
