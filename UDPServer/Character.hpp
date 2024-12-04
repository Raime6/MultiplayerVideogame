
#pragma once

namespace UserCharacter
{
	typedef class Character
	{
	protected:

		int   health;
		int   attack;
		float defense;

	public:

		virtual void Ability1() = 0;
		virtual void Ability2() = 0;
		virtual void Ability3() = 0;
		virtual void Ability4() = 0;
	};
}
