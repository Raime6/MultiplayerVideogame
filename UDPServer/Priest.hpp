
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
			type	  = PRIEST;
			maxHealth = 10;
			health    = 10;
			attack    = 3;
			defense   = 0.3f;
		}

		// Methods
		void Ability() override
		{
			health += 2;
			if (health > maxHealth)
				health = maxHealth;
		}
	};
}
