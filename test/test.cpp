#include "../server.h"
#include <fstream>

using namespace std;

//Server srv;

int main(){
    Server srv;
    
    srv.get("/",[](const Request &req,Response &res){
        filebuf file;
        file.open("index.html",ios_base::in);
        int len = file.in_avail();
        char body[len];
        file.sgetn(body,len);
        file.close();

        string ss = body;


        cout << ss.length() << endl;

        res.end(body);
    }); /*
    srv.post("/",[](Request&,Response &res){
        res.end("done");
    }); */

    cout << "staring server....." << endl;

    srv.listenTo(8767);
    
    return 0;
};
