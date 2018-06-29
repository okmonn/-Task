#pragma once
#include "Scene.h"

class Game
{
public:
	// �f�X�g���N�^
	~Game();

	// �C���X�^���X�̎擾
	static Game& Instance(void)
	{
		static Game instance;
		return instance;
	}

	// ������
	void Init(void);

	// �N���X�̃C���X�^���X��
	void Create(void);

	// �V�[���̈ڍs
	void ChangeScene(Scene* s);

	// �t���O�̃Z�b�g
	void ChangeFlag(bool flag);

	// ���ꂼ��̃N���X�̕`��
	void Draw(void);

	// ���ꂼ��̃N���X�̏���
	void UpData(void);

	// ���C�����[�v
	void Run(void);

	// �I������
	void Destroy(void);

private:
	// �R���X�g���N�^
	Game();
	Game(const Game&);

	// �C���v�b�g
	std::shared_ptr<Input>in;

	// �V�[���N���X
	std::shared_ptr<Scene>scene;

	bool flag;

};

