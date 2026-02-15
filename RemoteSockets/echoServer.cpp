#include<iostream>
#include<sys/socket.h>
#include<sys/types.h>
#include<unistd.h> //for read , write , close system calls
#include<netinet/in.h> //defines the structs sockaddr_in , in_addr , byte order conversions like htons htonl and vice versa , and protocol like IPPROTO_TCP , IPPROTO_UDP
//#include<netinet/ip.h> //this is only needed when we are manually constructing ip addresses , so its usually not required ! right now we are using existing structs to create ip address
#include<arpa/inet.h> //for inet_addr conversion function
#include<cstdint> //for functions like memset memcpy
#include<cstring>
int main(){
    int SocketFD =-1;
    SocketFD=socket(AF_INET,SOCK_STREAM,0);
    if(SocketFD < 0){
        perror("Socket Creation Failed"); //perror is uesd instead of printf or cout so that we know what is the reason for failure
        return -1;
    }
    std::cout<<"Socket Creation Successfull \n";
    //now we will be naming the socket by providing the address using bind 
    //we will be providing which ip address it belongs to and what is the port number
    //we can see info about the ip structure from "man 7 ip" page
    // struct sockaddr_in {
    //            sa_family_t    sin_family; /* address family: AF_INET */
    //            in_port_t      sin_port;   /* port in network byte order */
    //            struct in_addr sin_addr;   /* internet address */
    //        };

    //        /* Internet address */
    //     struct in_addr {
    //        uint32_t       s_addr;     /* address in network byte order */
    //    };

    struct sockaddr_in SocketAddress;
    memset(&SocketAddress,0,sizeof(SocketAddress));
    SocketAddress.sin_family=AF_INET;
    SocketAddress.sin_port=htons(9000); //we have manually assigned the port here and converted it into network byte order
    SocketAddress.sin_addr.s_addr= inet_addr("127.0.0.1"); //loopback address , localhost , only local processes can connect to it
    //now that our IP Address has been created ! we bind it 

    if(bind(SocketFD,(struct sockaddr *)&SocketAddress,sizeof(SocketAddress)) < 0){
        perror("Binding to Socket Failed");
        close(SocketFD);
        return -1;
    }
    //before printing the address and port , we need to convert them appropriately
    //inet_ntoa : internet network to address
    std::cout<<"Bind Success with : "<<inet_ntoa(SocketAddress.sin_addr)<<" Port : "<<ntohs(SocketAddress.sin_port)<<"\n";
    close(SocketFD);

    return 0;
}