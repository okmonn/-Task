#pragma once
#include "Camera.h"
#include "Player.h"
#include "Enemy.h"
#include <memory>

class EnemyMane
{
public:
	// デストラクタ
	~EnemyMane();

	// インスタンス化
	static void Create(void);
	// 破棄
	static void Destroy(void);

	// インスタンス変数の取得
	static EnemyMane* GetInstance(void)
	{
		return instance;
	}

	// デッドマンの生成
	std::shared_ptr<Enemy> CreateDeadman(float x, float y, std::shared_ptr<Player>pl, std::shared_ptr<Camera>cam);
	// バッドの生成
	std::shared_ptr<Enemy> CreateBat(float x, float y, std::shared_ptr<Player>pl, std::shared_ptr<Camera>acm);
private:
	// コンストラクタ
	EnemyMane();

	// インスタンス変数
	static EnemyMane* instance;
};

