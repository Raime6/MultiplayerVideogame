
#pragma once

#include "Enemy.hpp"

namespace VideoGameUDP
{
	class Slime : public VideoGameUDP::Enemy
	{

	public:

		// Builder
		Slime()
		{
			name        = "Slime";
			maxHealth   = 10;
			health      = 10;
			attack      = 2;
			reward      = 7;
		}

		// Methods
		void Ability() override {};
	};
}