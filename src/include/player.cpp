#include "player.hpp"

Player::Player(WindowInit statWinInit, WindowInit invWinInit)
{
	x = 5;
	y = 5;

	// Create the status window
	statusWindow = newwin(statWinInit.h, statWinInit.w, statWinInit.yPos,
			      statWinInit.xPos);
	box(statusWindow, 0, 0);
	wrefresh(statusWindow);

	// Create the inventory window
	inventoryWindow = newwin(invWinInit.h, invWinInit.w, invWinInit.yPos,
				 invWinInit.xPos);
	box(inventoryWindow, 0, 0);
	mvwprintw(inventoryWindow, 1, 1, "Inventory");
	wrefresh(inventoryWindow);
}

void Player::setSpawnPos(std::vector<std::vector<char>> &chrs, int maxX,
			 int maxY)
{
	int ySize = chrs.size();
	bool isSet = false;

	for (int i = 0; i < ySize; ++i) {
		int xSize = chrs[y].size();

		for (int j = 0; j < xSize; ++j) {
			if (chrs[i][j] == '.') {
				x = i;
				y = j;
				isSet = true;
				break;
			}
		}

		if (isSet) {
			break;
		}
	}

	if (y >= maxY - 1 || x >= maxX - 1) {
		y = 0;
		x = 0;
	}
}

void Player::drawStatus()
{
	werase(statusWindow);
	box(statusWindow, 0, 0);

	std::string xPos = "X: ";
	xPos += std::to_string(x);

	std::string yPos = "Y: ";
	yPos += std::to_string(y);

	std::string levelString = "Level:\t\t";
	levelString += std::to_string(level);

	std::string posString = xPos + " " + yPos;

	std::string goldString = "Gold:\t\t" + std::to_string(gold);

	// Player Name Position
	mvwprintw(statusWindow, 0, 2, name.c_str());

	// Table Row One
	mvwprintw(statusWindow, 1, 1, posString.c_str());
	mvwprintw(statusWindow, 2, 1, goldString.c_str());
	mvwprintw(statusWindow, 3, 1, levelString.c_str());
	mvwprintw(statusWindow, 4, 1, healthString.c_str());
	mvwprintw(statusWindow, 5, 1, staminaString.c_str());

	// Table Row Two
	mvwprintw(statusWindow, 1, 30, "Hunger:\tFull");
	mvwprintw(statusWindow, 2, 30, "Thirst:\tQuenched");
	mvwprintw(statusWindow, 3, 30, "Sleep:\tAwake");
	mvwprintw(statusWindow, 4, 30, "Status:\tNormal");
}

void Player::drawInventory()
{
	mvwprintw(inventoryWindow, 1, 1, "Inventory");

	wattron(inventoryWindow, A_REVERSE);
	for (int x = 0; x < inventory.size(); ++x) {
		mvwprintw(inventoryWindow, x + 2, 1, inventory[x].name.c_str());
	}
	wattroff(inventoryWindow, A_REVERSE);

	wrefresh(inventoryWindow);
}

void Player::draw(WINDOW *pWin, int centerX, int centerY)
{
	wattron(pWin, COLOR_PAIR(3));
	mvwaddch(pWin, centerY, centerX, 'O');
	wattroff(pWin, COLOR_PAIR(3));

	box(statusWindow, 0, 0);

	drawStatus();

	wrefresh(statusWindow);
}

void Player::reportChoice(int x, int y)
{
	mvwprintw(statusWindow, 3, 60, "SOMETHING");
}
