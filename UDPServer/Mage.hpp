
#pragma once

#include "Character.hpp"

namespace UserCharacter
{
	typedef class Mage : public UserCharacter::Character
	{

	public:

		// Builder
		Mage()
		{
			health = 7;
			attack = 5;
			defense = 0.2f;
		}

		// Methods
		void Ability1() override {}
		void Ability2() override {}
		void Ability3() override {}
		void Ability4() override {}
		void print() override { std::cout << "Hola, soy un Mage" << std::endl; }
	};
}
