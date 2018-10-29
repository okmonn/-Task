#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "Server.h"
#include <Windows.h>
#include <iostream>
#include <ws2tcpip.h>
#include <tchar.h>

// �N���C�A���g�ő�ڑ���
#define CLIENT_MAX 10

// �����ő吔
#define LENGTH_MAX 1024

// �R���X�g���N�^
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

// �f�X�g���N�^
Server::~Server()
{
	Close();
	closesocket(sock);
	WSACleanup();
}

// ��������
int Server::Start(void)
{
	auto hr = WSAStartup(MAKEWORD(2, 0), &data);
	if (hr != 0)
	{
		OutputDebugString(_T("\n�������F���s\n"));
	}

	return  hr;
}

// �\�P�b�g�̐���
int Server::Create(void)
{
	char port[5];
	printf("�|�[�g�ԍ��F");
	scanf_s("%s", port, sizeof(port));
	//�A�h���X�̐ݒ�
	addr.sin_family           = AF_INET;
	addr.sin_port             = htons(std::atoi(port));
	addr.sin_addr.S_un.S_addr = INADDR_ANY;

	//�\�P�b�g����
	sock = socket(addr.sin_family, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET)
	{
		OutputDebugString(_T("\n�\�P�b�g�̐����F���s\n"));
		return -1;
	}

	BOOL flag = 1;
	//�\�P�b�g�̐ݒ�̃Z�b�g
	auto hr = setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (const char*)&flag, sizeof(flag));
	if (hr != 0)
	{
		OutputDebugString(_T("\n�\�P�b�g�̃Z�b�g�F���s\n"));
	}

	return hr;
}

// �A�h���X�Ƃ̊֘A
int Server::Bind(void)
{
	auto hr = bind(sock, (struct sockaddr *)&addr, sizeof(addr));
	if (hr != 0)
	{
		OutputDebugString(_T("\n�A�h���X�Ƃ̊֘A�F���s\n"));
	}

	return hr;
}

// �N���C�A���g�Ƃ̐ڑ��ҋ@
int Server::Listen(void)
{
	auto hr = listen(sock, 5);
	if (hr != 0)
	{
		OutputDebugString(_T("\n�ڑ��ҋ@�F���s\n"));
	}

	return hr;
}

// ������
void Server::Init(void)
{
	Start();
	Create();
	Bind();
	Listen();
}

// �N���C�A���g�Ƃ̐ڑ�
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
			printf("IP�F%s���ڑ����܂���\n", inet_ntop(clientAddr[i].sin_family, &clientAddr[i].sin_addr, str, sizeof(str)));
		}
	}
}

// ��M
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
				// �T�[�o����f�[�^����M
				memset(&r, 0, sizeof(r));
				auto hr = recv(client[i], (char*)r.data(), sizeof(char) * r.size(), 0);
				if (hr == -1)
				{
					char str[INET_ADDRSTRLEN];
					printf("%s�Ƃ̐ڑ����؂�܂���\n", inet_ntop(clientAddr[i].sin_family, &clientAddr[i].sin_addr, str, sizeof(str)));
					closesocket(client[i]);
					client[i] = INVALID_SOCKET;

					continue;

				}
				
				char str[INET_ADDRSTRLEN];
				printf("%s����̎�M�F%s\n", inet_ntop(clientAddr[i].sin_family, &clientAddr[i].sin_addr, str, sizeof(str)), r.c_str());

				//�w��IP�A�h���X�ɑ��M
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
						p = dummy + "�F";
						for (int o = dummy.size() + 1; o < LENGTH_MAX; ++o)
						{
							p.push_back(r[o]);
						}
						if (send(client[i], p.c_str(), sizeof(char) * r.size(), 0) == SOCKET_ERROR)
						{
							char str[INET_ADDRSTRLEN];
							printf("%s�ւ̑��M�F���s\n", inet_ntop(clientAddr[i].sin_family, &clientAddr[i].sin_addr, str, sizeof(str)));
						}

						break;
					}
				}
			}
		}
	}
	else
	{
		//printf("�^�C���A�E�g\n");
	}
}

// ���M
void Server::Send(void)
{
	//���̓o�b�t�@�̃N���A
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
			printf("%s�ւ̑��M�F���s\n", inet_ntop(clientAddr[i].sin_family, &clientAddr[i].sin_addr, str, sizeof(str)));
		}
	}
}

// �N���C�A���g�Ƃ̐ڑ��I��
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
