#pragma comment(lib, "ws2_32.lib") //add WinSock2 library

#include <stdio.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <assert.h>
#include <iostream>
#include <math.h>
#include <format>

#include "UDPLib.hpp"
#include "VideoGame.hpp"

#define MAX_THREADS 5

using namespace VideoGameUDP;

HANDLE       serverFun (PDataPacket clientPacket, SOCKET s, sockaddr_in* client_addr, int i, string prefix);
DWORD WINAPI threadFun (LPVOID param);



int main()
{
    string prefix = "Server master:";
    std::cout << "Server master: starting..." << std::endl;

    //required intialization of WinSock 2 library, it writes some data om wsaData to check everything is ok
    int result;
    WSAData wsaData;
    result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    assert(result == NO_ERROR);

    std::cout << "Server master: WinSock started correctly" << std::endl;

    //now we create a socket that uses IP (AF_INET) with UDP (SOCK_DGRAM and IPPROTO_UDP)
    SOCKET s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (result == INVALID_SOCKET)
        treatErrorExit("Server master: Socket creation error: ", s, -1);

    std::cout << prefix << " socket created" << std::endl;

    //now we specify the other machine we want to send messages to
    sockaddr_in my_addr;
    PCSTR address = "127.0.0.1";
    if (!inet_pton(AF_INET, address, &(my_addr.sin_addr.s_addr))) // Replace with your desired IP address
        treatErrorExit("Server master: error converting IP in string to binary: ", s, -1);

    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(4000); //store bytes in network format big-endian

    //in the server we need to bind in order to be able to recvfrom through the socket
    result = bind(s, (sockaddr*)&my_addr, sizeof(my_addr));
    if (result == SOCKET_ERROR)
        treatErrorExit("Server master: bind error", s, -1);

    std::cout << "Server master: socket bound to address: " << address << " port: " << my_addr.sin_port << std::endl << std::endl;

                     PDataPacket packet = new DataPacket();
    [[maybe_unused]] PDataPacket pDataArray[MAX_THREADS];      //pointers to the data passed as params for each thread so that they can run the function with the params
    [[maybe_unused]] DWORD       dwThreadIdArray[MAX_THREADS]; //ids of threads as a long
                     HANDLE      hThreadArray[MAX_THREADS];    //handlers of threads
    
    int i = 0;
    while (i < MAX_THREADS)
    {
        std::cout << "Server master ready to recv" << std::endl;

        //recv msg and call serverFun
        sockaddr_in client_addr;
        recvfromMsg(s, &client_addr, packet, prefix);

        //do something
        hThreadArray[i] = serverFun(packet, s, &client_addr, i, prefix);
        ++i;
    }

    // Wait until all threads have terminated.
    //how many to wait for, array of handlers, wait for all of them, how long to wait
    WaitForMultipleObjects(MAX_THREADS, hThreadArray, TRUE, INFINITE);

    // Close all thread handles and free memory allocations.
    //this could be in a different function
    for (int i = 0; i < MAX_THREADS; i++)
        CloseHandle(hThreadArray[i]);

    std::cout << "Server master: cleaning up and returning" << std::endl;
    // cleanup
    closesocket(s);
    WSACleanup();
}


