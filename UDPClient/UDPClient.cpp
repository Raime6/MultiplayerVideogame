
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

using namespace std;
using namespace Interface;

int obtainNewPort (SOCKET s, sockaddr_in* server_addr, string prefix);



int main(int argc, char* argv[])
{
    if (argc < 2) {
        cout << "UDPClient usage: .\\UDPClient <id>" << endl;
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
    obtainNewPort(s, &server_addr, prefix);

    // Once the conexion is created, the Client creates de UI
    UI ui;
    int optionMainMenu = 0, optionSelectCharacter;
    
    // Main Menu
    do
    {
        // Shows UI
        optionMainMenu = ui.showMainMenu();
        
        // If User selects 1, shows SelectCharacer Menu, if it's 2, sends Datapack to close Thread in Server
        if (optionMainMenu == 1)
            optionSelectCharacter = ui.showSelectCharacter();
        else if (optionMainMenu == 2)
        {
            PDataPacket packet = new DataPacket(client, sequence, "exitGame");
            ++sequence;
            sendtoMsg(s, &server_addr, packet, prefix);
        }
    } while (optionMainMenu < 1 || optionMainMenu > 2);

    /*PDataPacket packet   = new DataPacket(client, 0, "printHello");
    PDataPacket response = new DataPacket();
    
    sendtoMsg(s, &server_addr, packet, prefix);*/

    //for (int i = 0; i < MAX_MSGS; i++) {

    //    DataPacket packet(client, i, SHOWMAINMENU, nullptr);
    //    if (i == MAX_MSGS - 1)
    //        packet.endServer = true;

    //    sendtoMsg(s, &server_addr, &packet, prefix);

    //    PDataPacket response = new DataPacket();

    //    string strMsg = string(packet.msg);
    //    if (strMsg.compare(0, 12, "Close_Server")) { //compare returns 0 when equal, we compare just the desired length
    //        //receive response from Server
    //        recvfromMsg(s, &server_addr, response, prefix);
    //        cout << endl;
    //    }

    //}

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
int obtainNewPort(SOCKET s, sockaddr_in* server_addr, string prefix)
{
    PDataPacket packet = new DataPacket(-1, -1, "");
    std::cout << "Client ready to send: " << *packet << std::endl;

    PDataPacket response = new DataPacket();
    //will overwrite server_addr with the server addr with the new port, since the response msg in the server is sent through the new socket!
    sendtorecvfromMsg(s, server_addr, packet, response, prefix);
    cout << endl;

    return 0;
}

