#include "Graphic.h"


Graphic::Graphic(){
	std::cout<<"\e[1;1HConsole Othhelo starting..."<<std::endl;
}


Graphic::~Graphic(){
}

void Graphic::StartGame(){
	std::cout<<"\e[2J\e[2;1H0\n1\n2\n3\n4\n5\n6\n7\n8\n9\nA\nB\nC\nD\nE\nF\n";
	std::cout<<"\e[1;1H  0 1 2 3 4 5 6 7 8 9 A B C D E F";
	std::cout<<"\e[2;2H";
	for(int i=0;i<16;i++){
		for(int j=0;j<17;j++){
			std::cout<<"| ";
		}
		std::cout<<"\e[1B\e[2G";
	}
}

void Graphic::Put(std::vector<std::vector<int>> &delta){//[n][0] white/black [n][1] x [n][2] y
	for(int i = 0; delta[i][0] && i < 40; i++){
		if(delta[i][0] == -1)
			std::cout<<"\e["<<delta[i][2]+2<<";"<<(delta[i][1]+1)*2+1<<"H"<<"b";
		else if(delta[i][0] == 1)
			std::cout<<"\e["<<delta[i][2]+2<<";"<<(delta[i][1]+1)*2+1<<"H"<<"w";
		delta[i][0] = delta[i][1] = delta[i][2] = 0;
	}
	std::cout<<"\e[2;42H       "<<std::endl;
}

void Graphic::changeturn(int turn){
	if(turn == -1)	std::cout<<"\e[1;38Hblack turn"<<std::endl;
	else if(turn == 1)	std::cout<<"\e[1;38Hwhite turn"<<std::endl;
	std::cout<<"\e[2;38H\e[0Kx y:";
}

void Graphic::end(){
	std::cout<<"\e[19;1Hended?"<<std::endl;
}

void Graphic::netwait(std::string room){
	std::cout<<"\e[2;38HWaiting guest at "<<room<<"..."<<std::endl;
}