
#pragma once

#include "Character.hpp"

namespace VideoGameUDP
{
	class Priest : public VideoGameUDP::Character
	{

	public:

		// Builder
		Priest()
		{
			health  = 10;
			attack  = 3;
			defense = 0.5f;
		}

		// Methods
		void Ability1() override {}
		void Ability2() override {}
		void Ability3() override {}
		void Ability4() override {}
	};
}
