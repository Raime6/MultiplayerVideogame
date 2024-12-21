
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

#define MSG_SIZE 512

//using Parameter = std::variant<int>;
using string = std::string;
using json   = nlohmann::json;



enum functionType
{
    NOT_FUNCTION,       // 0
    EXIT_GAME,          // 1
    CREATE_WARRIOR,     // 2
    CREATE_MAGE,        // 3
    CREATE_PRIEST,      // 4
    RETURN_ROOMS,       // 5
    GENERATE_SHOP,      // 6
    HEAL_CHARACTER,     // 7
    ADD_KEY,            // 8
    INCREASE_STRENGHT,  // 9
    INCREASE_VIGOR,     // 10
    INCREASE_ENDURANCE, // 11
    GENERATE_ROOM,      // 12
    OPEN_CHEST,         // 13
    LEAVE_ROOM,         // 14
    CHARACTER_ATTACK,   // 15
    CHARACTER_ABILITY   // 16
};



enum shopItemType
{
    HEAL_POTION,    // 0
    KEY,            // 1
    STRENGTH_FLASK, // 2
    VIGOR_FLASK,    // 3
    ENDURANCE_FLASK // 4
};



enum roomType
{
    ROOM_FIGHT, // 0
    ROOM_CHEST, // 1
    NOT_ROOM    // 2
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

        // ROOM
        roomType     roomGenerated = NOT_ROOM;

        // CHARACTER
        int          playerMoney         = 0;
        int          playerKeys          = 0;
        int          playerAttack        = 0;
        int          playerCurrentHealth = 0;
        int          playerMaxHealth     = 0;
        float        playerDefense       = 0;

        // ENEMY
        string       enemyName           = "";
        int          enemyCurrentHealth  = 0;
        int          enemyMaxHealth      = 0;
        int          enemyAttack         = 0;
        int          enemyReward         = 0;
    
    public:
        DataPacket() {}
        
        DataPacket(int _client_id, int _sequence, functionType _function)
        {
            client_id      = _client_id;
            sequence       = _sequence;
            function       = _function;
        }

        DataPacket(int _client_id, int _sequence, functionType _function, int _currentRoom, int _maxRooms, int _playerMoney, int _playerKeys, roomType _roomGenerated, int _playerAttack, int _playerCurrentHealth, int _playerMaxHealth, float _playerDefense) : DataPacket(_client_id, _sequence, _function)
        {
            currentRoom         = _currentRoom;
            maxRooms            = _maxRooms;
            playerMoney         = _playerMoney;
            playerKeys          = _playerKeys;
            roomGenerated       = _roomGenerated;
            playerAttack        = _playerAttack;
            playerCurrentHealth = _playerCurrentHealth;
            playerMaxHealth     = _playerMaxHealth;
            playerDefense       = _playerDefense;
        }

        DataPacket(int _client_id, int _sequence, functionType _function, int _currentRoom, int _maxRooms, int _playerMoney, int _playerKeys, roomType _roomGenerated, int _playerAttack, int _playerCurrentHealth, int _playerMaxHealth, float _playerDefense, string _enemyName, int _enemyCurrentHealth, int _enemyMaxHealth, int _enemyAttack, int _enemyReward) : DataPacket(_client_id, _sequence, _function, _currentRoom, _maxRooms, _playerMoney, _playerKeys, _roomGenerated, _playerAttack, _playerCurrentHealth, _playerMaxHealth, _playerDefense)
        {
            enemyName           = _enemyName;
            enemyCurrentHealth  = _enemyCurrentHealth;
            enemyMaxHealth      = _enemyMaxHealth;
            enemyAttack         = _enemyAttack;
            enemyReward         = _enemyReward;
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
