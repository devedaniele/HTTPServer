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
                return headers[header];
            }
            Response operator()(string header,string value){
                headers[header] = value;

                return (*this);
            }
        
            void end(string msg){
                const char* char_msg = msg.c_str();

                body.reserve(msg.size());
                (*this)("Content-Length",to_string(msg.size()));

                if ((*this)["Content-Type"] == "")
                    (*this)("Content-Type","text/html");

                for (int i = 0;i < msg.size();i++)
                    body.push_back(char_msg[i]);
            }

            Response parseType(string type,function<string(vector<char>&)> callback){
                types[type] = callback;

                return (*this);
            }

            string toString(){
                string result = version + " " + to_string(status) + " " + STATUS[status];

                map<string,string>::iterator it = headers.begin();

                while(it != headers.end()){
                    result += "\n" + it->first + ": " + it->second;

                    ++it;
                }

                if (headers["Content-Type"] != ""){
                    result += "\n\n";

                    string conetnt_type = headers["Content-Type"];

                    if (types[conetnt_type])
                        result += types[conetnt_type](body);
                    else{
                        result += string(body.begin(),body.end());
                    }
                }else if(headers["Content-Length"] != ""){
                    result += "\n\n";

                    result += string(body.begin(),body.end());
                }

                return result;
            }
            Response fromString(string request){
                return (*this);
            }

        private:
            map<string,string> headers;
            map<string,function<string(vector<char>&)>> types;
            vector<char> body;
    };

#endif
