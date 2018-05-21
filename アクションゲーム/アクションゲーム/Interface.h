#pragma once
#include "Player.h"
#include <memory>

class Interface
{
public:
	// コンストラクタ
	Interface(std::weak_ptr<Player>pl);
	// デストラクタ
	virtual ~Interface();

	// 描画
	void Draw(void);

	// 処理
	void UpData(void);

private:
	// プレイヤークラス
	std::weak_ptr<Player>pl;

	// 上のUI
	int top;

	// 下のUI
	int bottom;

	// プレイヤーの体力
	int hp;
};

