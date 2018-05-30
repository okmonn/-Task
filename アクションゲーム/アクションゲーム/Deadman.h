#pragma once
#include "Enemy.h"
#include "Camera.h"
#include "Player.h"
#include <memory>

class Deadman :
	public Enemy
{
public:
	// コンストラクタ
	Deadman();
	Deadman(Positionf pos, std::weak_ptr<Player>pl, std::weak_ptr<Camera>cam);
	// デストラクタ
	~Deadman();

	// 状態の設定
	void SetMode(std::string m, bool r);

	// 描画
	void Draw(void);

	// 処理
	void UpData(void);

	void ChangeDir(void);

	Positionf GetCamPos(void);
	CutData GetCut(void);

private:
	// プレイヤークラス
	std::weak_ptr<Player>pl;

	// カメラクラス
	std::weak_ptr<Camera>cam;

	float down;

	int wait;

	int go;

	bool dir[2];

	Positionf camPos;

	// 歩きの処理
	void Walk(void);
	// 死亡の処理
	void Die(void);

	// 関数ポインタ
	void(Deadman::*func)(void);
};

