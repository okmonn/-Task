#pragma once
#include "Enemy.h"
#include "Player.h"
#include <memory>

class Bat :
	public Enemy
{
public:
	// コンストラクタ
	Bat(Positionf pos, std::weak_ptr<Player>pl);
	// デストラクタ
	~Bat();

	// 状態の設定
	void SetMode(std::string m, bool r);

	// 描画
	void Draw(void);
	// 処理
	void UpData(void);

private:

	// プレイヤークラス
	std::weak_ptr<Player>pl;

	// テリトリー
	float range;

	// 関数ポインタ
	void(Bat::*func)(void);

	// 待機の処理
	void Wait(void);
	// 飛びの処理
	void Fly(void);
	// ダメージの処理
	void Damage(void);
	// 死亡の処理
	void Die(void);
};

