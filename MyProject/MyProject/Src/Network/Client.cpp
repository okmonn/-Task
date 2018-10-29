#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "Client.h"
#include <ws2tcpip.h>
#include <Windows.h>
#include <iostream>
#include <algorithm>
#include <tchar.h>

#pragma comment(lib,"ws2_32.lib")

// �R���X�g���N�^
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

// �f�X�g���N�^
Client::~Client()
{
	Close();
	WSACleanup();
}

// �e�L�X�g�̓ǂݍ���
int Client::LoadText(const std::string & fileName)
{
	FILE* file = nullptr;
	if (fopen_s(&file, fileName.c_str(), "rb") != 0)
	{
		OutputDebugString(_T("\n�t�@�C���̎Q�ƁF���s\n"));
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

// ��������
int Client::Start(void)
{
	auto hr = WSAStartup(MAKEWORD(2, 0), &data);
	if (hr != 0)
	{
		OutputDebugString(_T("\n�N�����̏��������F���s\n"));
	}

	return hr;
}

// �\�P�b�g�̐���
int Client::Create(void)
{
	//�g�pPC��
	char name[256];
	//�z�X�g���
	struct hostent* h = nullptr;

	auto hr = gethostname(name, _countof(name));
	if (hr != 0)
	{
		OutputDebugString(_T("\n�R���s���[�^���̎擾�F���s\n"));
		return hr;
	}

	printf("PC���F%s\n", name);
	h = gethostbyname(serverName.c_str());
	printf("�z�X�g���F%s\n", h->h_name);

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
		OutputDebugString(_T("\n�\�P�b�g�̐����F���s\n"));
		return -1;
	}

	return hr;
}

// �T�[�o�[�Ƃ̐ڑ�
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
			printf("%s�ɐڑ����܂���\n", inet_ntop(addr.sin_family, &addr.sin_addr, str, sizeof(str)));
			FD_SET(sock, &readfds);
			break;
		}
	}

	return hr;
}

// ������
void Client::Init(void)
{
	LoadText("Information.txt");
	Start();
	Create();
	Connect();
}

// ��M
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
				printf("�T�[�o�[�������オ���Ă��܂���\n");
			}
			else
			{
				printf("��M�F%s\n", r);
			}
		}
	}
}

// ���M
void Client::Send(void)
{
	fflush(stdin);
	memset(&s[0], 0, sizeof(s));
	scanf_s("%s", &s, sizeof(s));
	if (sendto(sock, s, strlen(s), 0, (struct sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR)
	{
		printf("���M�F���s�F%d\n", WSAGetLastError());
	}
}

// �\�P�b�g�����
void Client::Close(void)
{
	if (sock != INVALID_SOCKET)
	{
		closesocket(sock);
		sock = INVALID_SOCKET;
	}
}
