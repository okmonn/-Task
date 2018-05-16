#pragma once
#include "Player.h"
#include "Deadman.h"
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
	std::shared_ptr<Deadman> CreateDeadman(float x, float y, std::shared_ptr<Player>pl);
private:
	// �R���X�g���N�^
	EnemyMane();

	// �C���X�^���X�ϐ�
	static EnemyMane* instance;
};

