
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
			defense   = 0.4f;
		}

		// Methods
		void Ability() override {}
	};
}
