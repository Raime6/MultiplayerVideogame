#pragma once
#include <time.h>
#include <Windows.h>
#include <iostream>
#include <ostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <string>
#include <assert.h>

#define MSG_SIZE 256

enum Action
{
    SHOWMAINMENU
};

typedef class DataPacket {
public:
    int client_id;
    int sequence;
    //enum action;
    //void[] variables;
    
    DataPacket() {};
    DataPacket(int _client_id, int _sequence) {
        //DataPacket(int _client_id, int _sequence, int _op1, int _operation, int _op2) {
        client_id = _client_id;
        sequence = _sequence;
        //action = _action;
    }
} *PDataPacket;

typedef class ThreadInfo {
public:
    int thread_id;
    SOCKET s;
    std::string prefix;
    ThreadInfo() {};
    ThreadInfo(int _thread_id, SOCKET _s, std::string _prefix) {
        thread_id = _thread_id;
        s = _s;
        prefix = _prefix.c_str();
    }
    ~ThreadInfo() {
        closesocket(s);
    }
} *PThreadInfo;

std::ostream& operator << (std::ostream& os, const DataPacket& dp);
// The reason this is in a separate file is because I want to use this
// on the server and the client

void treatError(const std::string msg, SOCKET s);

void treatErrorExit(const std::string msg, SOCKET s, int error);

int udpCommonSocketSetup(SOCKET s, PCSTR address, u_short port, sockaddr_in* addr);

int udpServerSocketSetup(SOCKET s, PCSTR address, u_short port, sockaddr_in* addr);

//UDP calls

int sendtoMsg(SOCKET s, sockaddr_in* dest_addr, PDataPacket packet, std::string prefix);

int recvfromMsg(SOCKET s, sockaddr_in* sender_addr, PDataPacket response, std::string prefix);

int sendtorecvfromMsg(SOCKET s, sockaddr_in* dest_addr, PDataPacket packet, PDataPacket response, std::string prefix);

int recvfromsendtoMsg(SOCKET s, PDataPacket response, std::string prefix);
