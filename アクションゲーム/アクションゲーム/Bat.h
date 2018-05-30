#pragma once
#include "Enemy.h"
#include "Camera.h"
#include "Player.h"
#include <memory>

class Bat :
	public Enemy
{
public:
	// コンストラクタ
	Bat(Positionf pos, std::weak_ptr<Player>pl, std::weak_ptr<Camera>cam);
	// デストラクタ
	~Bat();

	// 状態の設定
	void SetMode(std::string m, bool r);

	// 描画
	void Draw(void);
	// 処理
	void UpData(void);

	void ChangeDir(void);

private:

	// プレイヤークラス
	std::weak_ptr<Player>pl;

	// カメラクラス
	std::weak_ptr<Camera>cam;

	// テリトリー
	float range;

	// カウント
	int cnt;

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