//makes operation with op1 and op2 storing the result in res, all of them fields of clientPacket
HANDLE serverFun(PDataPacket clientPacket, SOCKET s, sockaddr_in* client_addr, int i, string prefix)
{
    int result = -1;
    
    //now we create a socket that uses IP (AF_INET) with UDP (SOCK_DGRAM, IPPROTO_UDP) 
    SOCKET s_new = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (s_new == INVALID_SOCKET)
        treatErrorExit(std::format("Server master: Thread[{}] socket creation error: ", i), s, -1);
    
    std::cout << "Server master: socket created" << std::endl;

    //now we specify the other machine we want to send messages to
    sockaddr_in my_addr;
    PCSTR address = "127.0.0.1";
    if (!inet_pton(AF_INET, address, &(my_addr.sin_addr.s_addr))) // Replace with your desired IP address
        treatErrorExit(std::format("Server master: Thread[{}] error converting IP in string to binary: ", i), s, -1);

    my_addr.sin_family = AF_INET;
    //store bytes in network format == big-endian
    my_addr.sin_port = htons(0); //4000 Replace with your desired port or 0 so that the OS chooses one that is available

    //in the server we need to bind in order to be able to listen through the socket
    result = bind(s_new, (sockaddr*)&my_addr, sizeof(my_addr));
    if (result == SOCKET_ERROR)
        treatErrorExit("Server master: bind error", s, -1);

    std::cout << "Server master: socket bound to address: " << address << " port: " << my_addr.sin_port << std::endl;

    //int new_port = getAssignedPort(s_new, &my_addr); // getsockname apparently only works with connection oriented sockets!

    //sendtoMsg(...new_port...) through the new socket, so that the client gets a server_addr with the new port included
    sendtoMsg(s_new, client_addr, clientPacket, prefix);
    std::cout << std::endl;

    PThreadInfo thInfo = new ThreadInfo(i, s_new, prefix);
    //call to thread(... s_new ...) with the new socket that only the thread will use
    DWORD dwThreadId;
    HANDLE hThread = CreateThread
    (
        NULL,                   // default security attributes
        0,                      // use default stack size  
        threadFun,              // thread function name
        (void*)thInfo,          // argument to thread function 
        0,                      // use default creation flags 
        &dwThreadId
    ); // returns the thread identifier 

    if (hThread == NULL)
    {
        treatErrorExit("CreateThread", s, -1);
        ExitProcess(3);
    }

    return hThread;
}

