﻿#define SDL_MAIN_HANDLED
#include <memory>
#include "Graphic.h"
#include "Game.h"
#include "dialogbox.h"
#include "net.h"
#include <string>
#include <iostream>
#include <typeinfo>
#include <tuple>
#include <thread>

int intGetOption(const char *message){
	std::string temp;
	std::cout << message;
	std::cin >> temp;
	return std::stoi(temp);
}

long long llGetOption(const char *message){
	std::string temp;
	std::cout << message;
	std::cin >> temp;
	return std::stoll(temp);
}

std::string strGetOption(const char *message){
	std::string temp;
	std::cout << message;
	std::cin >> temp;
	return temp;
}

int main(int argc, char *argv[]){
	std::string ip, pass, arg;
	long long room;
	SDL_Event e;
	int x, y, mode = -1, netmode = -1, netret, freeput = 0;
	Uint32 eventid;
	std::unique_ptr<Game> game;
	std::unique_ptr<Net> net;
	std::thread netthread;
	//部屋番号指定
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
				std::cin >> arg;
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
				std::cin >> arg;
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
	SDL_Init(SDL_INIT_EVERYTHING);
	eventid = SDL_RegisterEvents(1);
	Graphic graphic;
	dialogbox dialog;

	graphic.StartGame();
	graphic.Put(game->board->delta);
	graphic.changeturn(game->turn);
	graphic.update();
	if(mode == 1)graphic.netwait();
	if(mode == 1)netthread = std::thread([&game, &net, &graphic, &freeput, &netmode, &eventid]{
		SDL_Event graph;
		while(net->closed == 0){
			std::tuple<std::string, int, int> action = net->get();
			if(std::get<0>(action).find("nodata")!=std::string::npos){
				net->closed = 1;
			} else if(std::get<0>(action).find("FREEPUT")!=std::string::npos && netmode != game->turn){
				game->put(std::get<1>(action), std::get<2>(action), freeput);
				SDL_zero(graph);
				graph.type = eventid;
				SDL_PushEvent(&graph);
			} else if(std::get<0>(action).find("PUT")!=std::string::npos && netmode != game->turn){
				game->put(std::get<1>(action), std::get<2>(action));
				SDL_zero(graph);
				graph.type = eventid;
				SDL_PushEvent(&graph);
			}else if(std::get<0>(action).find("CLOSED")!=std::string::npos){
				net->closed = 1;
			} else if(std::get<0>(action).find("READY")!=std::string::npos){
				graphic.changeturn(game->turn);
				net->started = 1;
				net->ready = 1;
			}
		}
										 });
	while(true){
		while(SDL_WaitEvent(&e)){
			if(e.type == eventid){
				graphic.Put(game->board->delta);
				graphic.changeturn(game->turn);
				graphic.update();
			}
			switch(e.type){
				case SDL_WINDOWEVENT:
					if(e.window.event == SDL_WINDOWEVENT_EXPOSED){
						graphic.update();
					}
					break;
				case SDL_KEYUP:
					if(e.key.keysym.sym == SDLK_F2)freeput = !freeput;
					break;
				case SDL_MOUSEBUTTONUP:
					x = (int)(e.button.x / 48);
					y = (int)(e.button.y / 48);
					if(mode == 0 || (netmode == game->turn&&net->ready)){
						if(freeput == 0){
							if(game->put(x, y)){
								if(mode == 1)net->put(x, y);
								graphic.Put(game->board->delta);
								graphic.changeturn(game->turn);
								graphic.update();
							}
						} else if(game->put(x, y, freeput)){
							if(mode == 1)net->freeput(x, y);
							graphic.Put(game->board->delta);
							graphic.changeturn(game->turn);
							graphic.update();
						}
						if(game->full){
							dialog.EndGameDialogBox(game->b, game->w, game->howturn);
							graphic.end();
							game->full = false;
						}
					}
					break;
				case SDL_QUIT:
					if(dialog.QuitinGameDialogbox()){
						game.release();
						graphic.~Graphic();
						SDL_Quit();
						if(mode == 1){
							net->closing();
							netthread.join();
							net.release();
						}
						return 0;
					}
					break;
			}
			if(mode == 1 && net->closed == 1 && net->ready == 1){
				dialog.ConnectionclosedDialogBox();
				graphic.end();
				net->ready=0;
			}
		}
	}
	netthread.join();
	return 0;
}