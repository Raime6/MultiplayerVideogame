
#pragma once

#include "Character.hpp"

namespace UserCharacter
{
	class Warrior: public UserCharacter::Character
	{

	public:

		// Builder
		Warrior()
		{
			health  = 12;
			attack  = 2;
			defense = 0.7f;
		}

		// Methods
		void Ability1() override {}
		void Ability2() override {}
		void Ability3() override {}
		void Ability4() override {}
		void print() override { std::cout << "Hola, soy un Warrior" << std::endl; }
	};
}
