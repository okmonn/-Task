#pragma once
#include "Event.h"
#include "Camera.h"
#include "Player.h"
class Ladder :
	public Event
{
public:
	// コンストラクタ
	Ladder(Positionf pos, std::shared_ptr<Player>pl, std::shared_ptr<Camera>cam);
	// デストラクタ
	~Ladder();

	// 描画
	void Draw(void);

	// 処理
	void UpData(void);

private:
	// カメラ　
	std::weak_ptr<Camera>cam;

	// プレイヤー
	std::weak_ptr<Player>pl;

	// 座標
	Positionf pos;
};

