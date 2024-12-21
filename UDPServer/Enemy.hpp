
#pragma once

#include <string>

#include "Character.hpp"

namespace VideoGameUDP
{
	class Enemy
	{
	public:

		std::string name      = "";
		int	        maxHealth = 0;
		int         health    = 0;
		int         attack    = 0;
		int         reward    = 0;

	public:

		/*void Attack(Character* character)
		{
			character->health -= (int) attack * character->defense;
		}*/

		virtual void Ability() = 0;
	};
}