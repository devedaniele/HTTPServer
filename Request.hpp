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
                return headers[header];
            }
            Request operator()(string header,string value){
                headers[header] = value;

                return (*this);
            }

            Request parseType(string type,function<void(Request&)> callback){
                types[type] = callback;

                return (*this);
            }

            string toString(){
                string result = method + " " + uri + " " + version;

                return result;
            }
            Request fromString(string request){
                method = "GET";
                version = "HTTP/1.1";
                uri = "/";

                return (*this);
            }

        private:
            map<string,string> headers;
            map<string,function<void(Request&)>> types;
            vector<char> body;
    };

#endif