
#pragma once

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

		virtual void Ability1() = 0;
		virtual void Ability2() = 0;
	};
}
