#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "Server.h"
#include <Windows.h>
#include <tchar.h>

// �R���X�g���N�^
Server::Server() : 
	sock(INVALID_SOCKET)
{
	data = {};
	addr = {};
	fds = {};
	readfds = {};
	time = { 0, 1000 };

	client.clear();
	clientAddr.clear();
}

// �f�X�g���N�^
Server::~Server()
{
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
	char port[5] = {};
	printf("�ڑ��|�[�g�ԍ��F");
	scanf_s("%5s", port, sizeof(port));

	//�A�h���X�̐ݒ�
	addr.sin_family           = AF_INET;
	addr.sin_port             = htons(std::atoi(port));
	addr.sin_addr.S_un.S_addr = INADDR_ANY;

	return 0;
}
