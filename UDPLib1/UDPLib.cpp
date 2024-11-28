// UDPLib1.cpp : Define las funciones de la biblioteca estática.
//
#include <iostream>
#include <ostream>
#include "pch.h"
#include "framework.h"
#include "UDPLib.h"
#include <format>

std::ostream& operator << (std::ostream& os, const DataPacket& dp) {
    return (os << "DataPacket{client: " << dp.client_id << " seq: " << dp.sequence /*<< " msg: " << dp.msg */<< " }");
}

//will print msg with WSAGetLastError, then closesocket
void treatError(const std::string msg, SOCKET s) {
    std::cout << msg << WSAGetLastError() << std::endl;
    closesocket(s);
}

//will print msg with WSAGetLastError, then closesocket, WSACleanup and ExitProcess(error)
void treatErrorExit(const std::string msg, SOCKET s, int error) {
    std::cout << msg << WSAGetLastError() << std::endl;
    closesocket(s);
    WSACleanup();
    ExitProcess(error);
}
//needs work
int udpCommonSocketSetup(SOCKET s, PCSTR address, u_short port, sockaddr_in* addr) {
    //now we create a socket that uses IP (AF_INET) with UDP (SOCK_DGRAM and IPPROTO_UDP) 
    s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    //assert(s != INVALID_SOCKET);
    if (s == INVALID_SOCKET) {
        treatErrorExit("Server: Socket creation error: ", s, -1);
    }
    std::cout << "Server: socket created" << std::endl;

    //now we specify the other machine we want to send messages to
    //sockaddr_in my_addr;
    //PCSTR address = "127.0.0.1";
    if (!inet_pton(AF_INET, address, &(addr->sin_addr.s_addr))) { // Replace with your desired IP address
        treatErrorExit("Server: error converting IP in string to binary: ", s, -1);
    }
    addr->sin_family = AF_INET;
    addr->sin_port = htons(port); //store bytes in network format big-endian
    return 0;
}
//needs work
int udpServerSocketSetup(SOCKET s_server, PCSTR address, u_short port, sockaddr_in* my_addr) {
    //sockaddr_in my_addr;
    udpCommonSocketSetup(s_server, address, port, my_addr);
    //in the server we need to bind in order to be able to recvfrom through the socket
    int result = bind(s_server, (sockaddr*)&my_addr, sizeof(my_addr));
    //assert(result != SOCKET_ERROR);
    if (result == SOCKET_ERROR) {
        treatErrorExit("Server: bind error", s_server, -1);
    }
    std::cout << "Server: socket bound to address: " << address << " port: " << my_addr->sin_port << std::endl;
    return 0;
}

//UDP calls

//performs sendto, assuming all required WinSock2 previous calls were succesfull
int sendtoMsg(SOCKET s, sockaddr_in* dest_addr, PDataPacket packet, std::string prefix) {
    //now we just send the data through the socket
    //we make the casting to char* because it expects data as just chars, last param 0 is for flags that we don't need
    int result = sendto(s, (char*)packet, sizeof(DataPacket), 0, (SOCKADDR*)dest_addr, sizeof(SOCKADDR));
    assert(result != SOCKET_ERROR);

    std::cout << prefix << " succesfully sent msg: " << *packet << std::endl;
    return result;
}

//performs recvfrom, assuming all required WinSock2 previous calls were succesfull
int recvfromMsg(SOCKET s, sockaddr_in* sender_addr, PDataPacket response, std::string prefix) {
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
int sendtorecvfromMsg(SOCKET s, sockaddr_in* dest_addr, PDataPacket packet, PDataPacket response, std::string prefix) {
    sendtoMsg(s, dest_addr, packet, prefix);
    return recvfromMsg(s, dest_addr, response, prefix);
}

//performs recvfrom and then sendto, assuming all required WinSock2 previous calls were succesfull
int recvfromsendtoMsg(SOCKET s, PDataPacket response, std::string prefix) {
    sockaddr_in sender_addr;
    recvfromMsg(s, &sender_addr, response, prefix);
    return sendtoMsg(s, &sender_addr, response, prefix);
}


//TCP calls

//performs send, assuming all required WinSock2 previous calls were succesfull
int sendMsg(SOCKET acceptSocket, PDataPacket packet, std::string prefix) {
    //IMPORTANT: size of struct/class not the size of the pointer!
    int sbyteCount = send(acceptSocket, (char*)packet, sizeof(DataPacket), 0);
    if (sbyteCount < 0) {
        treatError(std::format("{} send error: ", prefix), acceptSocket);
    }
    else {
        std::cout << prefix << " succesfully sent msg: " << *packet << std::endl;
    }
    return sbyteCount;
}

//performs recv, assuming all required WinSock2 previous calls were succesfull
int recvMsg(SOCKET acceptSocket, PDataPacket recv_msg, std::string prefix) {
    int rbyteCount = recv(acceptSocket, (char*)recv_msg, sizeof(DataPacket), 0);
    if (rbyteCount < 0) {
        treatError(std::format("{} recv error: ", prefix), acceptSocket); //change when loop to treatError
    }
    else {
        DataPacket clientPacket = (DataPacket)*recv_msg;
        std::cout << prefix << " succesfully received: " << clientPacket << std::endl;
    }
    return rbyteCount;
}

//performs send and then recv, assuming all required WinSock2 previous calls were succesfull
int sendrecvMsg(SOCKET s, PDataPacket packet, PDataPacket response, std::string prefix) {
    sendMsg(s, packet, prefix);
    return recvMsg(s, response, prefix);
}

//performs recv and then send, assuming all required WinSock2 previous calls were succesfull
int recvsendMsg(SOCKET s, PDataPacket response, std::string prefix) {
    recvMsg(s, response, prefix);
    return sendMsg(s, response, prefix);
}

//invokes getsockname and retrieves the port number in this machine's architecture encoding
int getAssignedPort(SOCKET s, sockaddr_in* my_addr) {
    int namelen = sizeof(my_addr);
    if (getsockname(s, (SOCKADDR*)my_addr, &namelen) != 0) {
        treatErrorExit("Server: getsockname error: ", s, -1);
    }
    int assigned_port = ntohs(my_addr->sin_port);
    std::cout << "Server: socket bound to port: " << my_addr->sin_port << std::endl;
    return assigned_port;
}