#pragma once
#include <vector>
#include <array>
#include <utility>
class Boards{
	public:
	Boards(int x, int y);
	~Boards();
	bool isPutable(unsigned int x, unsigned int y, int deltax, int deltay, int turn);
	bool isPutable(int x, int y, int turn);
	bool Put(int x, int y, int turn);
	bool isFull(int turn);
	bool freeput(int x, int y, int turn);
	std::vector<std::vector<int>> delta;
	std::pair<int, int> Count();
	std::array<std::array<int, 16>, 16> board;
	int boardx, boardy;
	private:
	void Reverse(unsigned int x, unsigned int y, int vect_x, int vect_y, int turn);//石をひっくり返す
	void noCheckPut(int x, int y, int turn);
};

