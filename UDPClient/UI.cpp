
#include "pch.h"
#include "UI.hpp"



VideoGameUDP::clientGameState VideoGameUDP::UI::UIFun(PDataPacket response, clientGameState gameState, functionType& function)
{
	switch (gameState)
	{
	case STATE_MAIN_MENU:
		switch (showMainMenu())
		{
			case 1:
				gameState = STATE_SELECTION_CHARACTER;
				break;

			case 2:
				gameState = STATE_EXIT_GAME;
				function  = EXIT_GAME;
				break;
		}
		break;

	case STATE_SELECTION_CHARACTER:
		switch (showSelectCharacter())
		{
			case 1:
				gameState = STATE_NEW_GAME_START;
				function  = CREATE_WARRIOR;
				break;

			case 2:
				gameState = STATE_NEW_GAME_START;
				function  = CREATE_MAGE;
				break;

			case 3:
				gameState = STATE_NEW_GAME_START;
				function  = CREATE_PRIEST;
				break;

			case 4:
				gameState = STATE_MAIN_MENU;
				function  = NOT_FUNCTION;
				break;
		}
		break;

	case STATE_NEW_GAME_START:
		switch (showNewGameStart())
		{
			case 1:
				gameState = STATE_ROOM_SELECTION;
				function  = RETURN_ROOMS;
				break;

			case 2:
				gameState = STATE_SELECTION_CHARACTER;
				function  = NOT_FUNCTION;
				break;
		}
		break;

	case STATE_ROOM_SELECTION:

		switch (showDungeonInterface(response->currentRoom, response->maxRooms))
		{
			case 1:
				gameState = STATE_ROOM;
				function  = GENERATE_ROOM;
				break;

			case 2:
				gameState = STATE_SHOP;
				function  = GENERATE_SHOP;
				break;

			case 3:
				gameState = STATE_MAIN_MENU;
				function  = NOT_FUNCTION;
				break;
		}
		break;

	case STATE_SHOP:

		switch (showShopInterface(response->shopItems, response->shopItemCosts, response->playerMoney))
		{
			case 1:
				if (response->playerMoney < response->shopItemCosts[response->shopItems[0]])
				{
					gameState = STATE_SHOP;
					function  = NOT_FUNCTION;
					std::cout << "Not enough money!" << std::endl;
				}
				else
				{
					gameState = STATE_ROOM_SELECTION;
					function  = shopItemFunction(response->shopItems[0]);
				}
				break;

			case 2:
				if (response->playerMoney < response->shopItemCosts[response->shopItems[1]])
				{
					gameState = STATE_SHOP;
					function  = NOT_FUNCTION;
					std::cout << "Not enough money!" << std::endl << std::endl;
				}
				else
				{
					gameState = STATE_ROOM_SELECTION;
					function  = shopItemFunction(response->shopItems[1]);
				}
				break;

			case 3:
				if (response->playerMoney < response->shopItemCosts[response->shopItems[2]])
				{
					gameState = STATE_SHOP;
					function  = NOT_FUNCTION;
					std::cout << "Not enough money!" << std::endl << std::endl;
				}
				else
				{
					gameState = STATE_ROOM_SELECTION;
					function  = shopItemFunction(response->shopItems[2]);
				}
				break;

			case 4:
				gameState = STATE_ROOM_SELECTION;
				function = NOT_FUNCTION;
				break;
		}
		break;

	case STATE_ROOM:
		if (response->roomGenerated == ROOM_FIGHT)
		{
			/*switch (showRoomFightInterface(response->enemyName, response->enemyCurrentHealth, response->enemyMaxHealth, response->playerCurrentHealth, response->playerMaxHealth))
			{
			case 1:
				gameState = STATE_ROOM;
				function  = NOT_FUNCTION;
				break;

			case 2:
				gameState = STATE_ROOM;
				function  = NOT_FUNCTION;
				break;

			case 3:
				gameState = STATE_MAIN_MENU;
				function  = NOT_FUNCTION;
				break;
			}*/
		}
		else
		{
			switch (showRoomChestInterface(response->playerKeys))
			{
				case 1:
					if (response->playerKeys <= 0)
					{
						gameState = STATE_ROOM;
						function  = NOT_FUNCTION;
						std::cout << "You don't have any keys!" << std::endl << std::endl;
					}
					else
					{
						gameState = STATE_ROOM_SELECTION;
						function  = OPEN_CHEST;
						std::cout << "You found 10 gold!" << std::endl << std::endl;
					}

					break;

				case 2:
					gameState = STATE_ROOM_SELECTION;
					function  = LEAVE_ROOM;
					break;
			}
		}

		break;
	}

	return gameState;
}



