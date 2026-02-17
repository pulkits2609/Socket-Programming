#include<iostream>
#include<sys/socket.h>
#include<sys/types.h>
#include<unistd.h>
#include<netinet/in.h>
#include<arpa/inet.h> //for inet_addr conversion function
#include<cstdint> //for functions like memset memcpy
#include<cstring>

int main(){
	int SocketFD = -1;
	SocketFD = socket(AF_INET,SOCK_STREAM,0);
    	if(SocketFD < 0){
        perror("Socket Creation Failed"); //perror is uesd instead of printf or cout so that we know what is the reason for failure
        return -1;
    	}
    	std::cout<<"Socket Creation Successful \n";

	//now to connect to the server , client needs the servers address , creating client address is of no use because client
	//does not require its identity , its provided by the OS
	struct sockaddr_in ServerAddress;
	ServerAddress.sin_family=AF_INET;
	ServerAddress.sin_port=htons(9000);
	ServerAddress.sin_addr.s_addr=inet_addr("127.0.0.1"); //servers address
	
	close(SocketFD);
	return 0;
}
