#ifndef DUNGEON_GENERATOR_HPP
#define DUNGEON_GENERATOR_HPP

#include <ncurses.h>
#include <vector>

#include "chest.hpp"
#include "functions.hpp"

struct Coord {
	int x;
	int y;
};

struct Box {
	int x1;
	int x2;
	int y1;
	int y2;
	bool drin;
};

struct UniqueItem {
	Coord pos;
	Chest chest;
};

bool checkIntersection(std::vector<Box> &bxs, Box &bx)
{
	int bxSize = bxs.size();

	for (int i = 0; i < bxSize; ++i) {
		int gap = Function::getRandNum(1, 5);

		if (((bx.x1 >= bxs[i].x1 - gap && bx.x1 <= bxs[i].x2 + gap) ||
		     (bx.x2 >= bxs[i].x1 - gap && bx.x2 <= bxs[i].x2 + gap)) &&
		    ((bx.y1 >= bxs[i].y1 - gap && bx.y1 <= bxs[i].y2 + gap) ||
		     (bx.y2 >= bxs[i].y1 - gap && bx.y2 <= bxs[i].y2 + gap))) {
			return true;
		}
	}

	return false;
}

bool drawSection(std::vector<Box> &bxs, std::vector<char> &line, int x, int y)
{
	int bxsSize = bxs.size();

	for (int i = 0; i < bxsSize; ++i) {
		if (!bxs[i].drin && Function::getRandNum(20) == 1 &&
		    (((x > bxs[i].x1 && x < bxs[i].x2) &&
		      (y == bxs[i].y1 || y == bxs[i].y2)) ||
		     ((y > bxs[i].y1 && y < bxs[i].y2) &&
		      (x == bxs[i].x1 || x == bxs[i].x2)))) {
			line.push_back(' ');
			bxs[i].drin = true;
			return true;
		}

		if ((x == bxs[i].x1 || x == bxs[i].x2) && y > bxs[i].y1 &&
		    y < bxs[i].y2) {
			line.push_back('#');
			return true;
		} else if ((x >= bxs[i].x1 && x <= bxs[i].x2) &&
			   (y == bxs[i].y1 || y == bxs[i].y2)) {
			line.push_back('#');
			return true;
		}
	}

	return false;
}

char drawBase(std::vector<Box> &bxs, int x, int y)
{
	int bxsSize = bxs.size();

	for (int i = 0; i < bxsSize; ++i) {
		if (x > bxs[i].x1 && x < bxs[i].x2 && y > bxs[i].y1 &&
		    y < bxs[i].y2) {
			return '-';
		}
	}

	return '.';
}

void drawBlock(WINDOW *pWin, std::vector<std::vector<char>> &block, int offsetX,
	       int offsetY)
{
	int ySize = block.size();

	wattron(pWin, COLOR_PAIR(1));
	for (int y = 0; y < ySize; ++y) {
		int xSize = block[y].size();

		for (int x = 0; x < xSize; ++x) {
			mvwaddch(pWin, y - offsetY, x - offsetX, block[y][x]);
		}
	}
	wattroff(pWin, COLOR_PAIR(1));

	wrefresh(pWin);
}

void makeUnique(std::vector<Box> &bxs, std::vector<Chest> &chests)
{
	int bxsSize = bxs.size();

	for (int i = 0; i < bxsSize; ++i) {
		if (Function::getRandNum(2) == 1) {
			Chest chest;

			chest.x =
			    Function::getRandNum(bxs[i].x1 + 1, bxs[i].x2 - 1);
			chest.y =
			    Function::getRandNum(bxs[i].y1 + 1, bxs[i].y2 - 1);

			chests.push_back(chest);
		}
	}
}

void drawUnique(WINDOW *pWin, std::vector<Chest> &chests, int offsetX,
		int offsetY)
{
	int itmsSize = chests.size();

	wattron(pWin, COLOR_PAIR(2));
	for (int i = 0; i < itmsSize; ++i) {
		if (chests[i].loot.size() <= 0) {
			wattron(pWin, A_REVERSE);
			mvwaddch(pWin, chests[i].y - offsetY,
				 chests[i].x - offsetX, 'C');
			wattroff(pWin, A_REVERSE);
		} else {
			mvwaddch(pWin, chests[i].y - offsetY,
				 chests[i].x - offsetX, 'C');
		}
	}
	wattroff(pWin, COLOR_PAIR(2));

	wrefresh(pWin);
}

void makeMap(std::vector<Box> &blds, std::vector<std::vector<char>> &chrs,
	     int maxY, int maxX)
{
	for (int y = 0; y < maxY; ++y) {
		std::vector<char> line;

		for (int x = 0; x < maxX; ++x) {
			Box bld;

			bld.x1 = x;
			bld.x2 = x + Function::getRandNum(COLS * 0.8 / 4);
			bld.y1 = y;
			bld.y2 = y + Function::getRandNum(LINES * 0.8 / 4);

			if (bld.x2 - bld.x1 >= 3 && bld.y2 - bld.y1 >= 3 &&
			    !checkIntersection(blds, bld)) {
				blds.push_back(bld);
			}

			if (!drawSection(blds, line, x, y)) {
				line.push_back(drawBase(blds, x, y));
			}
		}

		chrs.push_back(line);
	}
}

#endif
