#pragma once
#include <WinSock2.h>
#include <vector>

class Client
{
public:
	// �R���X�g���N�^
	Client();
	// �f�X�g���N�^
	~Client();

	// ��M
	void Recv(void);

	// ���M
	void Sent(void);

private:
	// ��������
	int Start(void);

	// �\�P�b�g�̐���
	int Create(void);

	// �T�[�o�[�Ƃ̐ڑ�
	int Connect(void);

	// ������
	void Init(void);


	// �f�[�^
	WSADATA data;

	// �\�P�b�g
	SOCKET sock;

	// �A�h���X
	struct sockaddr_in addr;

	// �T�[�o�[�̃A�h���X�z��
	std::vector<unsigned long>list;

	// 
	fd_set fds;

	// 
	fd_set readfds;
};
