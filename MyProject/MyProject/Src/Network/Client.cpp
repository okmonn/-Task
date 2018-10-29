#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "Client.h"
#include <Windows.h>
#include <iostream>
#include <ws2tcpip.h>
#include <string>
#include <tchar.h>

#pragma comment(lib, "ws2_32.lib")

// �R���X�g���N�^
Client::Client() : 
	sock(INVALID_SOCKET)
{
	data = {};
	addr = {};
	fds = {};
	readfds = {};

	list.clear();

	Init();
}

// �f�X�g���N�^
Client::~Client()
{
}

// ��M
void Client::Recv(void)
{
}

// ���M
void Client::Sent(void)
{
}

// ��������
int Client::Start(void)
{
	auto hr = WSAStartup(MAKEWORD(2, 0), &data);
	if (hr != 0)
	{
		OutputDebugString(_T("\n�������F���s\n"));
	}

	return  hr;
}

// �\�P�b�g�̐���
int Client::Create(void)
{
	//�g�pPC���O
	char name[256];

	//�z�X�g���
	struct hostent *h = nullptr;

	auto hr = gethostname(name, _countof(name));
	if (hr != 0)
	{
		OutputDebugString(_T("\n�g�p���Ă���PC���̎擾�F���s\n"));
		return hr;
	}
	else
	{
		printf("���̃R���s���[�^�̖��O��%s�ł�\n", name);
	}

	char hName[10];
	printf("�T�[�o�[PC���F");
	scanf_s("%s", hName, sizeof(hName));
	h = gethostbyname(hName);

	char port[5];
	printf("�|�[�g�ԍ��F");
	scanf_s("%s", port, sizeof(port));
	//�A�h���X���
	in_addr address = {};
	for (int i = 0; h->h_addr_list[i]; ++i)
	{
		memcpy(&address, h->h_addr_list[i], 4);
		list.push_back(address.S_un.S_addr);
	}
	
	//�\�P�b�g�̐ݒ�
	addr.sin_family           = AF_INET;
	addr.sin_port             = htons(std::atoi(port));
	addr.sin_addr.S_un.S_addr = address.S_un.S_addr;

	//�\�P�b�g�̐���
	sock = socket(addr.sin_family, SOCK_STREAM, 0);
	if (sock != INVALID_SOCKET)
	{
		OutputDebugString(_T("\n�\�P�b�g�̐����F���s\n"));
	}

	return hr;
}

// �T�[�o�[�Ƃ̐ڑ�
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
			printf("%s�ɐڑ����܂���\n", inet_ntop(addr.sin_family, &addr.sin_addr, str, sizeof(str)));
			FD_SET(sock, &readfds);
			break;
		}
	}

	if (hr != 0)
	{
		printf("�T�[�o�[�ڑ��F���s : %d\n", WSAGetLastError());
	}

	return hr;
}

// ������
void Client::Init(void)
{
	Start();
	Create();
	Connect();
}
