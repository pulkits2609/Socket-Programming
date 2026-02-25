#include<iostream>
#include<sys/socket.h>
#include<sys/types.h>
#include<unistd.h>
#include<arpa/inet.h>

int main(){
    int SocketFD=-1;
    char ReadBuffer[20]{};
    SocketFD=socket(AF_INET,SOCK_STREAM,0);
    if(SocketFD < 0){
        perror("Error Creating Socket");
        return -1;
    }
    std::cout<<"Socket Created!\n";
    
    sockaddr_in ServerAddress{};
    ServerAddress.sin_family=AF_INET;
    ServerAddress.sin_port=htons(9000);
    ServerAddress.sin_addr.s_addr=inet_addr("127.0.0.1");
    socklen_t ServSize = sizeof(ServerAddress);

    int ret=connect(SocketFD,(struct sockaddr*)&ServerAddress,ServSize);
    if(ret < 0){
        perror("Error Connecting to server");
        close(SocketFD);
        return -1;
    }
    std::cout<<"Connected to server : \n";
    write(SocketFD,"Hello From Client",18);
    ret = read(SocketFD,&ReadBuffer,sizeof(ReadBuffer));
    if(ret < 0){
        perror("Error Reading from server");
        close(SocketFD);
        return -1;
    }
    std::cout.write(ReadBuffer,ret)<<"\n";
    close(SocketFD);
    return 0;
}