
#pragma once

#include "Enemy.hpp"

namespace VideoGameUDP
{
	class Character
	{
	public:

		int	  maxHealth     = 0;
		int   health        = 0;
		int   attack        = 0;
		float defense       = 0;

		bool healthBoosted  = false;
		bool attackBoosted  = false;
		bool defenseBoosted = false;

	public:

		void Attack(Enemy* enemy)
		{
			enemy->health -= attack;
		}

		virtual void Ability() = 0;
	};
}
