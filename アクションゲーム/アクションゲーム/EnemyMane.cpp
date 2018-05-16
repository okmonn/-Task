#include "EnemyMane.h"
#include "Typedef.h"

EnemyMane* EnemyMane::instance = nullptr;

// �R���X�g���N�^
EnemyMane::EnemyMane()
{
}

// �f�X�g���N�^
EnemyMane::~EnemyMane()
{
}

// �C���X�^���X��
void EnemyMane::Create(void)
{
	if (instance == nullptr)
	{
		instance = new EnemyMane;
	}
}

// �j��
void EnemyMane::Destroy(void)
{
	if (instance != nullptr)
	{
		delete instance;
		instance = nullptr;
	}
}

// �f�b�h�}���̐���
std::shared_ptr<Deadman> EnemyMane::CreateDeadman(float x, float y, std::shared_ptr<Player>pl)
{
	Positionf pos = { x, y };
	std::shared_ptr<Deadman>man;
	man = std::make_shared<Deadman>(pos, pl);
	return man;
}
