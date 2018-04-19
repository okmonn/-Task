#include "SceneManeger.h"

//�C���X�^���X�ϐ��̏�����
SceneManeger* SceneManeger::s_Instance = nullptr;

// �R���X�g���N�^
SceneManeger::SceneManeger()
{
}

// �f�X�g���N�^
SceneManeger::~SceneManeger()
{
}

// �C���X�^���X��
void SceneManeger::Create(void)
{
	if (s_Instance == nullptr)
	{
		s_Instance = new SceneManeger;
	}
}

// �j��
void SceneManeger::Destroy(void)
{
	if (s_Instance != nullptr)
	{
		delete s_Instance;
	}

	s_Instance = nullptr;
}