DWORD WINAPI threadFun(LPVOID param)
{
    PThreadInfo thInfo = (ThreadInfo*)param;

    // TODO: while () receive msgs, send them back
    PDataPacket packet   = new DataPacket();
    PDataPacket response;
    thInfo->prefix = "Server thread (" + std::to_string(thInfo->thread_id) + "):";

    // Creates the videogame for the Client
    [[maybe_unused]] VideoGame videoGame;

    // =================================================================================================================================
    // SERVER BEHAVIOUR

    bool serve = true;
    while (serve)
    {
        std::cout << "Server thread (" << thInfo->thread_id << ") ready to recv" << std::endl;

        // receive the packet and write its data to the packet struct, from the address addr with a specific length
        sockaddr_in client_addr;
        recvfromMsg(thInfo->s, &client_addr, packet, thInfo->prefix);

        DataPacket clientPacket = (DataPacket)*packet;

        //THREAD BEHAVIOUR
        serve = videoGame.videoGameFun(clientPacket.function);

        switch (clientPacket.function)
        {
            case RETURN_ROOMS:
                videoGame.currentRoom  = 1;
                videoGame.playerKeys   = 0;
                videoGame.playerMoney  = 0;
                response = new DataPacket(clientPacket.client_id, clientPacket.sequence, clientPacket.function, videoGame.currentRoom, videoGame.maxRooms, videoGame.playerMoney, videoGame.playerKeys, videoGame.roomGenerated, videoGame.character->attack, videoGame.character->health, videoGame.character->maxHealth, videoGame.character->defense);
                sendtoMsg(thInfo->s, &client_addr, response, thInfo->prefix);
                break;

            case GENERATE_ROOM:
                if (videoGame.roomGenerated == ROOM_FIGHT)
                    response = new DataPacket(clientPacket.client_id, clientPacket.sequence, clientPacket.function, videoGame.currentRoom, videoGame.maxRooms, videoGame.playerMoney, videoGame.playerKeys, videoGame.roomGenerated, videoGame.character->attack, videoGame.character->health, videoGame.character->maxHealth, videoGame.character->defense, videoGame.enemy->name, videoGame.enemy->health, videoGame.enemy->maxHealth, videoGame.enemy->attack, videoGame.enemy->reward);
                else
                    response = new DataPacket(clientPacket.client_id, clientPacket.sequence, clientPacket.function, videoGame.currentRoom, videoGame.maxRooms, videoGame.playerMoney, videoGame.playerKeys, videoGame.roomGenerated, videoGame.character->attack, videoGame.character->health, videoGame.character->maxHealth, videoGame.character->defense);
                sendtoMsg(thInfo->s, &client_addr, response, thInfo->prefix);
                break;

            case OPEN_CHEST:
                response = new DataPacket(clientPacket.client_id, clientPacket.sequence, clientPacket.function, videoGame.currentRoom, videoGame.maxRooms, videoGame.playerMoney, videoGame.playerKeys, videoGame.roomGenerated, videoGame.character->attack, videoGame.character->health, videoGame.character->maxHealth, videoGame.character->defense);
                sendtoMsg(thInfo->s, &client_addr, response, thInfo->prefix);
                break;

            case LEAVE_ROOM:
                response = new DataPacket(clientPacket.client_id, clientPacket.sequence, clientPacket.function, videoGame.currentRoom, videoGame.maxRooms, videoGame.playerMoney, videoGame.playerKeys, videoGame.roomGenerated, videoGame.character->attack, videoGame.character->health, videoGame.character->maxHealth, videoGame.character->defense);
                sendtoMsg(thInfo->s, &client_addr, response, thInfo->prefix);
                break;

            case CHARACTER_ATTACK:
                response = new DataPacket(clientPacket.client_id, clientPacket.sequence, clientPacket.function, videoGame.currentRoom, videoGame.maxRooms, videoGame.playerMoney, videoGame.playerKeys, videoGame.roomGenerated, videoGame.character->attack, videoGame.character->health, videoGame.character->maxHealth, videoGame.character->defense, videoGame.enemy->name, videoGame.enemy->health, videoGame.enemy->maxHealth, videoGame.enemy->attack, videoGame.enemy->reward);
                sendtoMsg(thInfo->s, &client_addr, response, thInfo->prefix);
                break;

            case CHARACTER_ABILITY:
                response = new DataPacket(clientPacket.client_id, clientPacket.sequence, clientPacket.function, videoGame.currentRoom, videoGame.maxRooms, videoGame.playerMoney, videoGame.playerKeys, videoGame.roomGenerated, videoGame.character->attack, videoGame.character->health, videoGame.character->maxHealth, videoGame.character->defense, videoGame.enemy->name, videoGame.enemy->health, videoGame.enemy->maxHealth, videoGame.enemy->attack, videoGame.enemy->reward);
                sendtoMsg(thInfo->s, &client_addr, response, thInfo->prefix);
                break;

            case GENERATE_SHOP:
                response = new DataPacket(clientPacket.client_id, clientPacket.sequence, clientPacket.function, videoGame.currentRoom, videoGame.maxRooms, videoGame.playerMoney, videoGame.playerKeys, videoGame.roomGenerated, videoGame.character->attack, videoGame.character->health, videoGame.character->maxHealth, videoGame.character->defense);
                shopItemType arrayItems[5] = { HEAL_POTION, KEY, STRENGTH_FLASK, VIGOR_FLASK, ENDURANCE_FLASK };
                for (int i = 0; i < 3; i++)
                {
                    response->shopItems[i] = arrayItems[rand() % 5];
                }

                sendtoMsg(thInfo->s, &client_addr, response, thInfo->prefix);
                break;
        }

        std::cout << std::endl;
    }
    // =================================================================================================================================
    
    // TODO:cleanup of thread
    if (thInfo != NULL)
    {
        std::cout << "Server thread (" << thInfo->thread_id << "): cleaning up and returning" << std::endl;
        delete thInfo;
        thInfo = NULL;
    }

    return 0;
}

