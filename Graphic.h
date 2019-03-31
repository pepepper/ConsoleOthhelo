#pragma once
#include <iostream>
#include <vector>
class Graphic{
	public:
	Graphic();
	void StartGame();
	void Put(std::vector<std::vector<int>> &delta);
	void changeturn(int turn);
	void end();
	void netwait(std::string room);
	~Graphic();
};