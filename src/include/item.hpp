#ifndef ITEM_HPP
#define ITEM_HPP

#include <ncurses.h>
#include <string>
#include <vector>

enum Type { ITEM, GOLD };

class Item
{
    public:
	Item(std::string iname, Type itype);
	Item(std::string iname, Type itype, int value);

	std::string name;
	int value;
	Type type;

	int y;
};

#endif
