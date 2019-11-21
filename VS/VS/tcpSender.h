#pragma once
#pragma comment (lib, "ws2_32.lib")
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <vector>

class tcpSender
{
public:
	tcpSender();
	int checkSocket();
	void sendDataByTCP(std::vector<std::vector<float>>);
	~tcpSender();

private:
	WSADATA wsaData;
	SOCKET ListenSocket, ClientSocket;  // впускающий сокет и сокет для клиентов

	sockaddr_in ServerAddr;  // server address
	int err, maxlen = 512;  // code of error
	//char* data_string = new char[maxlen];  // bufer of sending

	int network_Status = 1;
};
