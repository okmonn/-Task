#pragma once
#include <WinSock2.h>
#include <string>
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
	void Send(void);

	// �T�[�o�[�̔F���t���O�̎擾
	bool GetFlag(void) const {
		return connecting;
	}

private:
	// ��������
	int Start(void);

	// �\�P�b�g�̐���
	int Create(void);

	// �T�[�o�[�Ƃ̐ڑ�
	int Connect(void);

	// ������
	void Init(void);

	// �\�P�b�g�����
	void Close(void);


	// �f�[�^
	WSADATA data;

	// �\�P�b�g
	SOCKET sock;

	// �A�h���X
	struct sockaddr_in addr;

	// �T�[�o�[�̃A�h���X�z��
	std::vector<unsigned long>list;

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

	// �T�[�o�[�̔F���t���O
	bool connecting;
};
