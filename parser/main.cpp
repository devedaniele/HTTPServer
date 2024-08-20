#include <iostream>
#include <string>
//#include <cstring>

using namespace std;

int main(){
    string req = "GET / HTTP/1.1\nHost: 127.0.0.1:8080\nAccept: *";

    char *res;
    const char *begin = req;

    while(begin != req.end()){
        /* switch (inp){
            case '':
            /* code 
                break;
        
            default:
                break;
        } */
       char inp = begin++;

        cout << inp;
        inp++;
    }

    cout << endl << res << endl;

    return 0;
}