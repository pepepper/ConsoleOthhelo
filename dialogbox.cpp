#include "dialogbox.h"
#include <string>
#include <iostream>

dialogbox::dialogbox(){}
dialogbox::~dialogbox(){}

int dialogbox::QuitinGameDialogbox(){
	return 0;
}

int dialogbox::EndGameDialogBox(int black, int white, int turn){
	std::cout<<"\e[19;7Hblack:"<<black<<" white:"<<white<<" turn:"<<turn;
	return 1;
}

int dialogbox::ConnectionclosedDialogBox(){
	std::cout<<"\e[18;1Hconnection closed...";
	return 1;
}
