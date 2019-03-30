#include "dialogbox.h"
#include <string>

dialogbox::dialogbox(){}
dialogbox::~dialogbox(){}

#ifdef Linux_System

int dialogbox::QuitinGameDialogbox(){
	boxer::Selection selection;
	selection = boxer::show(u8"本当にこのゲームを終了しますか?", u8"確認", boxer::Style::Question, boxer::Buttons::YesNo);
	if(selection == boxer::Selection::Yes){
		return 1;
	} else{
		return 0;
	}
}

int dialogbox::EndGameDialogBox(int black, int white, int turn){
	std::string str = u8"黒:" + std::to_string(black) + u8"\n白:" + std::to_string(white) + u8"\n総ターン数:" + std::to_string(turn);
	boxer::Selection selection;
	boxer::show(str.c_str(), u8"ゲーム終了", boxer::Style::Warning, boxer::Buttons::OK);
	return 1;
}

int dialogbox::ConnectionclosedDialogBox(){
	std::string str = u8"接続が切断されました";
	boxer::Selection selection;
	boxer::show(str.c_str(), u8"通知", boxer::Style::Warning, boxer::Buttons::OK);
	return 1;
}

#else

int dialogbox::QuitinGameDialogbox(){
	int ret;
	SDL_ShowMessageBox(&QuitinGameDialogDatas, &ret);
	return ret;
}

int dialogbox::EndGameDialogBox(int black, int white, int turn){
	std::string str = u8"黒:" + std::to_string(black) + u8"\n白:" + std::to_string(white) + u8"\n総ターン数:" + std::to_string(turn);
	SDL_MessageBoxData EndGameDialogDatas = {
		SDL_MESSAGEBOX_WARNING, /* .flags */
		NULL, /* .window */
		u8"ゲーム終了", /* .title */
		str.c_str(), /* .message */
		1, /* .numbuttons */
		&EndDialogButtons, /* .buttons */
		NULL /* .colorScheme */
	};
	int ret;
	SDL_ShowMessageBox(&EndGameDialogDatas, &ret);
	return ret;
}

int dialogbox::ConnectionclosedDialogBox(){
	int ret;
	SDL_ShowMessageBox(&ConnectionClosedDialogDatas, &ret);
	return ret;
}

#endif