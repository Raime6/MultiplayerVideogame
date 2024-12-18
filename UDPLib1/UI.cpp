
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
					function = NOT_FUNCTION;
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
			switch (showDungeonInterface(response->minRooms, response->maxRooms))
			{
			case 1:
				gameState = STATE_ROOM_SELECTION;
				function  = NOT_FUNCTION;
				break;

			case 2:
				gameState = STATE_ROOM_SELECTION;
				function  = NOT_FUNCTION;
				break;
			
			case 3:
				gameState = STATE_MAIN_MENU;
				function  = NOT_FUNCTION;
				break;
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
	std::cout << "1. New Game"		 << std::endl;
	std::cout << "2. Exit"			 << std::endl;
	std::cout << std::endl;

	return UI::selectOptionMenu(1, 2);
}



// SELECT CHARACTER

int VideoGameUDP::UI::showSelectCharacter()
{
	std::cout << "========================================================================================================================" << std::endl << std::endl;

	std::cout << "Select an option:"	  << std::endl;
	std::cout << "1. Warrior"			  << std::endl;
	std::cout << "	- HP : 12"			  << std::endl;
	std::cout << "	- Atk:  2"			  << std::endl;
	std::cout << "	- Def:  7"			  << std::endl << std::endl;

	std::cout << "2. Mage"				  << std::endl;
	std::cout << "	- HP :  7"			  << std::endl;
	std::cout << "	- Atk:  5"			  << std::endl;
	std::cout << "	- Def:  2"			  << std::endl << std::endl;

	std::cout << "3. Priest"			  << std::endl;
	std::cout << "	- HP : 10"			  << std::endl;
	std::cout << "	- Atk:  3"			  << std::endl;
	std::cout << "	- Def:  5"			  << std::endl << std::endl;

	std::cout << "4. Return to Main Menu" << std::endl;
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
	std::cout << "understand it."																										    << std::endl << std::endl;
	std::cout << "Your mission is clear: as a mercenary sent by the council, you must investigate the sealed crypt where this ancient"      << std::endl;
	std::cout << "evil lies. What you don’t know is that as you delve into the city's dark heart, you will not only face monsters and"      << std::endl;
	std::cout << "traps, but uncover secrets that could forever change the fate of Aelthar. Will you be able to stop the growing"           << std::endl;
	std::cout << "darkness, or will you succumb to its corruption? The answer lies in the depths of the Abyss of Yggrath."                  << std::endl << std::endl;

	std::cout << "Select an option:"                << std::endl;
	std::cout << "1. Enter the dungeon"             << std::endl;
	std::cout << "2. Return to character selection" << std::endl;
	std::cout << std::endl;

	return UI::selectOptionMenu(1, 2);
}



// DUNGEON INTERFACE

int VideoGameUDP::UI::showDungeonInterface(int currentRoom, int maxRoom)
{
	std::cout << "========================================================================================================================" << std::endl << std::endl;

	std::cout << "Room " << currentRoom << " / " << maxRoom << std::endl << std::endl;
	
	std::cout << "Select an option:"      << std::endl;
	std::cout << "1. Enter the room"      << std::endl;
	std::cout << "2. Enter the shop"      << std::endl;
	std::cout << "3. Return to Main Menu" << std::endl;
	std::cout << std::endl;

	return UI::selectOptionMenu(1, 3);
}
