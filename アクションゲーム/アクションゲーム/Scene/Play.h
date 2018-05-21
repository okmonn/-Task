#pragma once
#include "Scene.h"
#include "../BackGround.h"
#include "../Player.h"
#include "../Interface.h"
#include "../Enemy.h"
#include "../Ground.h"
#include <list>

class Play :
	public Scene
{
public:
	// �R���X�g���N�^
	Play(std::weak_ptr<Input>in);
	// �f�X�g���N�^
	~Play();

	// �C���X�^���X��
	void Create(void);

	// ����
	void Draw(void);

	// ����
	void UpData(void);

private:
	// �w�i�N���X
	std::shared_ptr<BackGround>back;

	// �v���C���[�N���X
	std::shared_ptr<Player>pl;

	// UI�N���X
	std::shared_ptr<Interface>ui;

	// �G�̃��X�g
	std::list<std::shared_ptr<Enemy>>e_list;

	// �n�ʃN���X
	std::shared_ptr<Ground>ground;
};