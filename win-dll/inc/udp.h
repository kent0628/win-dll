#pragma once

#include "baseDll.h"

class DLL_API Udp
{
private:
	bool isBlock = false;
	int server_port;
	int client_port;

public:
	SOCKET sockfd;
	SOCKADDR_IN cltAddr;
	SOCKADDR_IN serAddr;
	SOCKADDR_IN srvAddr1;
	bool isOpen = false;

	Udp(int sport, int cport);
	~Udp();

	int open(DWORD timeout = 0);
	void close();
	int send(char *buf, int len);
	int send(char *buf);
	int recv(char *buf, int len, DWORD timeout = 20);

	int setTimeout(DWORD timeout);
};