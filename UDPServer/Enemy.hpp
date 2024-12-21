
#pragma once

#include <string>

namespace VideoGameUDP
{
	class Enemy
	{
	public:

		std::string name      = "";
		int	        maxHealth = 0;
		int         health    = 0;
		int         attack    = 0;
		int         reward    = 0;

	public:

		int getAttack()
		{
			return attack;
		}

		virtual void Ability() = 0;
	};
}