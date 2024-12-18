
#pragma once

namespace VideoGameUDP
{
	enum clientGameState
	{
		STATE_EXIT_GAME,
		STATE_MAIN_MENU,
		STATE_SELECTION_CHARACTER,
		STATE_NEW_GAME_START
	};

	

	class UI
	{

	public:

		UI() = default;

		clientGameState UIFun(clientGameState, functionType&);
		
		void errorMessage();

		// Save Player's option
		int selectOptionMenu(int, int);

		// MAIN MENU
		int showMainMenu();

		// SELECT CHARACTER
		int showSelectCharacter();

		// NEW GAME
		int showNewGameStart();
	};
}
