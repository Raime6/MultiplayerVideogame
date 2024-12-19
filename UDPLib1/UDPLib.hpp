
#pragma once
#include <time.h>
#include <Windows.h>
#include <ostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <map>
#include <assert.h>

// JSON
#include <nlohmann/json.hpp>
#include <iostream>

#define MSG_SIZE 256

//using Parameter = std::variant<int>;
using string = std::string;
using json   = nlohmann::json;



enum functionType
{
    NOT_FUNCTION,
    EXIT_GAME,
    CREATE_WARRIOR,
    CREATE_MAGE,
    CREATE_PRIEST,
    RETURN_ROOMS,
    GENERATE_SHOP,
    HEAL_CHARACTER,
    ADD_KEY,
    INCREASE_STRENGHT,
    INCREASE_VIGOR,
    INCREASE_ENDURANCE,
    GENERATE_ROOM,
    OPEN_CHEST,
    LEAVE_ROOM
};



enum shopItemType
{
    HEAL_POTION,
    KEY,
    STRENGTH_FLASK,
    VIGOR_FLASK,
    ENDURANCE_FLASK
};



enum roomType
{
    ROOM_FIGHT,
    ROOM_CHEST,
    NOT_ROOM
};



typedef class DataPacket
{
    public:
        int          client_id;
        int          sequence;
        functionType function;
        
        // RETURN_ROOMS
        int          currentRoom         = 0;
        int          maxRooms            = 0;

        // SHOP
        shopItemType shopItems[3];
        int          shopItemCosts[5]    = {10, 5, 7, 7, 7};

        // CHARACTER
        int          playerMoney         = 0;
        int          playerKeys          = 0;
        int          playerCurrentHealth = 0;
        int          playerMaxHealth     = 0;

        //// ENEMY
        std::string  enemyName           = "";
        //int          enemyCurrentHealth  = 0;
        //int          enemyMaxHealth      = 0;

        // ROOM
        roomType     roomGenerated       = ROOM_FIGHT;
    
    public:
        DataPacket() {}
        
        DataPacket(int _client_id, int _sequence, functionType _function)
        {
            client_id      = _client_id;
            sequence       = _sequence;
            function       = _function;
        }

        DataPacket(int _client_id, int _sequence, functionType _function, int _currentRoom, int _maxRoom, int _playerMoney, int _playerKeys, int _playerCurrentHealth, int _playerMaxHealth, std::string _enemyName, int _enemyCurrentHealth, int _enemyMaxHealth, roomType _roomGenerated) : DataPacket(_client_id, _sequence, _function)
        {
            currentRoom         = _currentRoom;
            maxRooms            = _maxRoom;
            playerMoney         = _playerMoney;
            playerKeys          = _playerKeys;
            playerCurrentHealth = _playerCurrentHealth;
            playerMaxHealth     = _playerMaxHealth;
            enemyName           = _enemyName;
            //enemyCurrentHealth  = _enemyCurrentHealth;
            //enemyMaxHealth      = _enemyMaxHealth;
            roomGenerated       = _roomGenerated;
        }
} *PDataPacket;

typedef class ThreadInfo
{
    public:
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
} *PThreadInfo;



// METHODS

std::ostream& operator << (std::ostream& os, DataPacket& dp);
// The reason this is in a separate file is because I want to use this
// on the server and the client

void treatError           (const std::string msg, SOCKET s);

void treatErrorExit       (const std::string msg, SOCKET s, int error);

int  udpCommonSocketSetup (SOCKET s, PCSTR address, u_short port, sockaddr_in* addr);

int  udpServerSocketSetup (SOCKET s, PCSTR address, u_short port, sockaddr_in* addr);



// JSON

void to_json(json& j, const DataPacket& d);

void from_json(const json& j, DataPacket& p);



//UDP calls

int sendtoMsg         (SOCKET s, sockaddr_in* dest_addr, PDataPacket packet, std::string prefix);

int recvfromMsg       (SOCKET s, sockaddr_in* sender_addr, PDataPacket response, std::string prefix);

int sendtorecvfromMsg (SOCKET s, sockaddr_in* dest_addr, PDataPacket packet, PDataPacket response, std::string prefix);

int recvfromsendtoMsg (SOCKET s, PDataPacket response, std::string prefix);
