#include "Graphic.h"

void SDL_RenderCopyBynum(SDL_Renderer *renderer, SDL_Texture *tex, int x, int y, int w, int h){
	SDL_Rect rect = {x, y, w, h};
	SDL_RenderCopy(renderer, tex, NULL, &rect);
}

Graphic::Graphic(){
	SDL_Surface *temp;

	SDL_CreateWindowAndRenderer(768, 768, NULL, &window, &renderer);
	temp = SDL_LoadBMP("./icon.bmp");
	SDL_SetWindowIcon(window, temp);
	SDL_FreeSurface(temp);
	SDL_SetWindowTitle(window, u8"オセロ?");

	temp = SDL_LoadBMP("./green.bmp");
	board = SDL_CreateTextureFromSurface(renderer, temp);
	SDL_FreeSurface(temp);

	temp = SDL_LoadBMP("./white.bmp");
	white = SDL_CreateTextureFromSurface(renderer, temp);
	SDL_FreeSurface(temp);

	temp = SDL_LoadBMP("./black.bmp");
	black = SDL_CreateTextureFromSurface(renderer, temp);
	SDL_FreeSurface(temp);

	tex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 2104, 2104);
}


Graphic::~Graphic(){
	if(board)SDL_DestroyTexture(board);
	if(board)SDL_DestroyTexture(tex);
	if(white)SDL_DestroyTexture(white);
	if(black)SDL_DestroyTexture(black);
	if(renderer)SDL_DestroyRenderer(renderer);
	if(window)SDL_DestroyWindow(window);
}

void Graphic::StartGame(){
	SDL_SetRenderTarget(renderer, tex);
	SDL_SetRenderDrawColor(renderer, 10, 128, 10, 0);
	SDL_RenderClear(renderer);
	SDL_RenderCopy(renderer, board, NULL, &rect);
	SDL_SetRenderTarget(renderer, NULL);
	SDL_RenderCopy(renderer, tex, NULL, NULL);
}

void Graphic::Put(std::vector<std::vector<int>> &delta){//[n][0] white/black [n][1] x [n][2] y
	SDL_SetRenderTarget(renderer, tex);
	for(int i = 0; delta[i][0] && i < 40; i++){
		komarect.x = 132 * delta[i][1];
		komarect.y = 132 * delta[i][2];
		if(delta[i][0] == -1)
			SDL_RenderCopy(renderer, black, NULL, &komarect);
		else if(delta[i][0] == 1)
			SDL_RenderCopy(renderer, white, NULL, &komarect);
		delta[i][0] = delta[i][1] = delta[i][2] = 0;
	}
	SDL_SetRenderTarget(renderer, NULL);
	SDL_RenderCopy(renderer, tex, NULL, NULL);
}

void Graphic::update(){
	SDL_RenderPresent(renderer);
}

void Graphic::changeturn(int turn){
	if(turn == 1)	SDL_SetWindowTitle(window, u8"オセロ?:白番です");
	if(turn == -1)	SDL_SetWindowTitle(window, u8"オセロ?:黒番です");
}

void Graphic::end(){
	SDL_SetWindowTitle(window, u8"オセロ?:ゲーム終了");
}

void Graphic::netwait(){
	SDL_SetWindowTitle(window, u8"オセロ?:対戦相手待ち");
}