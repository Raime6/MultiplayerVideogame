
#pragma once

#include "Warrior.hpp"
#include "Mage.hpp"
#include "Priest.hpp"

using namespace UserCharacter;

namespace ServerVideoGame
{	
	typedef class VideoGame
	{

	public:

		VideoGame() = default;

		void exitGame(bool);

		// User Character
		Warrior createWarrior();
		Mage    createMage();
		Priest  createPriest();
	};



	// Inline implementations
	inline void exitGame(bool serve)
	{
		serve = false;
	}

	inline Warrior createWarrior()
	{
		Warrior newWarrior;
		return  newWarrior;
	}

	inline Mage createMage()
	{
		Mage   newMage;
		return newMage;
	}

	inline Priest createPriest()
	{
		Priest newPriest;
		return newPriest;
	}
}
