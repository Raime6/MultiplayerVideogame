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

using namespace std;
using namespace ServerVideoGame;

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
    PDataPacket pDataArray[MAX_THREADS];      //pointers to the data passed as params for each thread so that they can run the function with the params
    DWORD       dwThreadIdArray[MAX_THREADS]; //ids of threads as a long
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

    std::cout << "Server master: cleaning up and returning" << endl;
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
        treatErrorExit(format("Server master: Thread[{}] socket creation error: ", i), s, -1);
    
    std::cout << "Server master: socket created" << std::endl;

    //now we specify the other machine we want to send messages to
    sockaddr_in my_addr;
    PCSTR address = "127.0.0.1";
    if (!inet_pton(AF_INET, address, &(my_addr.sin_addr.s_addr))) // Replace with your desired IP address
        treatErrorExit(format("Server master: Thread[{}] error converting IP in string to binary: ", i), s, -1);

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
    PDataPacket packet = new DataPacket();
    thInfo->setPrefix("Server thread (" + std::to_string(thInfo->getId()) + "):");

    // Creates the videogame for the Client
    VideoGame videoGame;

    bool serve = true;
    while (serve)
    {
        std::cout << "Server thread (" << thInfo->getId() << ") ready to recv" << std::endl;

        // receive the packet and write its data to the packet struct, from the address addr with a specific length
        sockaddr_in client_addr;
        recvfromMsg(thInfo->getSocket(), &client_addr, packet, thInfo->getPrefix());

        DataPacket clientPacket = (DataPacket)*packet;

        //THREAD BEHAVIOUR

        // Exit Game
        /*cout << "ClientPacket: " << clientPacket << endl;
        cout << "ClientPacket: " << clientPacket.getFunction() << endl;
        cout << "Serve: "        << serve << endl;
        if (clientPacket.getFunction().compare("exitGame"))
            videoGame.exitGame(serve);
        cout << "Serve: " << serve << endl;*/
        
        Character* character;

        switch (clientPacket.getFunction())
        {
        case EXIT_GAME:
            serve = false;
            break;

        case CREATE_WARRIOR:
            character = new Warrior();
            break;

        case CREATE_MAGE:
            character = new Mage();
            break;

        case CREATE_PRIEST:
            character = new Priest();
            break;
        }

        cout << endl;
    }
    
    // TODO: close server thread with last msg
    
    // TODO:cleanup of thread
    if (thInfo != NULL)
    {
        if (!HeapFree(GetProcessHeap(), 0, thInfo))
            cout << "Fallo en el HeapFree" << endl;
        
        thInfo = NULL;
    }
    
    delete thInfo;
    thInfo = NULL;
    
    return 0;
}

