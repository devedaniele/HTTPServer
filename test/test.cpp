#include "../server.hpp"

using namespace std;

Server srv;

int main(){
    srv.PORT = 8767;

    /* srv.get("/",[](Request &req,Response &res){
        res.end("hello world.");
    });
    srv.post("/",[](Request&,Response &res){
        res.end("done");
    }); */

    cout << "staring server....." << endl;

    srv.listenTo(5,[](Server &sr){
        sr.readData();
        sr.sendString("Hello World.");

        sr.closeClient();
    });
    
    return 0;
};
