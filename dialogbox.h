#pragma once

#ifdef Linux_System
#include "Boxer/include/boxer/boxer.h"

class dialogbox{
	public:
	dialogbox();
	int QuitinGameDialogbox();
	int EndGameDialogBox(int black, int white, int turn);
	int ConnectionclosedDialogBox();
	~dialogbox();

};

#else
#include <SDL.h>

class dialogbox{
	public:
	dialogbox();
	int QuitinGameDialogbox();
	int EndGameDialogBox(int black, int white, int turn);
	int ConnectionclosedDialogBox();
	~dialogbox();

	private:
	const SDL_MessageBoxButtonData QuitDialogButtons[2] = {
		{0, 0, u8"いいえ"},
	{0, 1, u8"はい"},
	};

	const SDL_MessageBoxButtonData EndDialogButtons = {0, 0, u8"OK"};

	const SDL_MessageBoxData QuitinGameDialogDatas = {
		SDL_MESSAGEBOX_WARNING, /* .flags */
		NULL, /* .window */
		u8"確認", /* .title */
		u8"本当にこのゲームを終了しますか?", /* .message */
		SDL_arraysize(QuitDialogButtons), /* .numbuttons */
		QuitDialogButtons, /* .buttons */
		NULL /* .colorScheme */
	};

	const SDL_MessageBoxData ConnectionClosedDialogDatas = {
		SDL_MESSAGEBOX_WARNING, /* .flags */
		NULL, /* .window */
		u8"通知", /* .title */
		u8"接続が切断されました", /* .message */
		1, /* .numbuttons */
		&EndDialogButtons, /* .buttons */
		NULL /* .colorScheme */
	};
};
#endif