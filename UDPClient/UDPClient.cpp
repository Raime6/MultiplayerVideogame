
#pragma comment(lib, "ws2_32.lib")

#include <stdio.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <assert.h>
#include <iostream>
#include <string>
#include <Windows.h>

#include "UDPLib.hpp"
#include "UI.hpp"

#define MAX_MSGS 4

using namespace VideoGameUDP;

int obtainNewPort (SOCKET s, sockaddr_in* server_addr, int client, string prefix);



int main(int argc, char* argv[])
{
    if (argc < 2) {
        std::cout << "UDPClient usage: .\\UDPClient <id>" << std::endl;
        ExitProcess(-1);
    }

    int client = atoi(argv[1]);
    std::cout << "Client: starting..." << std::endl;

    //required intialization of WinSock 2 library, it writes some data om wsaData to check everything is ok
    int result;
    WSAData wsaData;
    result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    assert(result == NO_ERROR);
    std::cout << "Client: WinSock started correctly" << std::endl;

    //now we create a socket that uses IP (AF_INET) with UDP (SOCK_DGRAM and IPPROTO_UDP) 
    SOCKET s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (s == INVALID_SOCKET)
        treatErrorExit("Client: Socket creation error: ", s, -1);

    std::cout << "Client: socket created" << std::endl << std::endl;

    //now we specify the other machine we want to send messages to
    sockaddr_in server_addr;
    PCSTR address = "127.0.0.1";
    if (!inet_pton(AF_INET, address, &(server_addr.sin_addr.s_addr))) { // Replace with your desired IP address
        printf("error converting IP in string to binary\n");
        return -1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(4000);

    string prefix = "Client:";
    int sequence = 0;
    obtainNewPort(s, &server_addr, client, prefix);
    sequence++;

    // Once the conexion is created, the Client creates de UI
    UI              ui;
    bool            serve     = true;
    clientGameState gameState = STATE_MAIN_MENU; // Determines de state of the Client game (Ej: 0 - exit game, 1 - character selection, etc.)
    
    // =================================================================================================================================
    // CLIENT BEHAVIOUR
    while (serve)
    { 
        functionType function = NOT_FUNCTION;

        gameState = ui.UIFun(gameState, function);

        if (function != NOT_FUNCTION)
        {
            PDataPacket packet = new DataPacket(client, sequence, function);
            sendtoMsg(s, &server_addr, packet, prefix);
            ++sequence;
        }

        if (gameState == STATE_EXIT_GAME)
            serve = false;
    }
    // =================================================================================================================================

    std::cout << "Client finishing..." << std::endl;

    int iResult = closesocket(s);
    if (iResult == SOCKET_ERROR) {
        wprintf(L"closesocket failed with error: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    WSACleanup();
    return 0;
}


//sends first msg to server and returns with the new server_addr used for the server for the dedicated socket
int obtainNewPort(SOCKET s, sockaddr_in* server_addr, int client, string prefix)
{
    PDataPacket packet = new DataPacket(client, -1, NOT_FUNCTION);
    std::cout << "Client ready to send: " << *packet << std::endl;

    PDataPacket response = new DataPacket();
    //will overwrite server_addr with the server addr with the new port, since the response msg in the server is sent through the new socket!
    sendtorecvfromMsg(s, server_addr, packet, response, prefix);
    std::cout << std::endl;

    return 0;
}

