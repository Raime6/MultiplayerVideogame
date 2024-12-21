
#pragma once

#include "Enemy.hpp"

namespace VideoGameUDP
{
	class Demon : public VideoGameUDP::Enemy
	{

	public:

		// Builder
		Demon()
		{
			name = "Demon";
			maxHealth = 20;
			health = 20;
			attack = 4;
			reward = 20;
		}

		// Methods
		void Ability() override {};
	};
}