#ifndef ds_server
    #define ds_server

    #include <iostream>
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <unistd.h>
    #include "TP.h"
    #include "Request.hpp"
    #include "Response.hpp"

    using namespace std;

    class Server{
        using Callback = function<void(const int&)>;
        using Handler = function<void(const Request&,Response&)>;
        
        public:
            int MAX_CLIENTS;
            
            Server(){
                fd =  socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);

                if (fd == -1){
                    cout  << "socket creation failed." << endl;
                    exit(-1);
                }
                
                int opt = 1;

                if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt,sizeof(opt)) != 0){
                    cout << "setsockopt failed." << endl;
                    exit(-1);
                }

                serverAddress.sin_family = AF_INET;
                serverAddress.sin_addr.s_addr = INADDR_ANY;
                
                MAX_CLIENTS = 5;
            }

            Server &get(string uri,Handler cb){
                handler["GET"][uri] = cb;

                return (*this);
            }
            Server &post(string uri,Handler cb){
                handler["POST"][uri] = cb;

                return (*this);
            }
            Server &put(string uri,Handler cb){
                handler["PUT"][uri] = cb;

                return (*this);
            }
            Server &del(string uri,Handler cb){
                handler["DELETE"][uri] = cb;

                return (*this);
            }
            Server &all(string uri,Handler cb){
                handler["GET"][uri] = cb;
                handler["POST"][uri] = cb;
                handler["PUT"][uri] = cb;
                handler["DELETE"][uri] = cb;

                return (*this);
            }

            void listenTo(int port){
                serverAddress.sin_port = htons(port);

                if (bind(fd,(struct sockaddr*)&serverAddress,sizeof(serverAddress)) != 0){
                    cout << "socket binding failed." << endl;
                    exit(-1);
                }

                if (listen(fd, MAX_CLIENTS) != 0){
                    cout << "failed to listen." << endl;
                    exit(-1);
                }
                
                srvq.changeTPSize(MAX_CLIENTS);

                Callback cb = [this](const int &cfd){
                    Request req;
                    Response res;

                    char buffer[1024];

                    if (read(cfd,buffer,1024) == -1){
                        close(cfd);

                        return;
                    }

                    req.fromString(buffer);

                    if (req.method == ""){
                        close(cfd);

                        return;
                    }

                    cout << buffer << endl;

                    res.version = req.version;
                    res.status = 200;
                    res("Connection","close");

                    map<string,Handler>::iterator it = handler[req.method].begin();

                    while(it != handler[req.method].end()){
                        if (it->first == req.uri)
                            it->second(req,res);

                        ++it;
                    }

                    string resp = res.toString();
                    int totalByteSent = 0;

                    cout << resp << endl;

                    while (totalByteSent < resp.size()){
                        int byteSent = write(cfd,resp.c_str(),resp.size());

                        totalByteSent += byteSent;
                    }

                    close(cfd);
                };

                _listen(cb);
            }
            
            ~Server(){
                close(fd);
            }
        
        private:
            int fd;
            struct sockaddr_in serverAddress;
            
            TPQueue srvq;
            map <string,map<string,Handler>> handler;
            
            void _listen(Callback &cb){
                while(true){
                    struct sockaddr_in clientAddress;
                    socklen_t addressSize = sizeof(clientAddress);
                    
                    int cfd = accept(fd,(struct sockaddr*)&clientAddress,&addressSize);

                    if (cfd == -1)
                        continue;
                                        
                    srvq.enqueue([cb,cfd](){
                        cb(cfd);
                    });
                }
            }
    };

#endif
