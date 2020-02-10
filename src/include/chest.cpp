#include "chest.hpp"

std::vector<Item> ilist = {
    Item("Gloves", ITEM),
    Item("Pauldrons", ITEM),
};

Chest::Chest()
{
	int randItems = Function::getRandNum(0, ilist.size());
	int randGold = Function::getRandNum(0, 5);

	for (int i = 0; i < randItems; ++i) {
		loot.push_back(ilist[i]);
	}

	for (int i = 0; i < randGold; ++i) {
		std::string goldString = " Gold";
		int goldAmt = Function::getRandNum(1, 500);

		goldString = std::to_string(goldAmt) + goldString;

		loot.push_back(Item(goldString, GOLD, goldAmt));
	}
}

void Chest::displayLoot(WINDOW *pWin)
{
	mvwprintw(pWin, 1, 1, "Loot Window");

	wattron(pWin, A_REVERSE);
	for (int x = 0; x < loot.size(); ++x) {
		mvwprintw(pWin, x + 2, 1, loot[x].name.c_str());
		loot[x].y = x + 2;
	}
	wattroff(pWin, A_REVERSE);

	wrefresh(pWin);
}