void VideoGameUDP::UI::errorMessage()
{
	std::cout << "Not valid option. Please select a valid option." << std::endl;
}



// Save Player's option

int VideoGameUDP::UI::selectOptionMenu(int minOption, int maxOption)
{
	int optionMenu;

	do
	{
		std::cout << "Option selected: ";
		std::cin  >> optionMenu;

	} while (optionMenu < minOption || optionMenu > maxOption);

	system("cls");

	return optionMenu;
}



// MAIN MENU

int VideoGameUDP::UI::showMainMenu()
{
	std::cout << "========================================================================================================================" << std::endl << std::endl;

	std::cout << "Select an option:" << std::endl;
	std::cout << "1. New Game"       << std::endl;
	std::cout << "2. Exit"           << std::endl;
	std::cout << std::endl;

	return UI::selectOptionMenu(1, 2);
}



// SELECT CHARACTER

int VideoGameUDP::UI::showSelectCharacter()
{
	std::cout << "========================================================================================================================" << std::endl << std::endl;

	std::cout << "Select a character for your adventure:" << std::endl << std::endl;
	std::cout << "1. Warrior"                             << std::endl;
	std::cout << "	- HP : 12"                            << std::endl;
	std::cout << "	- Atk:  2"                            << std::endl;
	std::cout << "	- Def:  7"                            << std::endl << std::endl;

	std::cout << "2. Mage"                                << std::endl;
	std::cout << "	- HP :  7"                            << std::endl;
	std::cout << "	- Atk:  5"                            << std::endl;
	std::cout << "	- Def:  2"                            << std::endl << std::endl;

	std::cout << "3. Priest"                              << std::endl;
	std::cout << "	- HP : 10"                            << std::endl;
	std::cout << "	- Atk:  3"                            << std::endl;
	std::cout << "	- Def:  5"                            << std::endl << std::endl;

	std::cout << "4. Return to Main Menu"                 << std::endl;
	std::cout << std::endl;

	return UI::selectOptionMenu(1, 4);
}



// NEW GAME

int VideoGameUDP::UI::showNewGameStart()
{
	std::cout << "========================================================================================================================" << std::endl << std::endl;

	std::cout << "The city of Aelthar was once a place of splendor, where magic and wisdom thrived under the rule of a council of mages."   << std::endl;
	std::cout << "However, beneath its bright facade, something dark lurks. An ancient primordial demon was sealed away centuries ago in"   << std::endl;
	std::cout << "the depths of the city, but its power never disappeared. Fed by the suffering of the poor who dwell in the dark and"      << std::endl;
	std::cout << "filthy alleyways of the Lower City, its influence has begun to corrupt even the most powerful. Bloodthirsty cults have"   << std::endl;
	std::cout << "risen, and the mages, once protectors, now study and experiment with the malignant force in their desperate quest to"     << std::endl;
	std::cout << "understand it." << std::endl << std::endl;
	std::cout << "Your mission is clear: as a mercenary sent by the council, you must investigate the sealed crypt where this ancient"      << std::endl;
	std::cout << "evil lies. What you don’t know is that as you delve into the city's dark heart, you will not only face monsters and"      << std::endl;
	std::cout << "traps, but uncover secrets that could forever change the fate of Aelthar. Will you be able to stop the growing"           << std::endl;
	std::cout << "darkness, or will you succumb to its corruption? The answer lies in the depths of the Abyss of Yggrath."                  << std::endl << std::endl;

	std::cout << "Select an option:"																									    << std::endl;
	std::cout << "1. Enter the dungeon"																										<< std::endl;
	std::cout << "2. Return to character selection"																							<< std::endl;
	std::cout << std::endl;

	return UI::selectOptionMenu(1, 2);
}



// DUNGEON INTERFACE

