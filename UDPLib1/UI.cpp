
#include "pch.h"
#include "UI.h"



int UI::selectOptionMenu()
{
	int optionMenu;

	std::cout << "Option selected: ";
	std::cin >> optionMenu;
	std::cout << std::endl << std::endl;

	return optionMenu;
}



// MAIN MENU

int UI::showMainMenu()
{
	std::cout << "Select an option:" << std::endl;
	std::cout << "1. New Game"		 << std::endl;
	std::cout << "2. Exit"			 << std::endl;
	std::cout << std::endl;

	return UI::selectOptionMenu();
}



// SELECT CHARACTER

int UI::showSelectCharacter()
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
