
// UDPLib1.cpp : Define las funciones de la biblioteca estática.
//
#include <iostream>
#include <ostream>
#include <format>

#include "pch.h"
#include "framework.h"
#include "UDPLib.h"



// METHODS

std::ostream& operator << (std::ostream& os, DataPacket& dp)
{
    return (os << "DataPacket{client: " << dp.getClient() << " seq: " << dp.getSequence() << " func: " << dp.getFunction() << " }");
}

//will print msg with WSAGetLastError, then closesocket
void treatError(const std::string msg, SOCKET s)
{
    std::cout << msg << WSAGetLastError() << std::endl;
    closesocket(s);
}

//will print msg with WSAGetLastError, then closesocket, WSACleanup and ExitProcess(error)
void treatErrorExit(const std::string msg, SOCKET s, int error)
{
    std::cout << msg << WSAGetLastError() << std::endl;
    closesocket(s);
    WSACleanup();
    ExitProcess(error);
}
//needs work
int udpCommonSocketSetup(SOCKET s, PCSTR address, u_short port, sockaddr_in* addr)
{
    //now we create a socket that uses IP (AF_INET) with UDP (SOCK_DGRAM and IPPROTO_UDP) 
    s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    
    //assert(s != INVALID_SOCKET);
    if (s == INVALID_SOCKET)
        treatErrorExit("Server: Socket creation error: ", s, -1);

    std::cout << "Server: socket created" << std::endl;

    //now we specify the other machine we want to send messages to
    //sockaddr_in my_addr;
    //PCSTR address = "127.0.0.1";
    if (!inet_pton(AF_INET, address, &(addr->sin_addr.s_addr))) // Replace with your desired IP address
        treatErrorExit("Server: error converting IP in string to binary: ", s, -1);

    addr->sin_family = AF_INET;
    addr->sin_port   = htons(port); //store bytes in network format big-endian

    return 0;
}
//needs work
int udpServerSocketSetup(SOCKET s_server, PCSTR address, u_short port, sockaddr_in* my_addr)
{
    //sockaddr_in my_addr;
    udpCommonSocketSetup(s_server, address, port, my_addr);
    
    //in the server we need to bind in order to be able to recvfrom through the socket
    int result = bind(s_server, (sockaddr*)&my_addr, sizeof(my_addr));
    
    //assert(result != SOCKET_ERROR);
    if (result == SOCKET_ERROR)
        treatErrorExit("Server: bind error", s_server, -1);

    std::cout << "Server: socket bound to address: " << address << " port: " << my_addr->sin_port << std::endl;

    return 0;
}




//UDP calls

//performs sendto, assuming all required WinSock2 previous calls were succesfull
int sendtoMsg(SOCKET s, sockaddr_in* dest_addr, PDataPacket packet, std::string prefix)
{
    //now we just send the data through the socket
    //we make the casting to char* because it expects data as just chars, last param 0 is for flags that we don't need
    int result = sendto(s, (char*)packet, sizeof(DataPacket), 0, (SOCKADDR*)dest_addr, sizeof(SOCKADDR));
    assert(result != SOCKET_ERROR);

    std::cout << prefix << " succesfully sent msg: " << *packet << std::endl;

    return result;
}

//performs recvfrom, assuming all required WinSock2 previous calls were succesfull
int recvfromMsg(SOCKET s, sockaddr_in* sender_addr, PDataPacket response, std::string prefix)
{
    //receive response from Server
    //DataPacket response;
    int fromlen = sizeof(SOCKADDR);
    
    //recvfrom addr is ALWAYS an out param 
    int result = recvfrom(s, (char*)response, sizeof(DataPacket), 0, (SOCKADDR*)sender_addr, &fromlen);
    assert(result != SOCKET_ERROR);

    std::cout << prefix << " succesfully received: " << *response << std::endl;

    return result;
}

//performs sendto and then recvfrom, assuming all required WinSock2 previous calls were succesfull
int sendtorecvfromMsg(SOCKET s, sockaddr_in* dest_addr, PDataPacket packet, PDataPacket response, std::string prefix)
{
    sendtoMsg(s, dest_addr, packet, prefix);

    return recvfromMsg(s, dest_addr, response, prefix);
}

//performs recvfrom and then sendto, assuming all required WinSock2 previous calls were succesfull
int recvfromsendtoMsg(SOCKET s, PDataPacket response, std::string prefix)
{
    sockaddr_in sender_addr;
    recvfromMsg(s, &sender_addr, response, prefix);

    return sendtoMsg(s, &sender_addr, response, prefix);
}
