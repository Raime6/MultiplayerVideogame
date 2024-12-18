
#pragma once

#include "Character.hpp"

namespace VideoGameUDP
{
	class Warrior: public VideoGameUDP::Character
	{

	public:

		// Builder
		Warrior()
		{
			maxHealth = 12;
			health    = 12;
			attack    = 2;
			defense   = 0.7f;
		}

		// Methods
		void Ability1() override {}
		void Ability2() override {}
	};
}
