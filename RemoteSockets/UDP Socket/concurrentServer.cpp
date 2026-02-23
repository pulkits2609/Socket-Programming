#include<iostream>
#include<sys/socket.h>
#include<sys/types.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<cstring>

int main(){
    int SocketFD=-1,ret=-1;
    char ReadBuffer[50]{};
    SocketFD = socket(AF_INET,SOCK_DGRAM,0);
    if(SocketFD < 0){
        perror("Error Creating Socket");
        return -1;
    }
    std::cout<<"Socket Creation Successfull\n";

    //binding to address
    sockaddr_in ServerAddress{},ClientAddress{};
    ServerAddress.sin_family=AF_INET;
    ServerAddress.sin_port=htons(9000);
    ServerAddress.sin_addr.s_addr=inet_addr("127.0.0.1");

    //binding
    ret = bind(SocketFD,(struct sockaddr *)&ServerAddress,sizeof(ServerAddress));
    if(ret < 0){
        perror("Error Bindind Address to Socket");
        close(SocketFD);
        return -1;
    }
    std::cout<<"Binding Address Successful "<<inet_ntoa(ServerAddress.sin_addr)<<" PORT : "<<ntohs(ServerAddress.sin_port)<<"\n";
    std::cout<<"Server Started ::::::::::\n";
    while(1){
        memset(&ClientAddress,0,sizeof(ClientAddress));
        memset(&ReadBuffer,0,sizeof(ReadBuffer));
        socklen_t ClientSize =sizeof(ClientAddress);
        ssize_t Bytes=0;
        Bytes = recvfrom(SocketFD,&ReadBuffer,sizeof(ReadBuffer),0,(struct sockaddr *)&ClientAddress,&ClientSize);
        if(Bytes <= 0){
            perror("Error Receiving Message From Client");
            continue;
        }
        std::cout<<"Client "<<inet_ntoa(ClientAddress.sin_addr)<<":"<<ntohs(ClientAddress.sin_port)<<" Sent Message :\n";
        std::cout.write(ReadBuffer,Bytes)<<"\n";
        Bytes = sendto(SocketFD,"Hello From Server",18,0,(struct sockaddr *)&ClientAddress,sizeof(ClientAddress));
        if(Bytes <= 0){
            perror("Error Sending Message to Client");
            continue;
        }
        std::cout<<"Responded and closed Client Connection\n";        
    }
    close(SocketFD);
    return 0;
}