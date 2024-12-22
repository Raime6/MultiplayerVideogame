
#include "pch.h"
#include "VideoGame.hpp"



VideoGameUDP::VideoGame::VideoGame()
{
    character     = nullptr;
    maxRooms      = 4;
    currentRoom   = 1;
    playerMoney   = 0;
    playerKeys    = 0;
    roomGenerated = NOT_ROOM;
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

        case GENERATE_ROOM:
            generateRoom();
            break;

        case OPEN_CHEST:
            openChest();
            break;

        case LEAVE_ROOM:
            leaveRoom();
            break;

        case CHARACTER_ATTACK:
            character->Attack(enemy);
            if (enemy->health <= 0)
            {
                playerMoney += enemy->reward;

                if (character->attackBoosted)
                    character->attack -= 2;
                else if (character->healthBoosted)
                {
                    character->health -= 3;
                    character->maxHealth -= 3;
                }
                else if (character->defenseBoosted)
                    character->defense -= 0.2f;

                leaveRoom();
            }
            else
                character->health -= (int)round(enemy->getAttack() - (enemy->getAttack() * character->defense));

            break;

        case CHARACTER_ABILITY:
            character->Ability();
            
            if (character->isBlocking)
            {
                enemy->health -= enemy->getAttack();
                character->isBlocking = false;
            }
            else
                character->health -= (int)round(enemy->getAttack() - (enemy->getAttack() * character->defense));

            if (enemy->health <= 0)
            {
                playerMoney += enemy->reward;

                if (character->attackBoosted)
                    character->attack -= 2;
                else if (character->healthBoosted)
                {
                    character->health -= 3;
                    character->maxHealth -= 3;
                }
                else if (character->defenseBoosted)
                    character->defense -= 0.2f;

                leaveRoom();
            }

            if (character->type == MAGE)
            {
                if (currentRoom < maxRooms)
                    leaveRoom();
            }

            break;
    }

    return boolean;
}

void VideoGameUDP::VideoGame::healCharacter()
{
    character->health  = character->maxHealth;
    playerMoney       -= 10;
}

void VideoGameUDP::VideoGame::addKey()
{
    playerKeys++;
    playerMoney -= 5;
}

void VideoGameUDP::VideoGame::increaseDmg()
{
    if (!character->attackBoosted)
    {
        character->attack        += 2;
        character->attackBoosted  = true;
        playerMoney              -= 7;
    }
}

void VideoGameUDP::VideoGame::increaseHealth()
{
    if (!character->healthBoosted)
    {
        character->health        += 3;
        character->maxHealth     += 3;
        character->healthBoosted  = true;
        playerMoney              -= 7;
    }
}

void VideoGameUDP::VideoGame::increaseDef()
{
    if (!character->defenseBoosted)
    {
        character->defense        += 0.2f;
        character->defenseBoosted  = true;
        playerMoney               -= 7;
    }
}

void VideoGameUDP::VideoGame::generateRoom()
{
    int aux = rand() % 101;

    if (aux < 75 || currentRoom == maxRooms)
    {
        roomGenerated = ROOM_FIGHT;
        generateEnemy();
    }
    else
        roomGenerated = ROOM_CHEST;
}

void VideoGameUDP::VideoGame::generateEnemy()
{
    int aux = rand() % 2;

    if (currentRoom == maxRooms)
        enemy = new Demon();
    else if (aux == 0)
        enemy = new Skeleton();
    else
        enemy = new Slime();
}

void VideoGameUDP::VideoGame::openChest()
{
    playerKeys--;
    
    playerMoney += 10;
    
    leaveRoom();
}

void VideoGameUDP::VideoGame::leaveRoom()
{
    currentRoom++;
}
