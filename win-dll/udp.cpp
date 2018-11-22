#include "stdafx.h"
#include <winsock2.h>
#include <ws2tcpip.h>
#include "udp.h"

#pragma comment(lib, "ws2_32.lib")


Udp::Udp(int sport, int cport) : server_port(sport), client_port(cport)
{
}

Udp::~Udp()
{
}

int Udp::open(DWORD timeout)
{
	int ret;
	WSADATA wsaData;
	bool bOpt = true;
	
	ret = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (ret != 0) {
		return -1;
	}
	sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (INVALID_SOCKET == sockfd) {
		WSACleanup();
		return -1;
	}
	
	serAddr.sin_family = AF_INET;
	serAddr.sin_port = htons(server_port);
	serAddr.sin_addr.s_addr = htonl(INADDR_BROADCAST);
	//serAddr.sin_addr.s_addr = inet_addr("192.168.1.255");

	cltAddr.sin_family = AF_INET;
	cltAddr.sin_port = htons(client_port);
	cltAddr.sin_addr.s_addr = 0;
	if (bind(sockfd, (const SOCKADDR *)&cltAddr, sizeof(cltAddr)) < 0) {
		return -1;
	}

	// set this socket to broadcast type
	ret = setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, (char *)&bOpt, sizeof(bOpt));
	if (ret == SOCKET_ERROR) {
		closesocket(sockfd);
		WSACleanup();
		return -1;
	}

	if (timeout > 0) {
		isBlock = true;
		if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout)) == SOCKET_ERROR) {
			closesocket(sockfd);
			WSACleanup();
			return -1;
		}
	} else if (timeout == 0) {
		ULONG nVal = 1;
		if (ioctlsocket(sockfd, FIONBIO, &nVal) == SOCKET_ERROR) {
			closesocket(sockfd);
			WSACleanup();
			return -1;
		}
	}
	isOpen = true;
	return 0;
}

void Udp::close()
{
	closesocket(sockfd);
	WSACleanup();
	isOpen = false;
}

int Udp::send(char *buf, int len)
{
	int ret;

	if ((ret = sendto(sockfd, buf, len, 0, (const SOCKADDR *)&serAddr, sizeof(serAddr))) == SOCKET_ERROR) {
		return -1;
	}
	return ret;
}

int Udp::send(char *buf)
{
	return send(buf, strlen(buf));
}

int Udp::recv(char *buf, int len, DWORD timeout)
{
	int ret = 0;
	int tmp_len = 0;
	int nSrvAddrLen = sizeof(srvAddr1);
	
	if (isBlock) {
		if ((tmp_len = recvfrom(sockfd, buf, len - 1, 0, (SOCKADDR *)&srvAddr1, &nSrvAddrLen)) == SOCKET_ERROR) {
			tmp_len = 0;
		}
	} else {
		DWORD t1, t2;

		t2 = t1 = GetTickCount();
		while (t2 - t1 <= timeout) {
			if ((ret = recvfrom(sockfd, buf + tmp_len, len - 1 - tmp_len, 0, (SOCKADDR *)&srvAddr1, &nSrvAddrLen)) != SOCKET_ERROR) {
				tmp_len += ret;
				ret = 0;
				if (tmp_len >= (len - 1)) {
					break;
				}
			}
			t2 = GetTickCount();
		}
	}
	buf[tmp_len] = '\0';
	
	return tmp_len;
}


int Udp::setTimeout(DWORD timeout)
{
	if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout)) == SOCKET_ERROR) {
		return -1;
	}
	return 0;
}
