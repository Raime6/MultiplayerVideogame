
#pragma once
#include <time.h>
#include <Windows.h>
#include <iostream>
#include <ostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <string>
#include <variant>
#include <map>
#include <assert.h>

#define MSG_SIZE 256

using Parameter = std::variant<int>;
using string    = std::string;

typedef class DataPacket
{
    private:
        int                         client_id;
        int                         sequence;
        string                      functionName;
        std::map<string, Parameter> parameters;
    
    public:
        DataPacket() {};
        DataPacket(int _client_id, int _sequence, string _functionName)
        {
            client_id    = _client_id;
            sequence     = _sequence;
            functionName = _functionName;
        }

        int getClient()
        {
            return client_id;
        }
        int getSequence()
        {
            return sequence;
        }
        string getFunction()
        {
            return functionName;
        }
} *PDataPacket;

typedef class ThreadInfo
{
    private:
        int    thread_id;
        SOCKET s;
        string prefix;

    public:
        ThreadInfo() {};
        ThreadInfo(int _thread_id, SOCKET _s, std::string _prefix)
        {
            thread_id = _thread_id;
            s         = _s;
            prefix    = _prefix.c_str();
        }
        
        ~ThreadInfo()
        {
            closesocket(s);
        }

        int getId()
        {
            return thread_id;
        }
        SOCKET getSocket()
        {
            return s;
        }
        string getPrefix()
        {
            return prefix;
        }
        void setPrefix(string _prefix)
        {
            prefix = _prefix;
        }
} *PThreadInfo;



// METHODS

std::ostream& operator << (std::ostream& os, DataPacket& dp);
// The reason this is in a separate file is because I want to use this
// on the server and the client

void treatError           (const std::string msg, SOCKET s);

void treatErrorExit       (const std::string msg, SOCKET s, int error);

int  udpCommonSocketSetup (SOCKET s, PCSTR address, u_short port, sockaddr_in* addr);

int  udpServerSocketSetup (SOCKET s, PCSTR address, u_short port, sockaddr_in* addr);



//UDP calls

int sendtoMsg         (SOCKET s, sockaddr_in* dest_addr, PDataPacket packet, std::string prefix);

int recvfromMsg       (SOCKET s, sockaddr_in* sender_addr, PDataPacket response, std::string prefix);

int sendtorecvfromMsg (SOCKET s, sockaddr_in* dest_addr, PDataPacket packet, PDataPacket response, std::string prefix);

int recvfromsendtoMsg (SOCKET s, PDataPacket response, std::string prefix);
