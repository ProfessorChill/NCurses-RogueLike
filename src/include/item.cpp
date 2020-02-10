#include "item.hpp"

Item::Item(std::string iname, Type itype)
{
	name = iname;
	type = itype;
}

Item::Item(std::string iname, Type itype, int ivalue)
{
	name = iname;
	type = itype;
	value = ivalue;
}
