#pragma once
#include "Scene.h"
#include "../BackGround.h"
#include "../Interface.h"
#include "../Input.h"
#include "../Player.h"
#include "../Enemy.h"
#include "../Ground.h"
#include <memory>
#include <list>
class Game :
	public Scene
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

	// �w�i�N���X
	std::shared_ptr<BackGround>back;

	// UI�N���X
	std::shared_ptr<Interface>ui;

	// �C���v�b�g
	std::shared_ptr<Input>in;

	// �v���C���[�N���X
	std::shared_ptr<Player>pl;

	// �G�̃��X�g
	std::list<std::shared_ptr<Enemy>>e_list;

	// �n�ʃN���X
	std::shared_ptr<Ground>ground;
};

