#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "Server.h"
#include <Windows.h>
#include <iostream>
#include <ws2tcpip.h>
#include <tchar.h>

// クライアント最大接続数
#define CLIENT_MAX 10

// 文字最大数
#define LENGTH_MAX 1024

// コンストラクタ
Server::Server() : 
	sock(INVALID_SOCKET)
{
	data = {};
	addr = {};
	client.resize(CLIENT_MAX);
	memset(client.data(), INVALID_SOCKET, sizeof(SOCKET) * client.size());
	clientAddr.resize(CLIENT_MAX);
	fds = {};
	readfds = {};
	time = { 0, 1000 };

	r.resize(LENGTH_MAX);
	s.resize(LENGTH_MAX);

	Init();
}

// デストラクタ
Server::~Server()
{
	Close();
	closesocket(sock);
	WSACleanup();
}

// 初期処理
int Server::Start(void)
{
	auto hr = WSAStartup(MAKEWORD(2, 0), &data);
	if (hr != 0)
	{
		OutputDebugString(_T("\n初期化：失敗\n"));
	}

	return  hr;
}

// ソケットの生成
int Server::Create(void)
{
	char port[5];
	printf("ポート番号：");
	scanf_s("%s", port, sizeof(port));
	//アドレスの設定
	addr.sin_family           = AF_INET;
	addr.sin_port             = htons(std::atoi(port));
	addr.sin_addr.S_un.S_addr = INADDR_ANY;

	//ソケット生成
	sock = socket(addr.sin_family, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET)
	{
		OutputDebugString(_T("\nソケットの生成：失敗\n"));
		return -1;
	}

	BOOL flag = 1;
	//ソケットの設定のセット
	auto hr = setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (const char*)&flag, sizeof(flag));
	if (hr != 0)
	{
		OutputDebugString(_T("\nソケットのセット：失敗\n"));
	}

	return hr;
}

// アドレスとの関連
int Server::Bind(void)
{
	auto hr = bind(sock, (struct sockaddr *)&addr, sizeof(addr));
	if (hr != 0)
	{
		OutputDebugString(_T("\nアドレスとの関連：失敗\n"));
	}

	return hr;
}

// クライアントとの接続待機
int Server::Listen(void)
{
	auto hr = listen(sock, 5);
	if (hr != 0)
	{
		OutputDebugString(_T("\n接続待機：失敗\n"));
	}

	return hr;
}

// 初期化
void Server::Init(void)
{
	Start();
	Create();
	Bind();
	Listen();
}

// クライアントとの接続
void Server::Accept(void)
{
	FD_ZERO(&readfds);
	for (UINT i = 0; i < CLIENT_MAX; ++i)
	{
		if (client[i] != INVALID_SOCKET)
		{
			continue;
		}
		
		int size = sizeof(sockaddr_in);
		client[i] = accept(sock, (struct sockaddr *)&clientAddr[i], &size);
		if (client[i] != INVALID_SOCKET)
		{
			FD_SET(client[i], &readfds);
			char str[INET_ADDRSTRLEN];
			printf("IP：%sが接続しました\n", inet_ntop(clientAddr[i].sin_family, &clientAddr[i].sin_addr, str, sizeof(str)));
		}
	}
}

// 受信
void Server::Recv(void)
{
	memcpy(&fds, &readfds, sizeof(fd_set));
	int len = select(0, &fds, NULL, NULL, &time);
	if (len != 0)
	{
		for (UINT i = 0; i < CLIENT_MAX; ++i)
		{
			if (client[i] == INVALID_SOCKET)
			{
				continue;
			}
			
			if (FD_ISSET(client[i], &fds))
			{
				// サーバからデータを受信
				memset(&r, 0, sizeof(r));
				auto hr = recv(client[i], (char*)r.data(), sizeof(char) * r.size(), 0);
				if (hr == -1)
				{
					char str[INET_ADDRSTRLEN];
					printf("%sとの接続が切れました\n", inet_ntop(clientAddr[i].sin_family, &clientAddr[i].sin_addr, str, sizeof(str)));
					closesocket(client[i]);
					client[i] = INVALID_SOCKET;

					continue;

				}
				
				char str[INET_ADDRSTRLEN];
				printf("%sからの受信：%s\n", inet_ntop(clientAddr[i].sin_family, &clientAddr[i].sin_addr, str, sizeof(str)), r.c_str());

				//指定IPアドレスに送信
				std::string dummy;
				for (int i = 0; r[i] != '/' && i < LENGTH_MAX; ++i)
				{
					dummy.push_back(r[i]);
				}
				for (int i = 0; i < CLIENT_MAX; ++i)
				{
					if (inet_addr(dummy.c_str()) == clientAddr[i].sin_addr.S_un.S_addr)
					{
						std::string p;
						p = dummy + "：";
						for (int o = dummy.size() + 1; o < LENGTH_MAX; ++o)
						{
							p.push_back(r[o]);
						}
						if (send(client[i], p.c_str(), sizeof(char) * r.size(), 0) == SOCKET_ERROR)
						{
							char str[INET_ADDRSTRLEN];
							printf("%sへの送信：失敗\n", inet_ntop(clientAddr[i].sin_family, &clientAddr[i].sin_addr, str, sizeof(str)));
						}

						break;
					}
				}
			}
		}
	}
	else
	{
		//printf("タイムアウト\n");
	}
}

// 送信
void Server::Send(void)
{
	//入力バッファのクリア
	fflush(stdin);

	scanf_s("%s", (char*)s.data(), sizeof(char) * s.size());
	for (unsigned int i = 0; i < CLIENT_MAX; ++i)
	{
		if (client[i] == INVALID_SOCKET)
		{
			continue;
		}
		
		if (send(client[i], s.data(), sizeof(char) * s.size(), 0) == SOCKET_ERROR)
		{
			char str[INET_ADDRSTRLEN];
			printf("%sへの送信：失敗\n", inet_ntop(clientAddr[i].sin_family, &clientAddr[i].sin_addr, str, sizeof(str)));
		}
	}
}

// クライアントとの接続終了
void Server::Close(void)
{
	for (UINT i = 0; i < CLIENT_MAX; ++i)
	{
		if (client[i] != INVALID_SOCKET)
		{
			closesocket(client[i]);
			client[i] = INVALID_SOCKET;
		}
	}
}
