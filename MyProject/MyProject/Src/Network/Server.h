#pragma once
#include <WinSock2.h>
#include <string>
#include <vector>

class Server
{
public:
	// �R���X�g���N�^
	Server();
	// �f�X�g���N�^
	~Server();

	// �N���C�A���g�Ƃ̐ڑ�
	void Accept(void);

	// ��M
	void Recv(void);

	// ���M
	void Send(void);

private:
	// ��������
	int Start(void);

	// �\�P�b�g�̐���
	int Create(void);

	// �A�h���X�Ƃ̊֘A
	int Bind(void);

	// �N���C�A���g�Ƃ̐ڑ��ҋ@
	int Listen(void);

	// ������
	void Init(void);

	// �N���C�A���g�Ƃ̐ڑ��I��
	void Close(void);


	// �f�[�^
	WSADATA data;

	// �T�[�o�[�\�P�b�g
	SOCKET sock;

	// �A�h���X
	struct sockaddr_in addr;

	// �N���C�A���g�\�P�b�g
	std::vector<SOCKET>client;

	// �N���C�A���g�A�h���X
	std::vector<struct sockaddr_in>clientAddr;

	// �Z���N�g�Ŏg�p
	fd_set fds;

	// �Z���N�g�Ŏg�p
	fd_set readfds;

	// �^�C���A�E�g
	struct timeval time;

	// ��M����
	std::string r;

	// ���M����
	std::string s;
};
