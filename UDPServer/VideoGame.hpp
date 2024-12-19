
#pragma once

#include "Warrior.hpp"
#include "Mage.hpp"
#include "Priest.hpp"
#include "Skeleton.hpp"
#include "Slime.hpp"

namespace VideoGameUDP
{	
	class VideoGame
	{
	public:

		Character* character;
		Enemy*     enemy;
		int        maxRooms;
		int        currentRoom;
		int        playerMoney;
		int        playerKeys;
		roomType   roomGenerated;

	public:

		VideoGame();
		
		bool videoGameFun(int);

		// SHOP
		void healCharacter();
		void addKey();
		void increaseDmg();
		void increaseHealth();
		void increaseDef();
		void generateRoom();
		void generateEnemy();
		void openChest();
		void leaveRoom();
	};
}
