
#pragma once

namespace Interface
{
	typedef class UI
	{

	public:

		UI() = default;

		int  selectOptionMenu();

		// MAIN MENU
		int showMainMenu();

		// SELECT CHARACTER
		int showSelectCharacter();
	};
}
