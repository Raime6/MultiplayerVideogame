
#pragma once

#include "Character.hpp"

namespace VideoGameUDP
{
	class Mage : public VideoGameUDP::Character
	{

	public:

		// Builder
		Mage()
		{
			type      = MAGE;
			maxHealth = 7;
			health    = 7;
			attack    = 4;
			defense   = 0.2f;
		}

		// Methods
		void Ability() override {}
	};
}
