
#pragma once

#include "Warrior.hpp"
#include "Mage.hpp"
#include "Priest.hpp"

namespace VideoGameUDP
{	
	class VideoGame
	{
	public:

		Character* character;
		int        maxRooms;
		int        currentRoom;
		int        playerMoney;
		int        numKeys;

	public:

		VideoGame();
		
		bool videoGameFun(int);

		// SHOP
		void healCharacter();
		void addKey();
		void increaseDmg();
		void increaseHealth();
		void increaseDef();
	};
}
