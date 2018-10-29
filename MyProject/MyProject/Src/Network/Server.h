#pragma once
#include <winsock2.h>
#include <map>

class Server
{
public:
	// �R���X�g���N�^
	Server();
	// �f�X�g���N�^
	~Server();

private:
	// ��������
	int Start(void);

	// �\�P�b�g�̐���
	int Create(void);


	// �f�[�^
	WSADATA data;

	// �T�[�o�[�\�P�b�g
	SOCKET sock;

	// �A�h���X
	struct sockaddr_in addr;

	// �Z���N�g�Ŏg�p
	fd_set fds;

	// �Z���N�g�Ŏg�p
	fd_set readfds;

	// �^�C���A�E�g
	struct timeval time;

	// �N���C�A���g�\�P�b�g
	std::map<unsigned long, SOCKET>client;

	// �N���C�A���g�A�h���X
	std::map<unsigned long, struct sockaddr_in>clientAddr;
};
