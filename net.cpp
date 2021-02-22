#include "net.h"
#include <vector>
#include <sstream>
#include <functional>
#include <iostream>
#include <errno.h>

void Net::send_with_retry(std::string request){
	while(BIO_write(sock,request.c_str(),request.size()+1)<=0){
		if(!BIO_should_retry(sock)){
			std::cout<<"send error"<<std::endl;
			closed=1;
			return;
		}
	}
}

void Net::read_with_retry(std::string &data){
	char r[32];
	int ret;
	ret=BIO_read(sock,r,32);
	if(ret==0){
		std::cout<<"recv error(conn closed)"<<std::endl;
		closed=1;
		return;
	} else if(ret<0)
		if(!BIO_should_retry(sock)){
			std::cout<<"recv error"<<std::endl;
			closed=1;
			return;
		}
	data+=r;
}

Net::Net():closed(0),ready(0){
	ERR_load_BIO_strings();
}

Net::~Net(){
	BIO_free_all(sock);
}

int Net::makeconnect(std::string ip){
	ip+=":45451";
	sock=BIO_new_connect(ip.c_str());
	if(sock==NULL){
		std::cout<<"bio error"<<std::endl;
		return -1;
	}

	if(BIO_do_connect(sock)<=0){
		std::cout<<"bio error:";
		ERR_print_errors_fp(stdout);
		std::cout<<std::endl;
		closed=1;
		return -1;
	}
	return 0;
}

void Net::closing(){
	std::string request="CLOSE";
	send_with_retry(request);
	BIO_shutdown_wr(sock);
	closed=1;
}

std::tuple<int,int> Net::login(long long room){
	std::string request="LOGIN "+std::to_string(room);
	std::string reply;
	send_with_retry(request);
	if(closed)return std::make_tuple<int,int>(-1,-1);
	read_with_retry(reply);
	if(closed)return std::make_tuple<int,int>(-1,-1);

	std::stringstream stream(reply);
	std::string temp;
	std::vector<std::string> replys;
	while(std::getline(stream,temp,' ')){
		if(!temp.empty()){
			replys.push_back(temp);
		}
	}
	if(replys[0].find("SUCCESS")!=std::string::npos){
		return std::make_tuple<int,int>(std::stoi(replys[1]),std::stoi(replys[2]));
	}
	closed=1;
	return std::make_tuple<int,int>(-1,-1);
}

std::tuple<int,int> Net::login(long long room,std::string pass){
	std::string request="LOGIN "+std::to_string(room)+" PASSWORD "+pass;
	std::string reply;
	send_with_retry(request);
	if(closed)return std::make_tuple<int,int>(-1,-1);
	read_with_retry(reply);
	if(closed)return std::make_tuple<int,int>(-1,-1);

	std::stringstream stream(reply);
	std::string temp;
	std::vector<std::string> replys;
	while(std::getline(stream,temp,' ')){
		if(!temp.empty()){
			replys.push_back(temp);
		}
	}
	if(replys[0].find("SUCCESS")!=std::string::npos){
		return std::make_tuple<int,int>(std::stoi(replys[1]),std::stoi(replys[2]));
	}
	closed=1;
	return std::make_tuple<int,int>(-1,-1);
}

long long Net::makeroom(int x,int y){
	std::string request="ROOM "+std::to_string(x)+" "+std::to_string(y);
	std::string reply;
	send_with_retry(request);
	if(closed)return -1;
	read_with_retry(reply);
	if(closed)return -1;

	std::stringstream stream(reply);
	std::string temp;
	std::vector<std::string> replys;
	while(std::getline(stream,temp,' ')){
		if(!temp.empty()){
			replys.push_back(temp);
		}
	}
	if(replys[0].find("SUCCESS")!=std::string::npos){
		return std::stoll(replys[1]);
	}
	closed=1;
	return -1;
}

long long Net::makeroom(int x,int y,std::string pass){
	std::string request="ROOM "+std::to_string(x)+" "+std::to_string(y)+" PASSWORD "+pass;
	std::string reply;
	send_with_retry(request);
	if(closed)return -1;
	read_with_retry(reply);
	if(closed)return -1;
	std::stringstream stream(reply);
	std::string temp;
	std::vector<std::string> replys;
	while(std::getline(stream,temp,' ')){
		if(!temp.empty()){
			replys.push_back(temp);
		}
	}
	if(replys[0].find("SUCCESS")!=std::string::npos){
		return std::stoll(replys[1]);
	}
	closed=1;
	return -1;
}

int Net::put(int x,int y){
	std::string request="PUT "+std::to_string(x)+" "+std::to_string(y);
	send_with_retry(request);
	if(closed)return -1;
	return 0;
}

int Net::freeput(int x,int y){
	std::string request="FREEPUT "+std::to_string(x)+" "+std::to_string(y);
	send_with_retry(request);
	if(closed)return -1;
	return 0;
}

std::tuple<std::string,int,int> Net::get(){
	std::string ret;
	read_with_retry(ret);
	if(closed) return std::make_tuple("nodata",-1,-1);

	std::vector<std::string> parsed;
	std::stringstream stream{ret};
	std::string buf;
	while(std::getline(stream,buf,' ')){
		parsed.push_back(buf);
	}
	if(parsed.size()==3)
		return std::make_tuple(parsed[0],std::stoi(parsed[1]),std::stoi(parsed[2]));
	else if(parsed.size()==2){
		long long int temp=std::stoll(parsed[1]);
		return std::make_tuple(parsed[0],(temp&0xffffffff00000000)>>32,temp&0xffffffff);
	} else
		return std::make_tuple(parsed[0],-1,-1);
}


long long Net::automatch(){
	std::string request="AUTO ";
	std::string reply;
	send_with_retry(request);
	if(closed)return -1;
	read_with_retry(reply);
	if(closed)return -1;

	std::stringstream stream(reply);
	std::string temp;
	std::vector<std::string> replys;
	while(std::getline(stream,temp,' ')){
		if(!temp.empty()){
			replys.push_back(temp);
		}
	}
	if(replys[0].find("HOST")!=std::string::npos){
		return 0;
	} else if(replys[0].find("GUEST")!=std::string::npos) return std::stoll(replys[1]);
	closed=1;
	return -1;
}
