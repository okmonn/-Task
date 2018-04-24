#pragma once
#include "Typedef.h"
#include <vector>
#include <string>

class Load
{
public:
	// �f�X�g���N�^
	~Load();

	// �C���X�^���X��
	static void Create(void);
	// �j��
	static void Destroy(void);

	// �C���X�^���X�ϐ��̎擾
	static Load* GetInstance(void)
	{
		return s_Instance;
	}

	// �ǂݍ���
	bool LoadAct(std::string fileName);

private:
	// �R���X�g���N�^
	Load();

	// �C���X�^���X�ϐ�
	static Load* s_Instance;

	ImageHeader header;
	std::vector<ImageData>data;
};