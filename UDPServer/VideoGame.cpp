
#include "pch.h"
#include "VideoGame.hpp"



VideoGameUDP::VideoGame::VideoGame()
{
    character   = nullptr;
    maxRooms    = 4;
    currentRoom = 1;
    playerMoney = 0;
    numKeys     = 0;
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

    case HEAL_CHARACTER:
        healCharacter();
        break;

    case ADD_KEY:
        addKey();
        break;

    case INCREASE_STRENGHT:
        increaseDmg();
        break;

    case INCREASE_VIGOR:
        increaseHealth();
        break;

    case INCREASE_ENDURANCE:
        increaseDef();
        break;
    }

    return boolean;
}

void VideoGameUDP::VideoGame::healCharacter()
{
    character->health = character->maxHealth;
}

void VideoGameUDP::VideoGame::addKey()
{
    numKeys++;
}

void VideoGameUDP::VideoGame::increaseDmg()
{
    character->attack        += 2;
    character->attackBoosted  = true;
}

void VideoGameUDP::VideoGame::increaseHealth()
{
    character->health        += 3;
    character->healthBoosted  = true;
}

void VideoGameUDP::VideoGame::increaseDef()
{
    character->defense        += 0.2f;
    character->defenseBoosted  = true;
}
