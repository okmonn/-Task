#include "Load.h"

Load* Load::s_Instance = nullptr;

// �R���X�g���N�^
Load::Load()
{
}

// �f�X�g���N�^
Load::~Load()
{
}

// �C���X�^���X��
void Load::Create(void)
{
	if (s_Instance == nullptr)
	{
		s_Instance = new Load;
	}
}

// �j��
void Load::Destroy(void)
{
	if (s_Instance != nullptr)
	{
		delete s_Instance;
	}
	s_Instance = nullptr;
}

// �ǂݍ���
bool Load::LoadAct(std::string fileName)
{
	FILE file;
	
	return false;
}
