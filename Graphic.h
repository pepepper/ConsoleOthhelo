#pragma once
#include <SDL.h>
#include <vector>
class Graphic{
	public:
	Graphic();
	void StartGame();
	void Put(std::vector<std::vector<int>> &delta);
	void update();
	void changeturn(int turn);
	void end();
	void netwait();
	~Graphic();
	private:
	SDL_Window *window;
	SDL_Renderer *renderer;
	SDL_Texture *board, *white, *black, *tex;
	SDL_Rect rect = {526, 526, 1052, 1052}, komarect = {0, 0, 128, 128};
};
//192 +48
