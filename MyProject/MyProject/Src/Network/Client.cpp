#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "Client.h"
#include <Windows.h>
#include <iostream>
#include <ws2tcpip.h>
#include <tchar.h>

#pragma comment(lib, "ws2_32.lib")

// 文字最大数
#define LENGTH_MAX 1024

// コンストラクタ
Client::Client() : 
	sock(INVALID_SOCKET), connecting(false)
{
	data = {};
	addr = {};
	fds = {};
	readfds = {};

	list.clear();

	r.resize(LENGTH_MAX);
	s.resize(LENGTH_MAX);

	Init();
}

// デストラクタ
Client::~Client()
{
	Close();
	WSACleanup();
}

// 初期処理
int Client::Start(void)
{
	auto hr = WSAStartup(MAKEWORD(2, 0), &data);
	if (hr != 0)
	{
		OutputDebugString(_T("\n初期化：失敗\n"));
	}

	return  hr;
}

// ソケットの生成
int Client::Create(void)
{
	//使用PC名前
	char name[256];

	//ホスト情報
	struct hostent *h = nullptr;

	auto hr = gethostname(name, _countof(name));
	if (hr != 0)
	{
		OutputDebugString(_T("\n使用しているPC名の取得：失敗\n"));
		return hr;
	}
	else
	{
		printf("このコンピュータの名前は%sです\n", name);
	}

	std::string serverName;
	serverName.resize(256);
	printf("サーバーPC名：");
	scanf_s("%s", (char*)serverName.data(), sizeof(char) * serverName.size());
	h = gethostbyname(serverName.c_str());

	std::string port;
	port.resize(5);
	printf("ポート番号：");
	scanf_s("%s", (char*)port.data(), sizeof(char) * port.size());
	//アドレス情報
	in_addr address = {};
	for (int i = 0; h->h_addr_list[i]; ++i)
	{
		memcpy(&address, h->h_addr_list[i], 4);
		list.push_back(address.S_un.S_addr);
	}
	
	//ソケットの設定
	addr.sin_family           = AF_INET;
	addr.sin_port             = htons(std::atoi(port.c_str()));
	addr.sin_addr.S_un.S_addr = address.S_un.S_addr;

	//ソケットの生成
	sock = socket(addr.sin_family, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET)
	{
		OutputDebugString(_T("\nソケットの生成：失敗\n"));
	}

	return hr;
}

// サーバーとの接続
int Client::Connect(void)
{
	FD_ZERO(&readfds);
	auto hr = 0;
	for (UINT i = 0; i < list.size(); ++i)
	{
		addr.sin_addr.S_un.S_addr = list[i];

		hr = connect(sock, (struct sockaddr *)&addr, sizeof(addr));
		if (hr != 0)
		{
			continue;
		}
		else
		{
			char str[INET_ADDRSTRLEN];
			printf("%sに接続しました\n", inet_ntop(addr.sin_family, &addr.sin_addr, str, sizeof(str)));
			FD_SET(sock, &readfds);
			connecting = true;
			break;
		}
	}

	if (hr != 0)
	{
		printf("サーバー接続：失敗 : %d\n", WSAGetLastError());
	}

	return hr;
}

// 初期化
void Client::Init(void)
{
	Start();
	Create();
	Connect();
}

// 受信
void Client::Recv(void)
{
	memcpy(&fds, &readfds, sizeof(fd_set));
	int len = select(0, &fds, NULL, NULL, &time);
	if (len != 0)
	{
		if (FD_ISSET(sock, &fds))
		{
			auto hr = recv(sock, (char*)r.data(), sizeof(char) * r.size(), 0);
			if (hr == -1)
			{
				printf("サーバーが立ち上がっていません\n");
				connecting = false;
			}
			else
			{
				printf("受信：%s\n", r.c_str());
			}
		}
	}
}

// 送信
void Client::Sent(void)
{
	fflush(stdin);
	scanf_s("%s", s.data(), sizeof(char) * s.size());
	if (sendto(sock, s.data(), strlen(s.data()), 0, (struct sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR)
	{
		printf("送信：失敗：%d\n", WSAGetLastError());
	}
}

// ソケットを閉じる
void Client::Close(void)
{
	if (sock != INVALID_SOCKET)
	{
		closesocket(sock);
		sock = INVALID_SOCKET;
	}
}
