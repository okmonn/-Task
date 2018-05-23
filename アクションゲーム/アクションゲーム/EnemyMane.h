#pragma once
#include "Camera.h"
#include "Player.h"
#include "Enemy.h"
#include <memory>

class EnemyMane
{
public:
	// �f�X�g���N�^
	~EnemyMane();

	// �C���X�^���X��
	static void Create(void);
	// �j��
	static void Destroy(void);

	// �C���X�^���X�ϐ��̎擾
	static EnemyMane* GetInstance(void)
	{
		return instance;
	}

	// �f�b�h�}���̐���
	std::shared_ptr<Enemy> CreateDeadman(float x, float y, std::shared_ptr<Player>pl, std::shared_ptr<Camera>cam);
	// �o�b�h�̐���
	std::shared_ptr<Enemy> CreateBat(float x, float y, std::shared_ptr<Player>pl, std::shared_ptr<Camera>acm);
private:
	// �R���X�g���N�^
	EnemyMane();

	// �C���X�^���X�ϐ�
	static EnemyMane* instance;
};

