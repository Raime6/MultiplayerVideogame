
#pragma once

namespace Interface
{
	class UI
	{

	public:

		UI() = default;

		int  selectOptionMenu();

		// MAIN MENU
		int showMainMenu();

		// SELECT CHARACTER
		int showSelectCharacter();

		// NEW GAME
		int showNewGameStart();
	};
}
