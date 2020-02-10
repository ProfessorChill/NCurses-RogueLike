#ifndef CHEST_HPP
#define CHEST_HPP

#include <ncurses.h>
#include <random>
#include <string>
#include <vector>

#include "functions.hpp"
#include "item.hpp"

class Chest
{
    public:
	Chest();

	int x;
	int y;

	std::vector<Item> loot;

	void displayLoot(WINDOW *pWin);
};

#endif
