
#pragma once

#include "Enemy.hpp"

namespace VideoGameUDP
{
	class Skeleton : public VideoGameUDP::Enemy
	{

	public:

		// Builder
		Skeleton()
		{
			name        = "Skeleton";
			maxHealth   = 7;
			health      = 7;
			attack      = 3;
			reward      = 5;
		}

		// Methods
		void Ability() override {};
	};
}