
#pragma once

namespace VideoGameUDP
{
	enum clientGameState
	{
		STATE_EXIT_GAME,
		STATE_MAIN_MENU,
		STATE_SELECTION_CHARACTER,
		STATE_NEW_GAME_START,
		STATE_ROOM_SELECTION,
		STATE_SHOP,
		STATE_ROOM
	};

	

	class UI
	{

	public:

		UI() = default;

		clientGameState UIFun(PDataPacket, clientGameState, functionType&);
		
		void errorMessage();

		// Save Player's option
		int selectOptionMenu(int, int);

		// MAIN MENU
		int showMainMenu();

		// SELECT CHARACTER
		int showSelectCharacter();

		// NEW GAME
		int showNewGameStart();

		// DUNGEON INTERFACE
		int showDungeonInterface(int, int);

		// SHOP INTERFACE
		int showShopInterface(shopItemType*, int*, int);
		functionType shopItemFunction(shopItemType);

		// ROOM
		int showRoomFightInterface(std::string);
		int showRoomChestInterface(int, int, int);
	};
}
