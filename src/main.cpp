#include <ncurses.h>
#include <thread>
#include <vector>

#include "include/dungeon-generator.hpp"
#include "include/player.hpp"

const int maxMapY = 50;
const int maxMapX = 100;

enum GameState {
	PLAYING,
	INVENTORY,
	MAINMENU,
};

WINDOW *createNewWin(int h, int w, int sy, int sx)
{
	WINDOW *localWin;
	localWin = newwin(h, w, sy, sx);

	box(localWin, 0, 0);
	wrefresh(localWin);

	return localWin;
}

void destroyWin(WINDOW *localWin)
{
	wborder(localWin, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
	wrefresh(localWin);
	delwin(localWin);
}

void checkIfOnChest(WINDOW *pWin, std::vector<Chest> &chests, int x, int y)
{
	int chestSize = chests.size();
	bool chestDisplayed = false;

	for (int i = 0; i < chestSize; ++i) {
		if (x == chests[i].x && y == chests[i].y) {
			chests[i].displayLoot(pWin);
			chestDisplayed = true;
			break;
		}
	}

	if (!chestDisplayed) {
		werase(pWin);
		box(pWin, 0, 0);
		mvwprintw(pWin, 1, 1, "Loot Window");
		wrefresh(pWin);
	}
}

Chest &getActiveChest(std::vector<Chest> &chests, int x, int y)
{
	int chestSize = chests.size();

	for (int i = 0; i < chestSize; ++i) {
		if (x == chests[i].x && y == chests[i].y) {
			return chests[i];
		}
	}

	return chests[0];
}

void lootChest(WINDOW *lootWindow, std::vector<Chest> &chests, Player *player,
	       MEVENT &event)
{
	Chest &chst = getActiveChest(chests, player->x, player->y);
	int mousePos = static_cast<int>(floor(event.y - (LINES * 0.5)));
	int amtOfLoot = chst.loot.size();

	for (int i = 0; i < amtOfLoot; ++i) {
		if (mousePos == chst.loot[i].y) {
			if (chst.loot[i].type == ITEM) {
				player->inventory.push_back(chst.loot[i]);
				chst.loot.erase(chst.loot.begin() + i);
			} else if (chst.loot[i].type == GOLD) {
				player->gold += chst.loot[i].value;
				chst.loot.erase(chst.loot.begin() + i);
			}

			werase(lootWindow);
			box(lootWindow, 0, 0);
			chst.displayLoot(lootWindow);

			player->drawInventory();

			break;
		}
	}
}

int main()
{
	GameState gamestate = GameState::PLAYING;

	WINDOW *mainWindow;
	WINDOW *lootWindow;

	MEVENT event;

	// Initializing the screen
	initscr();

	// Setting up colours
	start_color();
	init_pair(1, COLOR_WHITE, COLOR_BLACK);
	init_pair(2, COLOR_YELLOW, COLOR_YELLOW);
	init_pair(3, COLOR_CYAN, COLOR_CYAN);

	// Setting up inputs
	raw();
	keypad(stdscr, TRUE);
	noecho();
	clear();
	cbreak();
	mousemask(ALL_MOUSE_EVENTS, NULL);

	// Disabling input
	curs_set(0);

	refresh();

	mainWindow = createNewWin(LINES * 0.8, COLS * 0.8, 0, 0);
	werase(mainWindow);

	int maxX;
	int maxY;
	getmaxyx(mainWindow, maxY, maxX);

	std::vector<Box> blds;
	std::vector<std::vector<char>> chrs;
	std::vector<Chest> chests;

	std::thread t_buildThread(makeMap, std::ref(blds), std::ref(chrs),
				  maxMapY, maxMapX);

	t_buildThread.join();

	std::thread t_uniqueThread(makeUnique, std::ref(blds),
				   std::ref(chests));

	t_uniqueThread.join();

	Player *player = new Player(
	    {static_cast<int>(COLS * 0.8), static_cast<int>(LINES * 0.2), 0,
	     static_cast<int>(LINES * 0.8)},
	    {static_cast<int>(COLS * 0.2), static_cast<int>(LINES * 0.5),
	     static_cast<int>(COLS * 0.8), 0});
	player->setSpawnPos(chrs, maxMapX, maxMapY);

	lootWindow = newwin(LINES * 0.5, COLS * 0.2, LINES * 0.5, COLS * 0.8);
	box(lootWindow, 0, 0);

	mvwprintw(lootWindow, 1, 1, "Loot Window");

	wrefresh(lootWindow);

	int cx = (COLS * 0.8) / 2;
	int cy = (LINES * 0.8) / 2;

	drawBlock(mainWindow, chrs, player->x - cx, player->y - cy);
	drawUnique(mainWindow, chests, player->x - cx, player->y - cy);

	player->draw(mainWindow, cx, cy);
	wrefresh(mainWindow);

	bool loop = true;

	// Really odd workaround to make the mouse work.
	keypad(mainWindow, TRUE);
	keypad(lootWindow, TRUE);
	keypad(player->inventoryWindow, TRUE);
	keypad(player->statusWindow, TRUE);

	while (loop) {
		int ch = getch();

		switch (gamestate) {
		case GameState::INVENTORY:
			break;

		case GameState::MAINMENU:
			break;

		case GameState::PLAYING:
			switch (ch) {
			case KEY_MOUSE:
				if (getmouse(&event) == OK) {
					if (event.bstate & BUTTON1_CLICKED) {
						lootChest(lootWindow, chests,
							  player, event);
					} else if (event.bstate &
						   BUTTON3_CLICKED) {
					}
				}
				break;

			case KEY_LEFT:
				if (chrs[player->y][player->x - 1] != '#' &&
				    player->x > 0) {
					player->x -= 1;
				}
				break;

			case KEY_RIGHT:
				if (chrs[player->y][player->x + 1] != '#' &&
				    player->x < maxMapX) {
					player->x += 1;
				}
				break;

			case KEY_UP:
				if (chrs[player->y - 1][player->x] != '#' &&
				    player->y > 1) {
					player->y -= 1;
				}
				break;

			case KEY_DOWN:
				if (chrs[player->y + 1][player->x] != '#' &&
				    player->y < maxMapY - 2) {
					player->y += 1;
				}
				break;

			default:
				break;
			}

			checkIfOnChest(lootWindow, chests, player->x,
				       player->y);

			werase(mainWindow);

			drawBlock(mainWindow, chrs, player->x - cx,
				  player->y - cy);
			drawUnique(mainWindow, chests, player->x - cx,
				   player->y - cy);
			player->draw(mainWindow, cx, cy);

			mvwprintw(mainWindow, 1, 1,
				  std::to_string(maxY).c_str());

			wrefresh(mainWindow);

			break;
		}
	}

	endwin();
	return 0;
}
