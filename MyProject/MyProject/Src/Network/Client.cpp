#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "Client.h"
#include <ws2tcpip.h>
#include <Windows.h>
#include <iostream>
#include <algorithm>
#include <tchar.h>

#pragma comment(lib,"ws2_32.lib")

// コンストラクタ
Client::Client() :
	sock(INVALID_SOCKET), end(false)
{
	data = {};
	addr = {};
	fds = {};
	readfds = {};
	time = { 0, 1000 };

	memset(&r[0], 0, sizeof(r));
	memset(&s[0], 0, sizeof(s));

	list.clear();

	Init();
}

// デストラクタ
Client::~Client()
{
	Close();
	WSACleanup();
}

// テキストの読み込み
int Client::LoadText(const std::string & fileName)
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
	auto end = min(dummy.find_first_of('\r'), dummy.find_first_of('\n'));
	serverName = dummy.substr(pos + 1, end - (pos + 1));

	pos = dummy.find_last_of(':');
	port = dummy.substr(pos + 1, 5);

	return 0;
}

// 初期処理
int Client::Start(void)
{
	auto hr = WSAStartup(MAKEWORD(2, 0), &data);
	if (hr != 0)
	{
		OutputDebugString(_T("\n起動時の初期処理：失敗\n"));
	}

	return hr;
}

// ソケットの生成
int Client::Create(void)
{
	//使用PC名
	char name[256];
	//ホスト情報
	struct hostent* h = nullptr;

	auto hr = gethostname(name, _countof(name));
	if (hr != 0)
	{
		OutputDebugString(_T("\nコンピュータ名の取得：失敗\n"));
		return hr;
	}

	printf("PC名：%s\n", name);
	h = gethostbyname(serverName.c_str());
	printf("ホスト名：%s\n", h->h_name);

	in_addr adr = {};
	for (int i = 0; h->h_addr_list[i]; ++i)
	{
		memcpy(&adr, h->h_addr_list[i], 4);
		list.push_back(adr.S_un.S_addr);
	}

	addr.sin_family = AF_INET;
	addr.sin_port = htons(std::atoi(port.c_str()));
	addr.sin_addr.S_un.S_addr = adr.S_un.S_addr;

	sock = socket(addr.sin_family, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET)
	{
		OutputDebugString(_T("\nソケットの生成：失敗\n"));
		return -1;
	}

	return hr;
}

// サーバーとの接続
int Client::Connect(void)
{
	FD_ZERO(&readfds);
	auto hr = 0;
	for (unsigned int i = 0; i < list.size(); ++i)
	{
		addr.sin_addr.S_un.S_addr = list[i];

		hr = connect(sock, (struct sockaddr *)&addr, sizeof(addr));
		if (hr == 0)
		{
			char str[INET_ADDRSTRLEN];
			printf("%sに接続しました\n", inet_ntop(addr.sin_family, &addr.sin_addr, str, sizeof(str)));
			FD_SET(sock, &readfds);
			break;
		}
	}

	return hr;
}

// 初期化
void Client::Init(void)
{
	LoadText("Information.txt");
	Start();
	Create();
	Connect();
}

// 受信
void Client::Recv(void)
{
	memcpy(&fds, &readfds, sizeof(fd_set));
	int len = select(0, &fds, nullptr, nullptr, &time);
	if (len != 0)
	{
		if (FD_ISSET(sock, &fds))
		{
			memset(&r[0], 0, sizeof(r));
			auto hr = recv(sock, r, sizeof(r), 0);
			if (hr == -1)
			{
				end = true;
				printf("サーバーが立ち上がっていません\n");
			}
			else
			{
				printf("受信：%s\n", r);
			}
		}
	}
}

// 送信
void Client::Send(void)
{
	fflush(stdin);
	memset(&s[0], 0, sizeof(s));
	scanf_s("%s", &s, sizeof(s));
	if (sendto(sock, s, strlen(s), 0, (struct sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR)
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
