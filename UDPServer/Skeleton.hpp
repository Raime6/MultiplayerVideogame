
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
			defense     = 0.1f;
		}

		// Methods
		void Attack()  override {};
		void Ability() override {};
	};
}