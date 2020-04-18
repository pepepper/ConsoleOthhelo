#include <string>
#include <utility>
#include <errno.h>  
#include <openssl/bio.h>
#include <openssl/err.h>


class Net{
public:
	Net();
	~Net();
	void closing();
	int makeconnect(std::string host);
	std::tuple<int,int> login(long long room,std::string pass);
	std::tuple<int,int> login(long long room);
	long long makeroom(int x,int y);
	long long makeroom(int x,int y,std::string pass);
	int put(int x,int y);
	int freeput(int x,int y);
	std::tuple<std::string,int,int> get();
	long long automatch();
	int closed,ready,started;
private:
	void send_with_retry(std::string);
	void read_with_retry(std::string &);
	BIO *sock;
};
