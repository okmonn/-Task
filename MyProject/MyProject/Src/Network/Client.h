#pragma once
#include <winsock2.h>
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

	// �I���t���O�̎擾
	bool GetFlag(void) const {
		return end;
	}

private:
	// �e�L�X�g�̓ǂݍ���
	int LoadText(const std::string& fileName);

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


	// �T�[�o�[PC��
	std::string serverName;

	// �|�[�g�ԍ�
	std::string port;

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

	// �I���t���O
	bool end;

	// ��M
	char r[256];

	// ���M
	char s[256];

	// �A�h���X���X�g
	std::vector<unsigned long>list;
};
