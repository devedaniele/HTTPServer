#include <map>
#include <vector>
#include <string>

#ifndef ds_Request
    #define ds_Request

    using namespace std;

    class Request{
        public:
            string method;
            string uri;
            string version;

            string operator[](string header){
                return (*this).headers[header];
            }
            Request operator()(string header,string value){
                (*this).headers[header] = value;

                return (*this);
            }

            Request parseType(string type,function<void(Request&)> callback){
                (*this).types[type] = callback;

                return (*this);
            }

            string toString(){
                return "hello world";
            }
            Request fromString(string request){
                return (*this);
            }

        private:
            map<string,string> headers;
            map<string,function<void(Request&)>> types;
            vector<char> body;
    };

#endif