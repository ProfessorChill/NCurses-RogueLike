#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <ncurses.h>
#include <string>
#include <vector>

#include "item.hpp"

struct WindowInit {
	int w;
	int h;
	int xPos;
	int yPos;
};

class Player
{
    public:
	Player(WindowInit statWinInit, WindowInit invWinInit);

	void setSpawnPos(std::vector<std::vector<char>> &chrs, int maxX,
			 int maxY);
	void draw(WINDOW *pWin, int centerX, int centerY);

	void reportChoice(int x, int y);

	void drawInventory();

	int x;
	int y;
	int gold;

	WINDOW *statusWindow;
	WINDOW *inventoryWindow;

	std::vector<Item> inventory;

    private:
	std::string name = "Luna";

	int health = 100;
	int stamina = 100;
	int level = 1;

	std::string healthString = "Health:\t100";
	std::string staminaString = "Stamina:\t100";

	void drawStatus();
};

#endif
