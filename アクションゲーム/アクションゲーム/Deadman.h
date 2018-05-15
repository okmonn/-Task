#pragma once
#include "Enemy.h"
#include "Player.h"
#include <memory>

class Deadman :
	public Enemy
{
public:
	// コンストラクタ
	Deadman();
	Deadman(Positionf pos, std::weak_ptr<Player>pl);
	// デストラクタ
	~Deadman();

	// 状態の設定
	void SetMode(std::string m, bool r);

	// 描画
	void Draw(void);

	// 処理
	void UpData(void);

private:
	// プレイヤークラス
	std::weak_ptr<Player>pl;

	float down;

	int wait;

	int go;

	bool dir[2];

	// あたり判定
	bool CheackHit(Positionf& pos1, Attack& a1, Positionf& pos2, Attack& a2);
	// 歩きの処理
	void Walk(void);
	// 死亡の処理
	void Die(void);

	// 関数ポインタ
	void(Deadman::*func)(void);
};

