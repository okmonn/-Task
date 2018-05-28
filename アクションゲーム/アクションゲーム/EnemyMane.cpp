#include "EnemyMane.h"
#include "Deadman.h"
#include "Bat.h"
#include "Spear.h"
#include "Typedef.h"

EnemyMane* EnemyMane::instance = nullptr;

// コンストラクタ
EnemyMane::EnemyMane()
{
}

// デストラクタ
EnemyMane::~EnemyMane()
{
}

// インスタンス化
void EnemyMane::Create(void)
{
	if (instance == nullptr)
	{
		instance = new EnemyMane;
	}
}

// 破棄
void EnemyMane::Destroy(void)
{
	if (instance != nullptr)
	{
		delete instance;
		instance = nullptr;
	}
}

// デッドマンの生成
std::shared_ptr<Enemy> EnemyMane::CreateDeadman(float x, float y, std::shared_ptr<Player>pl, std::shared_ptr<Camera>cam)
{
	Positionf pos = { x, y };
	std::shared_ptr<Deadman>man;
	man = std::make_shared<Deadman>(pos, pl, cam);
	return man;
}

// バッドの生成
std::shared_ptr<Enemy> EnemyMane::CreateBat(float x, float y, std::shared_ptr<Player> pl, std::shared_ptr<Camera>cam)
{
	Positionf pos = { x, y };
	std::shared_ptr<Bat>bat;
	bat = std::make_shared<Bat>(pos, pl, cam);
	return bat;
}

// 槍の生成
std::shared_ptr<Enemy> EnemyMane::CreateSpear(float x, float y, std::shared_ptr<Player> pl, std::shared_ptr<Camera> cam)
{
	Positionf pos = { x, y };
	std::shared_ptr<Spear>sp;
	sp = std::make_shared<Spear>(pos, pl, cam);
	return sp;
}
