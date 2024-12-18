
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
			health  = 12;
			attack  = 2;
			defense = 0.7f;
		}

		// Methods
		void Ability1() override {}
		void Ability2() override {}
		void Ability3() override {}
		void Ability4() override {}
	};
}
