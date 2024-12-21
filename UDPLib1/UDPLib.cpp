
// UDPLib1.cpp : Define las funciones de la biblioteca estática.
//
#include <iostream>
#include <ostream>
#include <format>

#include "pch.h"
#include "framework.h"
#include "UDPLib.hpp"



// METHODS

std::ostream& operator << (std::ostream& os, DataPacket& dp)
{
    return (os << "DataPacket{client: " << dp.client_id << " seq: " << dp.sequence << " func: " << dp.function << " }");
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



// JSON

//these functions should be visible to the library and it will call them automagically!
void to_json(json& j, const DataPacket& d)
{
    j = json
    {
        {"client_id"          , d.client_id},
        {"sequence"           , d.sequence},
        {"function"           , d.function},
        {"currentRoom"        , d.currentRoom},
        {"maxRooms"           , d.maxRooms},
        {"shopItems"          , d.shopItems},
        {"shopItemCosts"      , d.shopItemCosts},
        {"playerMoney"        , d.playerMoney},
        {"playerKeys"         , d.playerKeys},
        {"playerAttack"       , d.playerAttack},
        {"playerCurrentHealth", d.playerCurrentHealth},
        {"playerMaxHealth"    , d.playerMaxHealth},
        {"roomGenerated"      , d.roomGenerated},
        {"enemyName"          , d.enemyName},
        {"enemyCurrentHealth" , d.enemyCurrentHealth},
        {"enemyMaxHealth"     , d.enemyMaxHealth},
        {"enemyReward"        , d.enemyReward }
    };
}

//create Datapacket from json object
void from_json(const json& j, DataPacket& d)
{
    j.at("client_id")          .get_to(d.client_id);
    j.at("sequence")           .get_to(d.sequence);
    j.at("function")           .get_to(d.function);
    j.at("currentRoom")        .get_to(d.currentRoom);
    j.at("maxRooms")           .get_to(d.maxRooms);
    j.at("shopItems")          .get_to(d.shopItems);
    j.at("shopItemCosts")      .get_to(d.shopItemCosts);
    j.at("playerMoney")        .get_to(d.playerMoney);
    j.at("playerKeys")         .get_to(d.playerKeys);
    j.at("playerAttack")       .get_to(d.playerAttack);
    j.at("playerCurrentHealth").get_to(d.playerCurrentHealth);
    j.at("playerMaxHealth")    .get_to(d.playerMaxHealth);
    j.at("roomGenerated")      .get_to(d.roomGenerated);
    j.at("enemyName")          .get_to(d.enemyName);
    j.at("enemyCurrentHealth") .get_to(d.enemyCurrentHealth);
    j.at("enemyMaxHealth")     .get_to(d.enemyMaxHealth);
    j.at("enemyReward")        .get_to(d.enemyReward);
}



//UDP calls

//performs sendto, assuming all required WinSock2 previous calls were succesfull
int sendtoMsg(SOCKET s, sockaddr_in* dest_addr, PDataPacket packet, std::string prefix)
{
    // conversion: DataPacket -> json invoking to_json
    json j = *packet;
    
    std::cout << j.dump() << std::endl; // also std::cout << j << std::endl;

    char json_text[MSG_SIZE];
    memcpy(json_text, j.dump().c_str(), MSG_SIZE); // deep copy of data to buffer that will be sent

    //now we just send the data through the socket
    //we make the casting to char* because it expects data as just chars, last param 0 is for flags that we don't need
    int result = sendto(s, json_text, sizeof(json_text), 0, (SOCKADDR*)dest_addr, sizeof(SOCKADDR));
    assert(result != SOCKET_ERROR);

    std::cout << prefix << " succesfully sent msg: " << *packet << std::endl << std::endl;

    return result;
}

//performs recvfrom, assuming all required WinSock2 previous calls were succesfull
int recvfromMsg(SOCKET s, sockaddr_in* sender_addr, PDataPacket response, std::string prefix)
{
    //receive response from Server
    char buffer[MSG_SIZE];
    int  fromlen = sizeof(SOCKADDR);

    //recvfrom addr is ALWAYS an out param 
    int result = recvfrom(s, buffer, sizeof(buffer), 0, (SOCKADDR*)sender_addr, &fromlen);
    assert(result != SOCKET_ERROR);

    json j2   = json::parse(buffer);

    std::cout << j2.dump() << std::endl;

    //conversion json -> DataPacket invoking from_json
    *response = j2;

    std::cout << prefix << " succesfully received: " << *response << std::endl << std::endl;

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
