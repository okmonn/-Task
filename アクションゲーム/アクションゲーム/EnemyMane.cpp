#include "EnemyMane.h"
#include "Deadman.h"
#include "Bat.h"
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
std::shared_ptr<Enemy> EnemyMane::CreateDeadman(float x, float y, std::shared_ptr<Player>pl)
{
	Positionf pos = { x, y };
	std::shared_ptr<Deadman>man;
	man = std::make_shared<Deadman>(pos, pl);
	return man;
}

// �o�b�h�̐���
std::shared_ptr<Enemy> EnemyMane::CreateBat(float x, float y, std::shared_ptr<Player> pl)
{
	Positionf pos = { x, y };
	std::shared_ptr<Bat>bat;
	bat = std::make_shared<Bat>(pos, pl);
	return bat;
}
