#pragma once
#include "Enemy.h"
#include "Camera.h"
#include "Player.h"
#include <memory>

class Spear :
	public Enemy
{
public:
	// コンストラクタ
	Spear(Positionf pos, std::weak_ptr<Player>pl, std::weak_ptr<Camera>cam);
	// デストラクタ
	~Spear();

	// 状態のセット
	void SetMode(std::string m, bool r);
	// 描画
	void Draw(void);
	// 処理
	void UpData(void);

private:
	// プレイヤークラス
	std::weak_ptr<Player>pl;

	// カメラ
	std::weak_ptr<Camera>cam;

	Positionf camPos;

	// 関数ポインタ
	void(Spear::*func)();

	// 攻撃の処理
	void Attacker(void);
};

