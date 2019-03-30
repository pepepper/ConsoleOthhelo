#include "Boards.h"
Boards::Boards(int x, int y): delta(40, std::vector<int>(3, 0)){
	boardx = x;
	boardy = y;
	for(int i = 0; i < 16; i++){
		for(int j = 0; j < 16; j++){
			if(i < (16 - x) / 2 || i>15 - ((16 - x) / 2) || j < (16 - y) / 2 || j>15 - ((16 - y) / 2))
				board[i][j] = 99;//invalid space
			else board[i][j] = 0;
		}
	}
	noCheckPut(7, 7, 1);
	noCheckPut(8, 8, 1);
	noCheckPut(8, 7, -1);
	noCheckPut(7, 8, -1);
}

void Boards::Reverse(unsigned int x, unsigned int y, int vect_x, int vect_y, int turn){
	int i = 0;
	board[x][y] = turn;
	while(delta[i][0]){ i++; }
	delta[i][0] = turn;
	delta[i][1] = x;
	delta[i][2] = y;
	x += vect_x;
	y += vect_y;
	while(!(x>15 || y>15 || board[x][y] == 99)){
		if(board[x][y] == turn || board[x][y] == 0) break;
		board[x][y] = turn;
		while(delta[i][0]){ i++; }
		delta[i][0] = turn;
		delta[i][1] = x;
		delta[i][2] = y;
		x += vect_x;
		y += vect_y;
	}
}

bool Boards::isPutable(unsigned int x, unsigned int y, int deltax, int deltay, int turn){
	x += deltax;
	y += deltay;
	if(x>15 || y>15 || board[x][y]==99)return false;
	if(board[x][y] == turn)return false;
	if(board[x][y] == 0)return false;

	x += deltax;
	y += deltay;
	while(x<15 && y<15 && board[x][y]!=99){
		if(board[x][y] == 0)return false;
		if(board[x][y] == turn)return true;
		x += deltax;
		y += deltay;
	}
	return false;
}

bool Boards::isPutable(int x, int y, int turn){
	bool ret = false;
	if( board[x][y] != 0) return ret;//1 white -1 black

	if(isPutable(x, y, 1, 0, turn)){
		ret = true;
	}    //右
	if(isPutable(x, y, 0, 1, turn)){
		ret = true;
	}    //下
	if(isPutable(x, y, -1, 0, turn)){
		ret = true;
	}    //左
	if(isPutable(x, y, 0, -1, turn)){
		ret = true;
	}    //上
	if(isPutable(x, y, 1, 1, turn)){
		ret = true;
	}    //右下
	if(isPutable(x, y, -1, -1, turn)){
		ret = true;
	}    //左上
	if(isPutable(x, y, 1, -1, turn)){
		ret = true;
	}    //右上
	if(isPutable(x, y, -1, 1, turn)){
		ret = true;
	}    //左下

	return ret;
}

bool Boards::Put(int x, int y, int turn){
	bool ret = false;
	if(board[x][y] != 0) return ret;//1 white -1 black
	if(isPutable(x, y, 1, 0, turn)){
		Reverse(x, y, 1, 0, turn);
		ret = true;
	}    //右
	if(isPutable(x, y, 0, 1, turn)){
		Reverse(x, y, 0, 1, turn);
		ret = true;
	}    //下
	if(isPutable(x, y, -1, 0, turn)){
		Reverse(x, y, -1, 0, turn);
		ret = true;
	}    //左
	if(isPutable(x, y, 0, -1, turn)){
		Reverse(x, y, 0, -1, turn);
		ret = true;
	}    //上
	if(isPutable(x, y, 1, 1, turn)){
		Reverse(x, y, 1, 1, turn);
		ret = true;
	}    //右下
	if(isPutable(x, y, -1, -1, turn)){
		Reverse(x, y, -1, -1, turn);
		ret = true;
	}    //左上
	if(isPutable(x, y, 1, -1, turn)){
		Reverse(x, y, 1, -1, turn);
		ret = true;
	}    //右上
	if(isPutable(x, y, -1, 1, turn)){
		Reverse(x, y, -1, 1, turn);
		ret = true;
	}    //左下

	return ret;
}

void Boards::noCheckPut(int x, int y, int turn){
	int i = 0;
	board[x][y] = turn;
	while(delta[i][0]){ i++; }
	delta[i][0] = turn;
	delta[i][1] = x;
	delta[i][2] = y;
}

bool Boards::freeput(int x, int y, int turn){
	if(board[x][y] != 0)return false;
	int i = 0;
	board[x][y] = turn;
	if(isPutable(x, y, 1, 0, turn)){
		Reverse(x, y, 1, 0, turn);
	}    //右
	if(isPutable(x, y, 0, 1, turn)){
		Reverse(x, y, 0, 1, turn);
	}    //下
	if(isPutable(x, y, -1, 0, turn)){
		Reverse(x, y, -1, 0, turn);
	}    //左
	if(isPutable(x, y, 0, -1, turn)){
		Reverse(x, y, 0, -1, turn);
	}    //上
	if(isPutable(x, y, 1, 1, turn)){
		Reverse(x, y, 1, 1, turn);
	}    //右下
	if(isPutable(x, y, -1, -1, turn)){
		Reverse(x, y, -1, -1, turn);
	}    //左上
	if(isPutable(x, y, 1, -1, turn)){
		Reverse(x, y, 1, -1, turn);
	}    //右上
	if(isPutable(x, y, -1, 1, turn)){
		Reverse(x, y, -1, 1, turn);
	}    //左下

	while(delta[i][0]){ i++; }
	delta[i][0] = turn;
	delta[i][1] = x;
	delta[i][2] = y;
	return true;
}

bool Boards::isFull(int turn){
	for(int x = 0; x < 16; x++){
		for(int y = 0; y < 16; y++){
			if(board[x][y] == 0)
				if(isPutable(x, y, turn))return false;
		}
	}
	return true;
}

std::pair<int, int> Boards::Count(){
	int b = 0, w = 0;
	for(int i = 0; i < 16; i++){
		for(int j = 0; j < 16; j++){
			if(board[i][j] == 1)w++;
			else if(board[i][j] == -1)b++;
		}
	}
	return std::pair<int, int>(w, b);
}

Boards::~Boards(){
	std::array<std::array<int, 16>, 16>().swap(board);
}
