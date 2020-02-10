#ifndef CONTEXT_DIALOG_HPP
#define CONTEXT_DIALOG_HPP

#include <ncurses.h>
#include <string>
#include <vector>

class ContextDialog
{
    public:
	ContextDialog();

	std::vector<std::string> options;
};

#endif
