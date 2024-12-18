
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

	public:

		VideoGame();
		
		bool videoGameFun(int);
	};
}
