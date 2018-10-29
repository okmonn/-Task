#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "Server.h"
#include <ws2tcpip.h>
#include <Windows.h>
#include <iostream>
#include <tchar.h>

#pragma comment(lib,"ws2_32.lib")

// クライアント最大数
#define CLIENT_MAX 10

// コンストラクタ
Server::Server() : 
	sock(INVALID_SOCKET)
{
	data = {};
	addr = {};
	fds = {};
	readfds = {};
	time = { 0, 1000 };

	memset(&r[0], 0, sizeof(r));
	memset(&s[0], 0, sizeof(s));

	client.resize(CLIENT_MAX);
	memset(client.data(), INVALID_SOCKET, sizeof(SOCKET) * client.size());
	clientAddr.resize(CLIENT_MAX);

	Init();
}

// デストラクタ
Server::~Server()
{
	Close();
	closesocket(sock);
	WSACleanup();
}

// テキストの読み込み
int Server::LoadText(const std::string & fileName)
{
	FILE* file = nullptr;
	if (fopen_s(&file, fileName.c_str(), "rb") != 0)
	{
		OutputDebugString(_T("\nファイルの参照：失敗\n"));
		return -1;
	}

	std::string dummy;
	char n = 0;
	while (std::feof(file) == 0)
	{
		fread(&n, sizeof(char), 1, file);
		dummy.push_back(n);
	}

	fclose(file);

	auto pos = dummy.find_first_of(':');
	port = dummy.substr(pos + 1, 5);

	return 0;
}

// 初期処理
int Server::Start(void)
{
	auto hr = WSAStartup(MAKEWORD(2, 0), &data);
	if (hr != 0)
	{
		OutputDebugString(_T("\n起動時の初期処理：失敗\n"));
	}

	return hr;
}

// ソケットの生成
int Server::Create(void)
{
	//アドレスの設定
	addr.sin_family           = AF_INET;
	addr.sin_port             = htons(std::atoi(port.c_str()));
	addr.sin_addr.S_un.S_addr = INADDR_ANY;

	sock = socket(addr.sin_family, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET)
	{
		OutputDebugString(_T("\nソケットの生成：失敗\n"));
		return -1;
	}

	//ソケットの設定のセット
	int flag = 1;
	auto hr = setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (const char*)&flag, sizeof(flag));
	if (hr != 0)
	{
		OutputDebugString(_T("\nソケットのセット：失敗\n"));
	}

	return hr;
}

// アドレスとの関連付け
int Server::Bind(void)
{
	auto hr = bind(sock, (struct sockaddr *)&addr, sizeof(addr));
	if (hr != 0)
	{
		OutputDebugString(_T("アドレスとの関連付け：失敗\n"));
	}

	return hr;
}

// クライアントとの接続待機
int Server::Listen(void)
{
	auto hr = listen(sock, 5);
	if (hr != 0)
	{
		OutputDebugString(_T("\nクライアントとの接続待機：失敗\n"));
	}

	return hr;
}

// 初期化
void Server::Init(void)
{
	LoadText("Information.txt");
	Start();
	Create();
	Bind();
	Listen();
}

// クライアントの接続
void Server::Accept(void)
{
	FD_ZERO(&readfds);
	for (unsigned int i = 0; i < CLIENT_MAX; ++i)
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
			printf("\nIP：%sが接続しました\n", inet_ntop(clientAddr[i].sin_family, &clientAddr[i].sin_addr, str, sizeof(str)));
		}
	}
}

// 受信
void Server::Recv(void)
{
	memcpy(&fds, &readfds, sizeof(fd_set));
	int len = select(0, &fds, nullptr, nullptr, &time);
	if (len != 0)
	{
		for (unsigned int i = 0; i < CLIENT_MAX; ++i)
		{
			if (client[i] == INVALID_SOCKET)
			{
				continue;
			}

			if (FD_ISSET(client[i], &fds))
			{
				memset(&r[0], 0, sizeof(r));

				//データの受信
				auto hr = recv(client[i], r, sizeof(r), 0);
				if (hr == -1)
				{
					char str[INET_ADDRSTRLEN];
					printf("%sとの接続が切れました\n", inet_ntop(clientAddr[i].sin_family, &clientAddr[i].sin_addr, str, sizeof(str)));
					closesocket(client[i]);
					client[i] = INVALID_SOCKET;

					continue;
				}

				char str[INET_ADDRSTRLEN];
				printf("%sからの受信：%s\n", inet_ntop(clientAddr[i].sin_family, &clientAddr[i].sin_addr, str, sizeof(str)), r);

				//指定IPアドレスに送信
				std::string dummy;
				for (int i = 0; r[i] != '/' && i < _countof(r); ++i)
				{
					dummy.push_back(r[i]);
				}
				for (int i = 0; i < CLIENT_MAX; ++i)
				{
					if (inet_addr(dummy.c_str()) == clientAddr[i].sin_addr.S_un.S_addr)
					{
						std::string p;
						p = dummy + "：";
						for (int n = dummy.size() + 1; n < _countof(r); ++n)
						{
							p.push_back(r[n]);
						}
						if (send(client[i], p.c_str(), sizeof(r), 0) == SOCKET_ERROR)
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
}

// 送信
void Server::Send(void)
{
	//入力バッファのクリア
	fflush(stdin);

	memset(&s[0], 0, sizeof(s));
	scanf_s("%s", &s, sizeof(s));
	for (UINT i = 0; i < CLIENT_MAX; ++i)
	{
		if (client[i] == INVALID_SOCKET)
		{
			continue;
		}
		
		if (send(client[i], s, sizeof(s), 0) == SOCKET_ERROR)
		{
			char str[INET_ADDRSTRLEN];
			printf("%sへの送信：失敗\n", inet_ntop(clientAddr[i].sin_family, &clientAddr[i].sin_addr, str, sizeof(str)));
		}
	}
}

// クライアントとの接続終了
void Server::Close(void)
{
	for (unsigned int i = 0; i < CLIENT_MAX; ++i)
	{
		if (client[i] != INVALID_SOCKET)
		{
			closesocket(client[i]);
			client[i] = INVALID_SOCKET;
		}
	}
}

