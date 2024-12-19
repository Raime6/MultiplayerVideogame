
#pragma once

namespace VideoGameUDP
{
	class Enemy
	{
	public:

		int	  maxHealth = 0;
		int   health = 0;
		int   attack = 0;
		float defense = 0;

	public:

		virtual void Attack()   = 0;
		virtual void Ability2() = 0;
	};
}
