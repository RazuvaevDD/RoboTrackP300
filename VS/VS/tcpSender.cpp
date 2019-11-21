#include "stdafx.h"
#include "tcpSender.h"
#include <sstream>

tcpSender::tcpSender()
{
	// Initialize Winsock
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	// Create a SOCKET for connecting to server
	ListenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	// Setup the TCP listening socket
	ServerAddr.sin_family = AF_INET;
	ServerAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	ServerAddr.sin_port = htons(9090);
	checkSocket();
	ClientSocket = accept(ListenSocket, NULL, NULL);

}

int tcpSender::checkSocket()
{
	err = bind(ListenSocket, (sockaddr*)&ServerAddr, sizeof(ServerAddr));
	if (err == SOCKET_ERROR) {
		//printf("[WARNING]: bind failed: %d\n", WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}

	err = listen(ListenSocket, 50);
	if (err == SOCKET_ERROR) {
		//printf("[WARNING]: listen failed: %d\n", WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}
}

void tcpSender::sendDataByTCP(std::vector<std::vector<float>>Dataa)
{
		std::vector<char> dataVector;
		for (int i = 0; i < Dataa.size(); i++)
		{
			for (int j = 0; j < Dataa[i].size(); j++)
			{
				std::string strBuf = std::to_string(Dataa[i][j]);
				for (int k = 0; k < strBuf.length(); k++)
				{
					dataVector.push_back(strBuf[k]);
				}
				if (j < Dataa[i].size() - 1) dataVector.push_back(',');
			}
			dataVector.push_back(';');
		}
		dataVector.push_back(NULL);
		char* data_stringG = reinterpret_cast<char*>(dataVector.data());
		send(ClientSocket, data_stringG, strlen(data_stringG), 0);

}

tcpSender::~tcpSender()
{
	// shutdown the connection since we're done
	closesocket(ClientSocket);
}
