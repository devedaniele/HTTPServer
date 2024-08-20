#include <map>
#include <vector>
#include <string>

#ifndef ds_Response
    #define ds_Response

    using namespace std;

    map<int,string> STATUS = {
        {100,"Continue"},
        {101,"Switching Protocols"},
        {200,"OK"},
        {201,"Created"},
        {202,"Accepted"},
        {204,"No Content"},
        {300,"Multiple Choices"},
        {301,"Moved Permanently"},
        {302,"Found"},
        {400,"Bad Request"},
        {401,"Unauthorized"},
        {403,"Forbidden"},
        {404,"Not Found"},
        {405,"Method Not Allowed"},
        {406,"Not Acceptable"},
        {408,"Request Timeout"}
    };

    class Response{
        public:
            string version;
            int status;

            string operator[](string header){
                return (*this).headers[header];
            }
            Response operator()(string header,string value){
                (*this).headers[header] = value;

                return (*this);
            }
        
            void end(string msg){
            }

            Response parseType(string type,function<void(Response&)> callback){
                (*this).types[type] = callback;

                return (*this);
            }

            string toString(){
                (*this).types[(*this).headers["Content-Type"]]((*this));

                return "\n\n";
            }
            Response fromString(string request){
                return (*this);
            }

        private:
            map<string,string> headers;
            map<string,function<void(Response&)>> types;
            vector<char> body;
    };

#endif
