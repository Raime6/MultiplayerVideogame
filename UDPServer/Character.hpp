
#pragma once

namespace VideoGameUDP
{
	class Character
	{
	protected:

		int   health  = 0;
		int   attack  = 0;
		float defense = 0;

	public:

		virtual void Ability1() = 0;
		virtual void Ability2() = 0;
		virtual void Ability3() = 0;
		virtual void Ability4() = 0;
	};
}
