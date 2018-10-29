#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "Server.h"
#include <ws2tcpip.h>
#include <Windows.h>
#include <iostream>
#include <tchar.h>

#pragma comment(lib,"ws2_32.lib")

// �N���C�A���g�ő吔
#define CLIENT_MAX 10

// �R���X�g���N�^
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

// �f�X�g���N�^
Server::~Server()
{
	Close();
	closesocket(sock);
	WSACleanup();
}

// �e�L�X�g�̓ǂݍ���
int Server::LoadText(const std::string & fileName)
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
	port = dummy.substr(pos + 1, 5);

	return 0;
}

// ��������
int Server::Start(void)
{
	auto hr = WSAStartup(MAKEWORD(2, 0), &data);
	if (hr != 0)
	{
		OutputDebugString(_T("\n�N�����̏��������F���s\n"));
	}

	return hr;
}

// �\�P�b�g�̐���
int Server::Create(void)
{
	//�A�h���X�̐ݒ�
	addr.sin_family           = AF_INET;
	addr.sin_port             = htons(std::atoi(port.c_str()));
	addr.sin_addr.S_un.S_addr = INADDR_ANY;

	sock = socket(addr.sin_family, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET)
	{
		OutputDebugString(_T("\n�\�P�b�g�̐����F���s\n"));
		return -1;
	}

	//�\�P�b�g�̐ݒ�̃Z�b�g
	int flag = 1;
	auto hr = setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (const char*)&flag, sizeof(flag));
	if (hr != 0)
	{
		OutputDebugString(_T("\n�\�P�b�g�̃Z�b�g�F���s\n"));
	}

	return hr;
}

// �A�h���X�Ƃ̊֘A�t��
int Server::Bind(void)
{
	auto hr = bind(sock, (struct sockaddr *)&addr, sizeof(addr));
	if (hr != 0)
	{
		OutputDebugString(_T("�A�h���X�Ƃ̊֘A�t���F���s\n"));
	}

	return hr;
}

// �N���C�A���g�Ƃ̐ڑ��ҋ@
int Server::Listen(void)
{
	auto hr = listen(sock, 5);
	if (hr != 0)
	{
		OutputDebugString(_T("\n�N���C�A���g�Ƃ̐ڑ��ҋ@�F���s\n"));
	}

	return hr;
}

// ������
void Server::Init(void)
{
	LoadText("Information.txt");
	Start();
	Create();
	Bind();
	Listen();
}

// �N���C�A���g�̐ڑ�
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
			printf("\nIP�F%s���ڑ����܂���\n", inet_ntop(clientAddr[i].sin_family, &clientAddr[i].sin_addr, str, sizeof(str)));
		}
	}
}

// ��M
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

				//�f�[�^�̎�M
				auto hr = recv(client[i], r, sizeof(r), 0);
				if (hr == -1)
				{
					char str[INET_ADDRSTRLEN];
					printf("%s�Ƃ̐ڑ����؂�܂���\n", inet_ntop(clientAddr[i].sin_family, &clientAddr[i].sin_addr, str, sizeof(str)));
					closesocket(client[i]);
					client[i] = INVALID_SOCKET;

					continue;
				}

				char str[INET_ADDRSTRLEN];
				printf("%s����̎�M�F%s\n", inet_ntop(clientAddr[i].sin_family, &clientAddr[i].sin_addr, str, sizeof(str)), r);

				//�w��IP�A�h���X�ɑ��M
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
						p = dummy + "�F";
						for (int n = dummy.size() + 1; n < _countof(r); ++n)
						{
							p.push_back(r[n]);
						}
						if (send(client[i], p.c_str(), sizeof(r), 0) == SOCKET_ERROR)
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
}

// ���M
void Server::Send(void)
{
	//���̓o�b�t�@�̃N���A
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
			printf("%s�ւ̑��M�F���s\n", inet_ntop(clientAddr[i].sin_family, &clientAddr[i].sin_addr, str, sizeof(str)));
		}
	}
}

// �N���C�A���g�Ƃ̐ڑ��I��
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

