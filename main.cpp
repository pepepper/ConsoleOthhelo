#include <memory>
#include "Graphic.h"
#include "Game.h"
#include "dialogbox.h"
#include "net.h"
#include <string>
#include <iostream>
#include <typeinfo>
#include <tuple>
#include<sstream>

int intGetOption(const char *message){
	std::string temp;
	std::cout << message;
	std::getline(std::cin, temp);
	return std::stoi(temp);
}

long long llGetOption(const char *message){
	std::string temp;
	std::cout << message;
	std::getline(std::cin, temp);
	return std::stoll(temp);
}

std::string strGetOption(const char *message){
	std::string temp;
	std::cout << message;
	std::getline(std::cin, temp);
	return temp;
}

int main(int argc, char *argv[]){
	std::string ip, pass, arg;
	long long room;
	int x, y, mode = -1, netmode = -1, netret, freeput = 0;
	std::unique_ptr<Game> game;
	std::unique_ptr<Net> net;
	try{
		mode = intGetOption("モードを選択してください\n0:オフラインで交互にプレイする 1:オンラインでプレイする場合 :");
		if(mode != 0 && mode != 1)throw std::invalid_argument("");
		else if(mode == 0){//offline
			if(intGetOption("盤面サイズを指定しますか?\n0:指定しない 1:指定する :") == 0) game.reset(new Game());
			else{
				std::string sx, sy;
				int x, y;
				x = intGetOption("8以下で縦のサイズを入力してください(盤面は入力された数値の倍のサイズになります):");
				y = intGetOption("8以下で横のサイズを入力してください(盤面は入力された数値の倍のサイズになります):");
				if(x > 8 || y > 8)throw std::invalid_argument("");
				game.reset(new Game(x, y));
			}
		} else if(mode == 1){//online
			net.reset(new Net());
			ip = strGetOption("サーバーのIPアドレスまたはドメインを入力してください:");
			netret = net->makeconnect(ip);
			if(netret){
				std::cout << "通信エラー:終了します" << std::endl;
				return 1;
			}
			netmode = intGetOption("モードを選択してください\n0:ホストとして部屋を立てる 1:ゲストとして部屋に入る :");
			if(netmode != 0 && netmode != 1)throw std::invalid_argument("");
			else if(netmode == 0){//host
				netmode = -1;
				if(netret == -1){
					std::cout << "通信エラー:終了します" << std::endl;
					return 1;
				}
				if(intGetOption("盤面サイズを指定しますか?\n0:指定しない 1:指定する :") == 0){
					game.reset(new Game());
				} else{
					std::string sx, sy;
					int x, y;
					x = intGetOption("8以下で縦のサイズを入力してください(盤面は入力された数値の倍のサイズになります):");
					y = intGetOption("8以下で横のサイズを入力してください(盤面は入力された数値の倍のサイズになります):");
					if(x > 8 || y > 8)throw std::invalid_argument("");
					game.reset(new Game(x, y));
				}
				std::cout << "パスワードを設定しますか?" << std::endl << "0:設定しない 1:設定する :";
				std::getline(std::cin, arg);
				if(!arg.compare("1")){
					pass = strGetOption("パスワードを入力してください:");
						room = net->makeroom(game->board->boardx/2, game->board->boardy/2,pass);
				}else 	room = net->makeroom(game->board->boardx/2, game->board->boardy/2);
				std::cout << "部屋番号:" + std::to_string(room) << std::endl;
			} else if(netmode == 1){//guest
				netmode = 1;
				std::tuple<int, int> size;
				room = llGetOption("部屋番号を入力してください:");
				std::cout << "パスワードが設定されていますか?" << std::endl << "0:設定されていない 1:設定されている :";
				std::getline(std::cin, arg);
				if(arg.compare("0") && arg.compare("1"))throw std::invalid_argument("");
				else if(!arg.compare("1")){
					pass = strGetOption("パスワードを入力してください:");
					size = net->login(room, pass);
				} else{
					size = net->login(room);
				}
				if(std::get<0>(size) == -1){
					std::cout << "通信エラー:終了します" << std::endl;
					return 1;
				}
				game.reset(new Game(std::get<0>(size), std::get<1>(size)));
			}
		}
	} catch(const std::invalid_argument& e){
		std::cout << "入力が不正です:終了します";
		return 1;
	}
	Graphic graphic;
	dialogbox dialog;

	graphic.StartGame();
	graphic.Put(game->board->delta);
	graphic.changeturn(game->turn);
	if(mode == 1)graphic.netwait(std::to_string(room));

	std::string input;
	while(true){
		if(mode==1&&net->closed == 0){
			std::tuple<std::string, int, int> action = net->get();
			if(std::get<0>(action).find("nodata")!=std::string::npos){
				net->closed = 1;
			} else if(std::get<0>(action).find("FREEPUT")!=std::string::npos && netmode != game->turn){
				game->put(std::get<1>(action), std::get<2>(action), freeput);
				graphic.Put(game->board->delta);
				graphic.changeturn(game->turn);
			} else if(std::get<0>(action).find("PUT")!=std::string::npos && netmode != game->turn){
				game->put(std::get<1>(action), std::get<2>(action));
				graphic.Put(game->board->delta);
				graphic.changeturn(game->turn);
			}else if(std::get<0>(action).find("CLOSED")!=std::string::npos){
				net->closed = 1;
			} else if(std::get<0>(action).find("READY")!=std::string::npos){
				graphic.changeturn(game->turn);
				net->started = 1;
				net->ready = 1;
			}
		}
		while(true){
			std::getline(std::cin,input);
			if(!input.compare("f")){
				freeput=!freeput;
				std::cout<<"\e[2;42H\e[0K";
				continue;
			}
			try{
				std::stringstream stream(input);
				std::string temp;
				std::vector<std::string> pos;
				while(std::getline(stream, temp, ' ')){
					if(!temp.empty()){
					pos.push_back(temp);
					}
				}
				x=stoi(pos[0]);
				y=stoi(pos[1]);
				break;
			}catch(const std::invalid_argument& e){
				std::cout<<"\e[2;42H\e[0K";
			}
		}
		if(mode == 0 || (netmode == game->turn&&net->ready)){
			if(freeput == 0){
				if(game->put(x, y)){
					if(mode == 1)net->put(x, y);
					graphic.Put(game->board->delta);
					graphic.changeturn(game->turn);
				}else{
					std::cout<<"\e[2;42H\e[0K";
				}
			} else {
				if(game->put(x, y, freeput)){
					if(mode == 1)net->freeput(x, y);
					graphic.Put(game->board->delta);
					graphic.changeturn(game->turn);
				}else{
					std::cout<<"\e[2;42H\e[0K";
				}
			}
			if(game->full){
				dialog.EndGameDialogBox(game->b, game->w, game->howturn);
				graphic.end();
				game->full = false;
			}
		}
			if(mode == 1 && net->closed == 1 && net->ready == 1){
				dialog.ConnectionclosedDialogBox();
				graphic.end();
				net->ready=0;
			}
	}
	return 0;
}