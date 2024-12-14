
#include "pch.h"
#include "UI.hpp"



int Interface::UI::selectOptionMenu()
{
	int optionMenu;

	std::cout << "Option selected: ";
	std::cin  >> optionMenu;
	std::cout << std::endl << std::endl;

	return optionMenu;
}



// MAIN MENU

int Interface::UI::showMainMenu()
{
	std::cout << "Select an option:" << std::endl;
	std::cout << "1. New Game"		 << std::endl;
	std::cout << "2. Exit"			 << std::endl;
	std::cout << std::endl;

	return UI::selectOptionMenu();
}



// SELECT CHARACTER

int Interface::UI::showSelectCharacter()
{
	std::cout << "Select an option:" << std::endl;
	std::cout << "1. Warrior"        << std::endl;
	std::cout << "	- HP : 12"       << std::endl;
	std::cout << "	- Atk:  2"       << std::endl;
	std::cout << "	- Def:  7"       << std::endl << std::endl;

	std::cout << "2. Mage"           << std::endl;
	std::cout << "	- HP :  7"       << std::endl;
	std::cout << "	- Atk:  5"       << std::endl;
	std::cout << "	- Def:  2"       << std::endl << std::endl;

	std::cout << "3. Priest"         << std::endl;
	std::cout << "	- HP : 10"       << std::endl;
	std::cout << "	- Atk:  3"       << std::endl;
	std::cout << "	- Def:  5"       << std::endl;
	std::cout << std::endl;

	return UI::selectOptionMenu();
}



// NEW GAME

int Interface::UI::showNewGameStart()
{
	std::cout << "The city of Aelthar was once a place of splendor, where magic and wisdom thrived under the rule of a council of mages." << std::endl;
	std::cout << "However, beneath its bright facade, something dark lurks. An ancient primordial demon was sealed away centuries ago in" << std::endl;
	std::cout << "the depths of the city, but its power never disappeared. Fed by the suffering of the poor who dwell in the dark and"    << std::endl;
	std::cout << "filthy alleyways of the Lower City, its influence has begun to corrupt even the most powerful. Bloodthirsty cults have" << std::endl;
	std::cout << "risen, and the mages, once protectors, now study and experiment with the malignant force in their desperate quest to"   << std::endl;
	std::cout << "understand it."																										  << std::endl << std::endl;
	std::cout << "Your mission is clear: as a mercenary sent by the council, you must investigate the sealed crypt where this ancient"    << std::endl;
	std::cout << "evil lies. What you don’t know is that as you delve into the city's dark heart, you will not only face monsters and"    << std::endl;
	std::cout << "traps, but uncover secrets that could forever change the fate of Aelthar. Will you be able to stop the growing"         << std::endl;
	std::cout << "darkness, or will you succumb to its corruption? The answer lies in the depths of the Abyss of Yggrath."                << std::endl << std::endl;

	std::cout << "Select an option:"    << std::endl;
	std::cout << "1. Enter the dungeon" << std::endl;
	std::cout << std::endl;

	return UI::selectOptionMenu();
}
