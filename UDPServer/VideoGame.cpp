
#include "pch.h"
#include "VideoGame.hpp"



VideoGameUDP::VideoGame::VideoGame()
{
    character   = nullptr;
    maxRooms    = 4;
    currentRoom = 1;
}



bool VideoGameUDP::VideoGame::videoGameFun(int function)
{
    bool       boolean = true;

    switch (function)
    {
    case EXIT_GAME:
        boolean = false;
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

    return boolean;
}
