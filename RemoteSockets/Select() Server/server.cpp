#include<iostream>
#include<sys/socket.h>
#include<sys/types.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<sys/select.h> //for select system call
#include<cstring>

int main(){
    int SocketFD = -1,ret=-1;
    char ReadBuffer[20]{};

    //////Select System Call Variables ///////
    fd_set readFDs; //read file descriptor set
    //////////////////////////////////////////

    SocketFD = socket(AF_INET,SOCK_STREAM,0);
    if(SocketFD < 0){
        perror("Error Creating Socket");
        return -1;
    }
    //socket created
    
    //Server Address Creation
    sockaddr_in ServerAddress{},ClientAddress{};
    ServerAddress.sin_family=AF_INET;
    ServerAddress.sin_port=htons(9000);
    ServerAddress.sin_addr.s_addr=inet_addr("127.0.0.1");

    socklen_t len = sizeof(ClientAddress);
    
    //binding
    ret=bind(SocketFD,(struct sockaddr*)&ServerAddress,sizeof(ServerAddress));
    if(ret < 0){
        perror("Error Binding Socket to Address");
        close(SocketFD);
        return -1;
    }
    //bindind successfull
    std::cout<<"SERVER STARTED ON :::: "<<inet_ntoa(ServerAddress.sin_addr)<<":"<<ntohs(ServerAddress.sin_port)<<"\n";
    //listening
    ret = listen(SocketFD,5); //5 backlog
    if(ret < 0){
        perror("Error Listening");
        close(SocketFD);
        return -1;
    }
    std::cout<<":::::::::SERVER LISTENING::::::\n";

    ///////INITIALIZING SELECT()///////////
    FD_ZERO(&readFDs); //Zeroing the read fd set
    FD_SET(SocketFD,&readFDs); //adding sockfd or listen fd to rfds set
    fd_set iset; //iteration set
    int maxfd = SocketFD; //
    while(1){
        //take an iteration set , because after every new client connection we need to remove the listening socketFD from select system call
        iset = readFDs; //whenever we are adding socket we add it to readFDs , whenever we remove , we remove it from iset
        ret = select(maxfd+1, &iset, NULL, NULL, NULL);
        if(ret < 0){ //failure
            perror("Select System Call Failed");
            continue;
        }
        else if(ret > 0){ //data available on some FDs
            //whether data available on SocketFD or Some other
            if(FD_ISSET(SocketFD,&iset)==1){
                std::cout<<"SELECT::: New Connection Request \n";
                int SessionFD = accept(SocketFD,(struct sockaddr *)&ClientAddress,&len);
                if(SessionFD < 0){
                    perror("Accept Failed");
                    continue;
                }
                std::cout<<"Client Request accepted from "<<inet_ntoa(ClientAddress.sin_addr)<<":"<<ntohs(ClientAddress.sin_port)<<"\n";
                //add this new session to the rfds
                FD_SET(SessionFD,&readFDs);
                std::cout<<"New SessionFD added to list of ReadFDs\n";
                FD_CLR(SocketFD,&iset); //remove the FD on which the data has come
                if(maxfd < SessionFD){
                    maxfd = SessionFD;
                }
            }
            else{
                //in this case data is available other than SocketFD , that is from a client session FD
                for(int i=0;i<maxfd+1;i++){
                    if(FD_ISSET(i,&iset)==1){
                        memset(&ReadBuffer,0,sizeof(ReadBuffer));
                        //ith fd is ready or set with data
                        std::cout<<"SELECT::::"<<i<<"th fd is ready with data: \n";
                        ret = read(i,ReadBuffer,sizeof(ReadBuffer));
                        if(ret < 0){
                            perror("Error Reading ");
                            continue;
                        }
                        else if(ret==0){
                            //this case means end of the file !
                            std::cout<<"Client "<<i<<" Closing Connection\n";
                            close(i);
                            FD_CLR(i,&readFDs);
                            std::cout<<i<<"th session is removed from ReadFDs\n";
                        }
                        else if(ret > 0){ //meaning that data is read
                            std::cout<<"Data Read from "<<i<<"th fd : \n";
                            std::cout.write(ReadBuffer,ret)<<"\n";
                            write(i,"Hello",6);
                            std::cout<<"Data Written to Client\n";
                        }
                    }
                }
            }
        }   
        else if(ret == 0){ //only happens when timeout expires
            std::cout<<"timeout expired\n";
            continue;
        }
    }
}