int VideoGameUDP::UI::showDungeonInterface(int currentRoom, int maxRoom)
{
	std::cout << "========================================================================================================================" << std::endl << std::endl;

	std::cout << "Room " << currentRoom << " / " << maxRoom << std::endl << std::endl;

	std::cout << "Select an option:"      << std::endl;
	std::cout << "1. Enter the room"	  << std::endl;
	std::cout << "2. Enter the shop"	  << std::endl;
	std::cout << "3. Return to Main Menu" << std::endl;
	std::cout << std::endl;

	return UI::selectOptionMenu(1, 3);
}



// SHOP INTERFACE

int VideoGameUDP::UI::showShopInterface(shopItemType *shopItems, int *shopItemCosts, int playerMoney)
{
	int i;

	std::cout << "========================================================================================================================" << std::endl << std::endl;

	std::cout << "Welcome to my shop!" << std::endl;
	std::cout << "Please feel free to buy whatever you want " << std::endl << std::endl;

	std::cout << "Your money: " << playerMoney << std::endl << std::endl;

	std::cout << "Select an item to buy:" << std::endl;
	for (i = 0; i < 3; i++)
	{
		std::cout << i + 1 << ". ";

		switch (*(shopItems + i))
		{
			case HEAL_POTION:
				std::cout << "Heal potion       - " << *(shopItemCosts + *(shopItems + i)) << "g (restores all chater's health)"     << std::endl;
				break;

			case KEY:
				std::cout << "Key               - " << *(shopItemCosts + *(shopItems + i)) << "g (needed to open chests)"            << std::endl;
				break;

			case STRENGTH_FLASK:
				std::cout << "Strengh Flask     - " << *(shopItemCosts + *(shopItems + i)) << "g (+2 DMG during the next combat)"    << std::endl;
				break;

			case VIGOR_FLASK:
				std::cout << "Vigor Flask       - " << *(shopItemCosts + *(shopItems + i)) << "g (+3 HEALTH during the next combat)" << std::endl;
				break;

			case ENDURANCE_FLASK:
				std::cout << "Endurance Flask   - " << *(shopItemCosts + *(shopItems + i)) << "g (+0.2 DEF during the next combat)"  << std::endl;
				break;
		}
	}
	std::cout << i + 1 << ". Leave Shop" << std::endl << std::endl;

	return UI::selectOptionMenu(1, 4);
}

functionType VideoGameUDP::UI::shopItemFunction(shopItemType shopItem)
{
	functionType function = NOT_FUNCTION;

	switch (shopItem)
	{
	case HEAL_POTION:
		function = HEAL_CHARACTER;
		break;

	case KEY:
		function = ADD_KEY;
		break;

	case STRENGTH_FLASK:
		function = INCREASE_STRENGHT;
		break;

	case VIGOR_FLASK:
		function = INCREASE_VIGOR;
		break;

	case ENDURANCE_FLASK:
		function = INCREASE_ENDURANCE;
		break;
	}

	return function;
}



// ROOM

// Fight

int VideoGameUDP::UI::showRoomFightInterface(std::string enemyName, int enemyCurrentHealth, int enemyMaxHealth, int playerCurrentHealth, int playerMaxHealth)
{
	std::cout << "========================================================================================================================" << std::endl << std::endl;

	std::cout << "A " << enemyName << "is attacking!" << std::endl << std::endl;

	std::cout << "HP: " << enemyCurrentHealth  << "/" << enemyMaxHealth << std::endl << std::endl;
	std::cout << "--------------------"        << std::endl << std::endl;
	std::cout << "HP: " << playerCurrentHealth << "/" << playerMaxHealth << std::endl << std::endl;

	std::cout << "Choose an action:"    << std::endl;
	std::cout << "1. Ability 1"         << std::endl;
	std::cout << "2. Ability 2"         << std::endl;
	std::cout << "3. Exit to Main Menu" << std::endl;
	std::cout << std::endl;

	return UI::selectOptionMenu(1, 3);
}

// Chest

int VideoGameUDP::UI::showRoomChestInterface(int playerKeys)
{
	std::cout << "========================================================================================================================" << std::endl << std::endl;

	std::cout << "You have found a locked chest!" << std::endl;

	std::cout << "Your keys: " << playerKeys      << std::endl << std::endl;

	std::cout << "Select an option:"              << std::endl;
	std::cout << "1. Use a key to unlock it"      << std::endl;
	std::cout << "2. Leave the room"              << std::endl;
	std::cout << std::endl;

	return UI::selectOptionMenu(1, 2);
}
