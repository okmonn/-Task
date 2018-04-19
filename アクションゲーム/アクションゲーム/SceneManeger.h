#pragma once
class SceneManeger
{
public:
	// �f�X�g���N�^
	~SceneManeger();

	// �C���X�^���X��
	void Create(void);

	// �j��
	void Destroy(void);

	// �C���X�^���X�ϐ��擾
	static SceneManeger* GetInstance()
	{
		return s_Instance;
	}

private:
	// �R���X�g���N�^
	SceneManeger();

	// �C���X�^���X�ϐ�
	static SceneManeger* s_Instance;
};

