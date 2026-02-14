#include<iostream>
#include<sys/socket.h> //for socket system calls
#include<sys/types.h> //for socket specific datatypes
#include<sys/un.h> //unix address families
#include<unistd.h>  //unix standard , read and write system calls 
#include<string.h>
#include<cstring>
int main(){
    int ClientFD = -1;
    char ReadBuffer[30]; // where client will store the contents of Message sent by server
    char WriteBuffer[] = "Hello Mister Server";

    ClientFD=socket(AF_UNIX,SOCK_STREAM,0);
    if(ClientFD<0){
        std::cout<<"Unable to Initialize Socket\n";
        return -1;
    }
    std::cout<<"Socket Initialized Successfully";
    //after creating socket , in client side instead of using bind , we directly connect to the server
    //to bind with the server , we need the server address !
    struct sockaddr_un ServerAddress={};
    ServerAddress.sun_family=AF_UNIX; //AF_LOCAL WOULD WORK TOO
    strcpy(ServerAddress.sun_path,"./LocalSocketFile");
    //now that we have the server address , we connect
    if(connect(ClientFD,(struct sockaddr *)&ServerAddress,sizeof(ServerAddress)) < 0){
        std::cout<<"Connection to Server Failed !!! \n";
        close(ClientFD);
        return -1;
    };
    std::cout<<"Connected to Server SUCCESSFULLY\n";
    ssize_t ReadWriteBytes=0;
    ReadWriteBytes = write(ClientFD,WriteBuffer,sizeof(WriteBuffer));
    if(ReadWriteBytes <= 0){
        std::cout<<"Error Writing To Server\n";
    }
    else{
        std::cout<<"Successfully Sent Message to Server";
    }
    ReadWriteBytes = read(ClientFD,ReadBuffer,sizeof(ReadBuffer));
    if(ReadWriteBytes < 0){
        std::cout<<"Error Reading Message From server\n";
    }
    else{
        std::cout<<"Message From Server : ";
        std::cout.write(ReadBuffer,sizeof(ReadBuffer))<<"\n";
    }
    close(ClientFD);
    std::cout<<"Client Connection Terminated From Server !!!\n";
    return 0;
}