#include <iostream>
#include "server.hpp"
#include <map>
#include "Request.hpp"
#include "Response.hpp"

#ifndef ds_http
    #define ds_http

    using namespace std;

    class http{
        public:
            string host;
            int port;

            http(){
                (*this).res("Content-Type","text/html");
                (*this).res("Connection","close");

                (*this).res.parseType("text/html",[](Response &res){
                    res;
                });
            }

            http get(string uri,function<void(Request&,Response&)> callback){
                (*this).handler["GET"][uri] = callback;

                return (*this);
            }
            http post(string uri,function<void(Request&,Response&)> callback){
                (*this).handler["POST"][uri] = callback;

                return (*this);
            }
            http put(string uri,function<void(Request&,Response&)> callback){
                (*this).handler["PUT"][uri] = callback;

                return (*this);
            }
            http del(string uri,function<void(Request&,Response&)> callback){
                (*this).handler["DELETE"][uri] = callback;

                return (*this);
            }

            void listen(){
                (*this).server.PORT = 8767;

                http _this = (*this);

                (*this).server.listenTo(5,[](Server &srv){
                    srv.readData();
                    srv.sendString("Helio World.");                    

                    srv.closeClient();
                });
            }

        private:
            Server server;
            Request req;
            Response res;

            map <string,map<string,function<void(Request&,Response&)>>> handler;
    };

#endif
