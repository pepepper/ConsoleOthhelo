#pragma once
#include "Boards.h"
class Game{
	public:
	Game();
	Game(int x, int y);
	~Game();
	bool put(int x, int y);
	bool put(int x, int y, int freeput);
	void canPass();
	Boards *board;
	bool full = false;
	int howturn, b, w, turn;
};
